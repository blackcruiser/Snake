#pragma once
#include "stdafx.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:

public:
	GLuint program;
	
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
};
