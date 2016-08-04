#pragma once

#include "stdafx.h"

struct Rectf
{
	float x, y, width, height;
};


class Shader
{
private:

public:
	GLuint program;
	
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
};
