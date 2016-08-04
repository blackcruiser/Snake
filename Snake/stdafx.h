// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>


// Definition
#ifdef _DEBUG
	#define GL_PRINT_ERROR printf("%s %d: %d\n", __FILE__, __LINE__, glGetError())
#elif
	#define GL_PRINT_ERROR 
#endif