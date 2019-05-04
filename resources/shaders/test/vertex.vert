#version 420 core

//layout (location = 0) in vec4 vertex;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
//UBO to share over diffrent shaders
layout(std140, binding = 0) uniform Matrices{
	mat4 projection;
	mat4 view;
};

uniform vec3 alightColor;
uniform vec3 aviewPos;

out vec3 Color;
out vec2 Texcoord;
out vec3 lightColor;
out vec3 Normal;
out vec3 FragPos;
out vec3 viewPos;
out vec4 FragPosLightSpace;
out float time;

uniform mat4 model      =mat4(1);
uniform mat4 lightSpaceMatrix =mat4(1);;

uniform sampler2D tex_heightmap; 




float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 5

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    // Rotate to reduce axial bias
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = _st * 2.0;
        a *= 0.5;
    }
    return v;
}




void main()
{

	float normalizedHeight;
	float height;

	mat4 MVP = projection * view * model;
	lightColor = alightColor;
	viewPos = aviewPos;
	FragPos = vec3(model * vec4(aPos, 1.0));	
	Normal = mat3(transpose(inverse(model))) * aNormal; 
    Texcoord = aTexCoords; 
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);


	height = texture2D(tex_heightmap,aPos.xz).x;
	height *= 5;


	gl_Position = MVP * vec4(aPos.x, aPos.y + height, aPos.z, 1.0); 
	
}
