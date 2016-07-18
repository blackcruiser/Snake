#pragma once
#include "stdafx.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
	GLuint Program;
	
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
};
