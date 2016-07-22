#pragma once

#include "BaseModule.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>


struct Character {
	GLuint     textureID;  // ��������ID
	glm::ivec2 size;       // ���δ��С
	glm::ivec2 bearing;    // ���λ��ڻ��ߺ�����λ��
	GLuint     advance;    // ��㵽��һ���������ľ���
};


class Scoreboard
{
private:
	int m_cols, m_rows, m_score;
	GLfloat m_x, m_y, m_scale;
	glm::vec3 m_glmTextColor;

	Shader *mp_shader;
	GLuint m_glVtxBuf, m_glVtxArr,
		m_glColorLoc, m_glProjLoc;
	glm::mat4 m_glmProjMat;



	std::map<GLchar, Character> characters;

public:
	Scoreboard(int cols, int rows);
	~Scoreboard();

	void Reset();

	void LoadCharacters();

	virtual void WillRender();
	virtual void DidRender();
	virtual void Render();
};

