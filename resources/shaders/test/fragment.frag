#version 330 core
#define NR_POINTLIGHTS 99


struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct DirLight {
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight; 

struct PointLight {
	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights[NR_POINTLIGHTS];

in vec2 Texcoord;
in vec3 lightColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPos;
in vec3 viewPos;
in vec4 FragPosLightSpace;

uniform int gNumOfLights;

out vec4 FragColor;
uniform sampler2D texOne;
uniform sampler2D stoneTex;

uniform sampler2D shadowMap;
uniform float u_Time;


vec3 DirectionalLight(DirLight light, vec3 normal, vec3 viewDir){

	vec3 lightDir = normalize(-light.direction);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * (spec * material.specular);  

	return ambient + diffuse + specular;
}


vec3 PointLightCalc(PointLight pointlight, vec3 norm, vec3 fragPos, vec3 viewDir){
float distance    = length(pointlight.position - FragPos);
float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * (distance * distance));   


    vec3 lightDir = normalize(pointlight.position - fragPos);

	//Diffuse
	float diff = max(dot(norm, lightDir), 0.0);

	float specularStrength = 0.5;
	//Specular
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = pointlight.ambient * material.ambient;
	vec3 diffuse = pointlight.diffuse * diff * material.diffuse;
	vec3 specular = pointlight.specular * (spec * material.specular);  


ambient  *= attenuation; 
diffuse  *= attenuation;
specular *= attenuation;



	return ambient + diffuse + specular;
}





void main()
 {

	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = DirectionalLight(dirLight, norm, viewDir);
	for(int i = 0; i < gNumOfLights;i++)
		result += PointLightCalc(pointLights[i], norm, FragPos, viewDir);


		

    FragColor = vec4(texture(texOne, Texcoord).rgb * result, 1.);
 }
