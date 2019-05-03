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
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.6), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}




void main()
 {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = DirectionalLight(dirLight, norm, viewDir);
	for(int i = 0; i < gNumOfLights;i++)
		result += PointLightCalc(pointLights[i], norm, FragPos, viewDir);

	vec2 u_resolution = vec2(1024, 769);

	vec2 st = gl_FragCoord.xy/u_resolution.xy*3.;
    // st += st * abs(sin(u_time*0.1)*3.0);
    vec3 color = vec3(0.0);

    vec2 q = vec2(0.);
    q.x = fbm( st + 0.00*u_Time);
    q.y = fbm( st + vec2(1.0));

    vec2 r = vec2(0.);
    r.x = fbm( st + 1.0*q + vec2(1.7,9.2)+ 0.15*u_Time);
    r.y = fbm( st + 1.0*q + vec2(8.3,2.8)+ 0.126*u_Time);

    float f = fbm(st+r);

    color = mix(vec3(0.101961,0.619608,0.666667),
                vec3(0.666667,0.666667,0.498039),
                clamp((f*f)*4.0,0.0,1.0));

    color = mix(color,
                vec3(0,0,0.164706),
                clamp(length(q),0.0,1.0));

    color = mix(color,
                vec3(0.666667,1,1),
                clamp(length(r.x),0.0,1.0));

    FragColor = vec4((f*f*f+.6*f*f+.5*f)*color,1.);
 }
