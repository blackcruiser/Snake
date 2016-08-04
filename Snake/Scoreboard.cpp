#include "stdafx.h"
#include "Scoreboard.h"

#include <iostream>


Scoreboard::Scoreboard(Rectf &renderRegion, int cols, int rows, Shader *pSceneShader, Shader *pTextShader) :
	m_cols(cols), m_rows(rows), m_scale(1.0f), m_score(0),
	m_glmTextColor(0.5f, 0.8f, 0.2f), m_renderRegion(renderRegion),
	m_pSceneShader(pSceneShader), m_pTextShader(pTextShader),
	m_formater("Score:%2d")
{}

Scoreboard::~Scoreboard()
{}

void Scoreboard::Reset()
{
	m_score = 0;
}

void Scoreboard::AddPoint()
{
	m_score++;
}

void Scoreboard::LoadCharacters()
{
	FT_Library ft;
	FT_Face face;

	this->characters.clear();

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	if (FT_New_Face(ft, "./fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		GLuint texture;
		Character character;

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x),
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Scoreboard::WillRender()
{
	LoadCharacters();

	m_glColorLoc = glGetUniformLocation(m_pTextShader->program, "textColor");
	GL_PRINT_ERROR;

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_glVtxArr);
	glBindVertexArray(m_glVtxArr);
	GL_PRINT_ERROR;

	glGenBuffers(1, &m_glVtxBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVtxBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	GL_PRINT_ERROR;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	GL_PRINT_ERROR;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GL_PRINT_ERROR;
}

void Scoreboard::DidRender()
{
	for (auto iter_character = characters.begin(); iter_character != characters.end(); iter_character++)
	{
		glDeleteTextures(1, &iter_character->second.textureID);
	}
}

void Scoreboard::Render()
{
	std::string text(boost::str(m_formater % m_score));
	GLfloat  baseX, baseY;

	m_pTextShader->Use();

	// Set OpenGL options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3f(m_glColorLoc, m_glmTextColor.x, m_glmTextColor.y, m_glmTextColor.z);
	GL_PRINT_ERROR; 

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_glVtxArr);
	GL_PRINT_ERROR;

	baseX = m_renderRegion.x;
	baseY = m_renderRegion.y;
	for (auto c = text.begin(); c != text.end(); c++)
	{
		GLfloat x, y, w, h;
		Character ch;

		ch = characters[*c];

		x = baseX + ch.bearing.x * m_scale;
		y = baseY - (ch.size.y - ch.bearing.y) * m_scale;
		w = ch.size.x * m_scale;
		h = ch.size.y * m_scale;

		GLfloat vertices[6][4] = {
			{ x,     y + h,   0.0, 0.0 },
			{ x,	 y,       0.0, 1.0 },
			{ x + w, y,       1.0, 1.0 },

			{ x,     y + h,   0.0, 0.0 },
			{ x + w, y,		  1.0, 1.0 },
			{ x + w, y + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_glVtxBuf);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
		GL_PRINT_ERROR;

		glDrawArrays(GL_TRIANGLES, 0, 6);
		baseX += (ch.advance >> 6) * m_scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
		GL_PRINT_ERROR;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_PRINT_ERROR;
	}
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}