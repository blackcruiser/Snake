#include "stdafx.h"
#include "Scoreboard.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Scoreboard::Scoreboard(int cols, int rows) :
	m_cols(cols), m_rows(rows), m_x(0.0f), m_y(0.0f), m_scale(1.0f),
	m_glmTextColor(0.5f, 0.8f, 0.2f)
{
	mp_shader = new Shader("./Shader/Text.vs", "./Shader/Text.frag");
}

Scoreboard::~Scoreboard()
{
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_glProjLoc = glGetUniformLocation(mp_shader->program, "projection");
	m_glColorLoc = glGetUniformLocation(mp_shader->program, "textColor");

	m_glmProjMat = glm::ortho(0.0f, static_cast<GLfloat>(800),
		0.0f, static_cast<GLfloat>(600));

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_glVtxArr);
	glBindVertexArray(m_glVtxArr);

	glGenBuffers(1, &m_glVtxBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVtxBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	int err = glGetError();
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
	std::string text("This is sample text");
	GLfloat  baseX, baseY;

	mp_shader->Use();

	// Set OpenGL options
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniformMatrix4fv(m_glProjLoc, 1, GL_FALSE, glm::value_ptr(m_glmProjMat));
	glUniform3f(m_glColorLoc, m_glmTextColor.x, m_glmTextColor.y, m_glmTextColor.z);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_glVtxArr);

	baseX = m_x;
	baseY = m_y;
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
		glBindBuffer(GL_ARRAY_BUFFER, m_glVtxArr);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glDrawArrays(GL_TRIANGLES, 0, 6);
		baseX += (ch.advance >> 6) * m_scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindVertexArray(0);

	//glDisable(GL_BLEND);
}