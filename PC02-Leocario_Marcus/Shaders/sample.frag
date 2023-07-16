#version 330 core

uniform sampler2D tex0;

// uniform vec4 rgba;
uniform vec3 cameraPos;

out vec4 FragColor;

in vec3 normCoord;
in vec3 fragPos;
in vec2 texCoord;

// struct DirLight {
//     vec3 direction;
  
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;

// 	float ambientStr;
// 	float specStr;
// 	float specPhong;

// 	vec3 lightColor;
// 	vec3 ambientColor;
// };  

// struct PointLight {    
//     vec3 position;
    
//     float constant;
//     float linear;
//     float quadratic;  

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;

// 	float ambientStr;
//     float specStr;
//     float specPhong;

//     vec3 lightColor;
//     vec3 ambientColor;
// };

// vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
// vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

// uniform DirLight dirLight;
// uniform PointLight pointLight;

void main(){
	//				     r     g      b      a
	// FragColor = vec4(1.0f, 0.72f, 0.77f, 1.0f);
	// FragColor = rgba;

	// light source info
	vec3 normal = normalize(normCoord);
	vec3 viewDir = normalize(cameraPos - fragPos);

	// FragColor = CalcDirLight(dirLight, normal, viewDir) * texture(tex0, texCoord);
	// FragColor = CalcPointLight(pointLight, normal, viewDir) * texture(tex0, texCoord);
	// FragColor = CalcDirLight(dirLight, normal, viewDir) * CalcPointLight(pointLight, normal, viewDir) * texture(tex0, texCoord);
	// FragColor = vec4(attenuation * (specColor + diffuse + ambientCol), 1.0) * texture(tex0, texCoord);
	FragColor = texture(tex0, texCoord);
}

// vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     // light source info
//     vec3 lightDir = normalize(-light.direction);

// 	// ambient
// 	vec3 ambient = light.ambientColor * light.ambientStr;

// 	// diffuse
// 	float diff = max(dot(normal, lightDir), 0.0);
// 	vec3 diffuse = diff * light.lightColor;
	
// 	// specular
// 	vec3 reflectDir = reflect(-lightDir, normal);
// 	float spec = pow(max(dot(reflectDir, viewDir), 0.1), light.specPhong);
// 	vec3 specColor = spec * light.specStr * light.lightColor;

//     return vec4(light.specColor + diffuse + ambient, 1.0);
// } 

// vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     // precalculated intensity scaling taken from online sources (OpenGL tutorials...)
// 	// float constant = 1.0;
// 	// float linear = 0.14;
// 	// float quadratic = 0.07;

//     vec3 lightDir = normalize(light.position - fragPos);
// 	float dist = length(light.position - fragPos);

// 	// More realistic computation for attenuation
// 	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

// 	// Simple attenuation calculation
// 	// float attenuation = 1.0 / (dist * dist);

// 	// ambient
// 	vec3 ambient = light.ambientColor * light.ambientStr;

// 	// diffuse
// 	float diff = max(dot(normal, lightDir), 0.0);
// 	vec3 diffuse = diff * light.lightColor;
	
// 	// specular
// 	vec3 reflectDir = reflect(-lightDir, normal);
// 	float spec = pow(max(dot(reflectDir, viewDir), 0.1), light.specPhong);
// 	vec3 specular = spec * light.specStr * light.lightColor;

// 	return vec4(attenuation * (specular + diffuse + ambient), 1.0);
// }