#version 420
layout(quads, fractional_even_spacing) in;

// The code related to the 3d simplex noise come from:
// https://www.shadertoy.com/view/XsX3zB

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

///* skew constants for 3d simplex functions */
const float F3 =  0.3333333;
const float G3 =  0.1666667;

///* const matrices for 3d rotation */
const mat3 rot1 = mat3(-0.37, 0.36, 0.85,-0.14,-0.93, 0.34,0.92, 0.01,0.4);
const mat3 rot2 = mat3(-0.55,-0.39, 0.74, 0.33,-0.91,-0.24,0.77, 0.12,0.63);
const mat3 rot3 = mat3(-0.71, 0.52,-0.47,-0.08,-0.72,-0.68,-0.7,-0.45,0.56);

// Uniforms
uniform float Time;
uniform	mat4 M;
uniform	mat4 V;
uniform	mat4 P;
uniform	mat4 MV;
uniform	mat4 MVP;
uniform	mat3 N;
uniform	unsigned int waveSize;

uniform Light Lights[3];
uniform Mat Material;
uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

// Inputs\outputs
in vec3 cPosition[];
out vec3 colorOut;
out vec3 normal;

out vec3 fragLight0Vect;
out vec3 fragLight1Vect;
out vec3 fragLight2Vect;

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

///* 3d simplex noise */
float simplex3d(vec3 p) {
	 /* 1. find current tetrahedron T and it's four vertices */
	 /* s, s+i1, s+i2, s+1.0 - absolute skewed (integer) coordinates of T vertices */
	 /* x, x1, x2, x3 - unskewed coordinates of p relative to each of T vertices*/
	 
	 /* calculate s and x */
	 vec3 s = floor(p + dot(p, vec3(F3)));
	 vec3 x = p - s + dot(s, vec3(G3));
	 
	 /* calculate i1 and i2 */
	 vec3 e = step(vec3(0.0), x - x.yzx);
	 vec3 i1 = e*(1.0 - e.zxy);
	 vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	 /* x1, x2, x3 */
	 vec3 x1 = x - i1 + G3;
	 vec3 x2 = x - i2 + 2.0*G3;
	 vec3 x3 = x - 1.0 + 3.0*G3;
	 
	 /* 2. find four surflets and store them in d */
	 vec4 w, d;
	 
	 /* calculate surflet weights */
	 w.x = dot(x, x);
	 w.y = dot(x1, x1);
	 w.z = dot(x2, x2);
	 w.w = dot(x3, x3);
	 
	 /* w fades from 0.6 at the center of the surflet to 0.0 at the margin */
	 w = max(0.6 - w, 0.0);
	 
	 /* calculate surflet components */
	 d.x = dot(random3(s), x);
	 d.y = dot(random3(s + i1), x1);
	 d.z = dot(random3(s + i2), x2);
	 d.w = dot(random3(s + 1.0), x3);
	 
	 /* multiply d by w^4 */
	 w *= w;
	 w *= w;
	 d *= w;
	 
	 /* 3. return the sum of the four surflets */
	 return dot(d, vec4(52.0));
}

///* directional artifacts can be reduced by rotating each octave */
float simplex3d_fractal(vec3 m) {
    return   0.5333333*simplex3d(m*rot1)
			+0.2666667*simplex3d(2.0*m*rot2)
			+0.1333333*simplex3d(4.0*m*rot3)
			+0.0666667*simplex3d(8.0*m);
}

vec3 interpole( vec3 v0, vec3 v1, vec3 v2, vec3 v3 )
{
    vec3 v01 = mix( v0, v1, gl_TessCoord.x );
    vec3 v32 = mix( v3, v2, gl_TessCoord.x );
    return mix( v01, v32, gl_TessCoord.y );
}

vec4 height( vec4 pos )
{
	vec2 worldPos = (M * pos).xz / 500.f;
    vec3 params = vec3(worldPos, Time * 0.003);

    float height = simplex3d_fractal(params * 20 + 20);
	height = 0.5 + 0.5 * height;
	float heightVal = height * waveSize;
	return M * (pos + vec4(0 , heightVal , 0 , 0));
}

vec3 getNormal(vec3 ws_p1, vec3 ws_p2, vec3 ws_p3)
{
    vec4 edge1 = normalize((vec4(ws_p1, 2) - vec4(ws_p2, 1)));
    vec4 edge2 = normalize((vec4(ws_p3, 3) - vec4(ws_p2, 2)));
    vec3 e1 = vec3(edge1.x, edge1.y, edge1.z);
    vec3 e2 =  vec3(edge2.x, edge2.y, edge2.z);

    return normalize(cross(e1, e2));
}

out vec3 Light0HV;
out vec3 Light1HV;
out vec3 Light2HV;

void main()
{
    vec3 p0 = cPosition[0];
    vec3 p1 = cPosition[1];
    vec3 p2 = cPosition[2];
    vec3 p3 = cPosition[3];
    vec4 pos = vec4(interpole( p0, p1, p2, p3 ), 1);

	vec4 posInterpol = height(pos);
	vec4 posInterpolXP = height(pos + vec4(0.1, 0, 0, 0));
	vec4 posInterpolZP = height(pos + vec4(0, 0, 0.1, 0));

    gl_Position = P * V * posInterpol;

	normal = mat3(V) * getNormal(posInterpolZP.xyz, posInterpol.xyz, posInterpolXP.xyz).xyz;

	vec4 ecPosition = V * posInterpol;
	vec3 ecPosition3;
	ecPosition3 = (vec3 (ecPosition)) / ecPosition.w;
	fragLight0Vect = vec3 (Lights[0].Position) - ecPosition3;
    fragLight1Vect = vec3 (Lights[1].Position) - ecPosition3;
    fragLight2Vect = vec3 (-Lights[2].Position);

	Light0HV = normalize(fragLight0Vect - ecPosition.xyz);
	Light1HV = normalize(fragLight1Vect - ecPosition.xyz);
	Light2HV = normalize(-fragLight2Vect);
}