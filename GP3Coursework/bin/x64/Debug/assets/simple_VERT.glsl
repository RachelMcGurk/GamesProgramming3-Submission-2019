#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;

out vec4 colour;
out vec2 TexCoord;

void main()
{
	gl_Position = (projection * view * model) * vec4(aPos, 1.0);
	colour = vec4(objectColor, 1.0);
	TexCoord = aTexCoord;
}