#pragma once
#include "FreeType/ft2build.h"
#include "MathLib/MathLibCore.h"
#include "GL/glew.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include FT_FREETYPE_H


using std::unordered_map;
using std::string;
using std::cout;
using std::endl;

struct Character {
	GLuint TextureID; //id handle of glyph
	vec2 Size; // size of the glyph
	vec2 bearing; //Offset from baseline to left/top of glyph
	GLuint advance; //Horizontal offset to next glyph
};

class FreeType
{
public:
	FreeType(string ttfName);
	void Init(int width, int height);
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec4 color);

private:
	ShaderProgram shader;
	unordered_map<GLchar, Character> Characters;
	GLuint VAO, VBO, shaderID;
	FT_Library ft;
	FT_Face face;
};
