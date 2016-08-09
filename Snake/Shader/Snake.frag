#version 330 core

in vec2 textPt;

out vec4 color;

uniform sampler2D smpTexture;

void main()
{
	color = texture(smpTexture, textPt);
}