#version 420

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 texCoord0;
out vec3 FragPos;
out vec4 VertexColour;

void main()
{
	texCoord0 = aTexCoord;
	FragPos = (model * vec4(aPos, 1.0f)).xyz;
	Normal = mat3(transpose(inverse(model))) * aNormal;	
	gl_Position = projection * view * vec4(FragPos, 1.0f);
}