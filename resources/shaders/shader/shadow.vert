#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix =mat4(1);;
uniform mat4 model =mat4(1);;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}