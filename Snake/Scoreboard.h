#pragma once

#include "BaseModule.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <boost/format.hpp>

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
	Rectf m_renderRegion;
	int m_score;
	GLfloat m_scale;
	glm::vec3 m_glmTextColor;
	glm::mat4 m_glmTextTransMat;

	Shader *m_pTextShader, *m_pSceneShader;
	GLuint m_glVtxBuf, m_glVtxArr,
		m_glColorLoc, m_glTextTransMatLoc;

	boost::format m_formater;
	std::map<GLchar, Character> characters;


public:
	Scoreboard(Rectf &renderRegion, Shader *pSceneShader, Shader *pTextShader);
	~Scoreboard();

	void Reset();
	void AddPoint();

	void LoadCharacters();

	virtual void WillRender();
	virtual void DidRender();
	virtual void Render();
};

