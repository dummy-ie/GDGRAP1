#version 330 core

uniform sampler2D tex0;
uniform vec4 rgba;
uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform float specStr;
uniform float specPhong;

out vec4 FragColor;

in vec3 normCoord;
in vec3 fragPos;
in vec2 texCoord;

void main(){
	//				     r     g      b      a
	// FragColor = vec4(1.0f, 0.72f, 0.77f, 1.0f);
	// FragColor = rgba;

	// light source info
	vec3 normal = normalize(normCoord);
    vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(cameraPos - fragPos);

	// precalculated intensity scaling taken from online sources (OpenGL tutorials...)
	float constant = 1.0;
	float linear = 0.14;
	float quadratic = 0.07;

	float dist = length(lightPos - fragPos);

	// More realistic computation for attenuation
	float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

	// Simple attenuation calculation
	// float attenuation = 1.0 / (dist * dist);

	// ambient
	vec3 ambientCol = ambientColor * ambientStr;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	// diffuse *= attenuation;
	// ambientCol *= attenuation;
	// specColor *= attenuation;

	FragColor = vec4(attenuation * (specColor + diffuse + ambientCol), 1.0) * texture(tex0, texCoord);
}