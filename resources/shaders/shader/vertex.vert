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
//uniform vec2 screenSize;


out vec3 Color;
out vec2 Texcoord;
out vec3 lightColor;
out vec3 Normal;
out vec3 FragPos;
out vec3 viewPos;
out vec4 FragPosLightSpace;
out float u_Time;

uniform mat4 model      =mat4(1);
//uniform float time;
uniform mat4 lightSpaceMatrix =mat4(1);;

void main()
{
	lightColor = alightColor;
	viewPos = aviewPos;
	//u_Time = time;
	FragPos = vec3(model * vec4(aPos, 1.0));	
	Normal = mat3(transpose(inverse(model))) * aNormal; 
    Texcoord = aTexCoords; 
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
	
}
