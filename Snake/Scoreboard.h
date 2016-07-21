#pragma once

#include "BaseModule.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

struct Character {
	GLuint     TextureID;  // ��������ID
	glm::ivec2 Size;       // ���δ��С
	glm::ivec2 Bearing;    // ���λ��ڻ��ߺ�����λ��
	GLuint     Advance;    // ��㵽��һ���������ľ���
};

class Scoreboard
{
private:
	int m_score;

	Shader *mp_shader;
	GLuint m_glVtxBuf, m_glVtxArr;



	std::map<GLchar, Character> Characters;

public:
	Scoreboard();
	~Scoreboard();

	void Reset();

	virtual void WillRender();
	virtual void DidRender();
	virtual void Render();
};

