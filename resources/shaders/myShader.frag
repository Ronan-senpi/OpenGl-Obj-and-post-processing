#version 430 core
in vec3 fragPos;
in vec3 normals;
in vec2 TexCoords;

layout(location = 2) uniform vec3 camPos;//camera Position
layout(location = 3) uniform vec3 objectColor;
layout(location = 4) uniform vec3 lightColor;
layout(location = 5) uniform vec3 lightPos;
layout(location = 8) uniform float ambientStrength;
layout(location = 9) uniform float specularStrength;
layout(location = 10) uniform int invert;

uniform sampler2D texture1;

out vec4 fragColor;
void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
    //ambient = vec3(0);
    // diffuse
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
//    if (invert == 1){
        vec3 diffuse = diff * lightColor * vec3(texture(texture1, TexCoords));
//    }
    //diffuse = vec3(0);
    // speculaaaaaaaaaaaaar
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    // specular = vec3(0);
    vec3 result = (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0);
}