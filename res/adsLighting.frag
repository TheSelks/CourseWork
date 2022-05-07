#version 330 core

in VS_OUT {
vec3 fragmentPosition;
vec3 Position;
vec2 TexCoords;
} fs_in;


uniform vec3 lightColour;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D inTexture;

out vec4 fragmentColour;

void main()
{ 
     vec3 color = texture(inTexture, fs_in.TexCoords).rgb;
     float ambientStrength = 0.05;
     vec3 ambient = ambientStrength * lightColour;

     vec3 norm = normalize(fs_in.Position);
     vec3 lightDirection = normalize(lightPosition - fs_in.fragmentPosition);
     float diff = max(dot(norm, lightDirection), 0.0);
     vec3 diffuse = diff * lightColour;

     float specularStrength = 0.5;
     vec3 viewDirection = normalize(viewPosition - fs_in.fragmentPosition);
     vec3 reflectDirection = reflect(-lightDirection, norm);
     float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
     vec3 specular = specularStrength * spec * lightColour;

     vec3 result = (ambient + diffuse + specular) * color;
     fragmentColour = vec4(result, 1.0);
}