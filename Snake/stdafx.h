// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>


// Definition
#ifdef _DEBUG
	#define GL_PRINT_ERROR printf("%s %d: %d\n", __FILE__, __LINE__, glGetError())
#elif
	#define GL_PRINT_ERROR 
#endif