#pragma once

#include "BaseModule.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

struct Character {
	GLuint     TextureID;  // 字形纹理ID
	glm::ivec2 Size;       // 字形大大小
	glm::ivec2 Bearing;    // 字形基于基线和起点的位置
	GLuint     Advance;    // 起点到下一个字形起点的距离
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

