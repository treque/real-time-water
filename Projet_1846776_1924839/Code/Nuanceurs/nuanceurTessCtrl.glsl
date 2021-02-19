#version 420

layout(vertices = 4) out;

uniform	mat4 M;
uniform	mat4 P;
uniform	mat4 MV;
uniform	mat4 MVP;
uniform vec3 eyePos;

uniform float tscale_negx;
uniform float tscale_negz;
uniform float tscale_posx;
uniform float tscale_posz;

in vec3 vPosition[];
out vec3 cPosition[];

// The code related to the tesselation level adjustements come from:
// https://bitbucket.org/victorbush/ufl.cap5705.terrain/src/master/

float dlodCameraDistance(vec4 p0, vec4 p1)
{

	vec4 view0 = MV * p0;
	vec4 view1 = MV * p1;

	float MinDepth = 10.0;
	float MaxDepth = 100000.0;

	float d0 = clamp( (abs(p0.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
	float d1 = clamp( (abs(p1.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);

	float t = mix(64, 2, (d0 + d1) * 0.5);

	if (t <= 2.0)
	{ 
		return 2.0;
	}
	if (t <= 4.0)
	{ 
		return 4.0;
	}
	if (t <= 8.0)
	{ 
		return 8.0;
	}
	if (t <= 16.0)
	{ 
		return 16.0;
	}
	if (t <= 32.0)
	{ 
		return 32.0;
	}
	
	return 64.0;
}

void main(void)
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    cPosition[gl_InvocationID] = vPosition[gl_InvocationID];

    if ( gl_InvocationID == 0 )
    {
        vec3 eyeWorldPos = eyePos;

        gl_TessLevelOuter[0] = dlodCameraDistance(gl_in[3].gl_Position, gl_in[0].gl_Position);
	    gl_TessLevelOuter[1] = dlodCameraDistance(gl_in[0].gl_Position, gl_in[1].gl_Position);
	    gl_TessLevelOuter[2] = dlodCameraDistance(gl_in[1].gl_Position, gl_in[2].gl_Position);
	    gl_TessLevelOuter[3] = dlodCameraDistance(gl_in[2].gl_Position, gl_in[3].gl_Position);

	    if (tscale_posz == 2.0)
	    	gl_TessLevelOuter[0] = max(2.0, gl_TessLevelOuter[0] * 0.5);
	    if (tscale_posx == 2.0)
	    	gl_TessLevelOuter[1] = max(2.0, gl_TessLevelOuter[1] * 0.5);
	    if (tscale_negz == 2.0)
	    	gl_TessLevelOuter[2] = max(2.0, gl_TessLevelOuter[2] * 0.5);
	    if (tscale_negx == 2.0)
	    	gl_TessLevelOuter[3] = max(2.0, gl_TessLevelOuter[3] * 0.5);

        gl_TessLevelInner[0] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]) / 2;
        gl_TessLevelInner[1] = (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]) / 2;
    }
}