#version 430 core

struct Light
{
        vec3 Ambient; 
        vec3 Diffuse;
        vec3 Specular;
        vec4 Position;  // Si .w = 1.0 -> Direction de lumiere directionelle.
        vec3 SpotDir;
        float SpotExp;
        float SpotCutoff;
        vec3 Attenuation; //Constante, Lineraire, Quadratique
};

struct Mat
{
        vec4 Ambient; 
        vec4 Diffuse;
        vec4 Specular;
        vec4 Exponent;
        float Shininess;
};

uniform Light Lights[3];
uniform Mat Material;
uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

in vec3 normal;

in vec3 Light0HV;
in vec3 Light1HV;
in vec3 Light2HV;

in vec3 fragLight0Vect;
in vec3 fragLight1Vect;
in vec3 fragLight2Vect;

// Déclaration des variables de sortie
layout(location = 0) out vec4 fragColor;

// Accumulateurs pour les facteurs d'éclairage
vec4 Ambient;
vec4 Diffuse;
vec4 specular;

// Calcule la spécularité d'une source lumineuse
vec4 lightSpec(in int i, in vec3 normal, in vec3 halfVector, in float shininess)
{
    // À compléter
    float NdotH = max(0.0, dot(normal, halfVector));
    return  vec4(Lights[i].Specular * pow(NdotH, shininess), 1.0);
}

void pointLight(in vec3 lightVect)
{
   float nDotVP;       // Produit scalaire entre VP et la normale
   float attenuation;  // facteur d'atténuation calculé
   float d;            // distance entre lumière et fragment
   vec3  VP;           // Vecteur lumière

   // Calculer vecteur lumière
   VP = lightVect;

   // Calculer distance à la lumière
   d = length(VP);

   // Normaliser VP
   VP = normalize(VP);

   // Calculer l'atténuation due à la distance
   attenuation = 1.0 / (Lights[0].Attenuation.x +
       Lights[0].Attenuation.y * d +
       Lights[0].Attenuation.z * d * d);

   nDotVP = max(0.0, dot(normal, VP));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[0].Ambient, 1.0) * attenuation;
   Diffuse  += vec4(Lights[0].Diffuse, 1.0) * nDotVP * attenuation;
   specular +=  lightSpec(0, normal, Light0HV, Material.Shininess);
}


// Calcul pour une lumière directionnelle
void directionalLight(in vec3 lightVect)
{
   float nDotVP;         // Produit scalaire entre VP et la normale

   nDotVP = max(0.0, dot(normal, normalize(vec3 (-Lights[2].Position))));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[2].Ambient,1.0);
   Diffuse  += vec4(Lights[2].Diffuse,1.0) * nDotVP;
   specular +=  lightSpec(2, normal, Light2HV, Material.Shininess);
}


// Calcul pour une lumière "spot"
void spotLight(in vec3 lightVect)
{
   float nDotVP;            // Produit scalaire entre VP et la normale
   float spotDot;           // Cosinus angle entre VP et direction spot
   float spotAttenuation;   // Facteur d'atténuation du spot
   float attenuation;       // Facteur d'atténuation du à la distance
   float lightToSurfaceAngle;// 
   float d;                 // Distance à la lumière
   vec3  VP;                // Vecteur lumière

   // Calculer le vecteur Lumière
   VP = lightVect;

   // Calculer la distance à al lumière
   d = length(VP);

   // Normaliser VP
   VP = normalize(VP);

   // Calculer l'atténuation due à la distance
   attenuation = 1.0 / (Lights[1].Attenuation.x +
       Lights[1].Attenuation.y * d +
       Lights[1].Attenuation.z * d * d);

   // Le fragment est-il à l'intérieur du cône de lumière ?
   spotDot = dot(-VP, normalize(Lights[1].SpotDir));
   lightToSurfaceAngle = degrees(acos(spotDot));

   if (lightToSurfaceAngle > Lights[1].SpotCutoff)
   {
       spotAttenuation = 0.0; // en dehors... aucune contribution
   }
   else
   {
       spotAttenuation = pow(spotDot, Lights[1].SpotExp);

   }

   // Combine les atténuation du spot et de la distance
   attenuation *= spotAttenuation;

   nDotVP = max(0.0, dot(normal, VP));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[1].Ambient, 1.0) * attenuation;
   Diffuse  += vec4(Lights[1].Diffuse, 1.0) * nDotVP * attenuation;
   specular +=  lightSpec(1, normal, Light1HV, Material.Shininess) * attenuation;
}


vec4 flight()
{
    vec4 color;

    // Réinitialiser les accumulateurs
    Ambient  = vec4 (0.0);
    Diffuse  = vec4 (0.0);
    specular = vec4(0.0);

    if (pointLightOn == 1) {
        pointLight(fragLight0Vect);
    }

    if (dirLightOn == 1) {
        directionalLight(fragLight2Vect);
    }
    
    if (spotLightOn == 1) {
        spotLight(fragLight1Vect);
    }

    color = Ambient * Material.Ambient
            + Diffuse  * Material.Diffuse 
            + specular * Material.Specular;
    color = clamp( color, 0.0, 1.0 );
    return color;
}

void main () {

	fragColor = flight();
}