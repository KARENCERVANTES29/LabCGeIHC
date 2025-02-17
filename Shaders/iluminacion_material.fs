#version 330 core

in vec3 our_normal;
in vec3 fragPos;
out vec4 color;

struct PositionalLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos; 
uniform PositionalLight light;
uniform vec4 ourColor;

void main(){

	// Iluminacion ambiental
	vec3 ambient = light.ambient * vec3(ourColor);
	// Iluminacion difusa
	vec3 lightDir = normalize(fragPos - light.position);
	float diff = max(dot(our_normal, -lightDir), 0.0);
	vec3 diffuse  = diff * light.diffuse * vec3(ourColor);
	// Iluminacion specular
	vec3 r = reflect(lightDir, our_normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow( max(dot(r , viewDir), 0.0), 32.0);
	vec3 specular  = spec * light.specular * vec3(ourColor);

	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0);

}
