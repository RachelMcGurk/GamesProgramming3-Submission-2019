#version 420

in vec4 colour;
in vec2 TexCoord;

out vec4 fragColour;

uniform sampler2D ourTexture;

void main()
{
	fragColour = texture(ourTexture,TexCoord) * colour;
}