#version 330 core
layout (location = 0) in vec2 iVtxPt;
layout (location = 1) in vec2 iTextPt;

out vec2 textPt;

uniform mat4 projMat;

void main()
{
	gl_Position = projMat * vec4(iVtxPt, 0.0, 1.0);
	textPt = iTextPt;
}