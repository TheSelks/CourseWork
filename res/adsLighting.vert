#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
vec3 fragmentPosition;
vec3 Position;
vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    vs_out.fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));
    vs_out.Position = vec3(model * vec4(vertexPosition, 1.0));
}