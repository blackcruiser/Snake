#pragma once

#include "BaseModule.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>


struct Character {
	GLuint     textureID;  // 字形纹理ID
	glm::ivec2 size;       // 字形大大小
	glm::ivec2 bearing;    // 字形基于基线和起点的位置
	GLuint     advance;    // 起点到下一个字形起点的距离
};


class Scoreboard
{
private:
	Rectf m_renderRegion;
	int m_cols, m_rows, m_score;
	GLfloat m_scale;
	glm::vec3 m_glmTextColor;

	Shader *m_pTextShader, *m_pSceneShader;
	GLuint m_glVtxBuf, m_glVtxArr,
		m_glColorLoc;


	std::map<GLchar, Character> characters;

public:
	Scoreboard(Rectf &renderRegion, int cols, int rows, Shader *pSceneShader, Shader *pTextShader);
	~Scoreboard();

	void Reset();

	void LoadCharacters();

	virtual void WillRender();
	virtual void DidRender();
	virtual void Render();
};

