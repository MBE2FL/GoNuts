#include "Font.h"
#include "ShaderProgram.h"
//#include "ResourceManager.h"
#include "IO.h"
//#include "Camera.h"

bool FontManager::loaded = false;
ShaderProgram* FontManager::defaultFontShader = new ShaderProgram;
FT_Library FontManager::ft;
std::unordered_map<std::string, FontFace*> FontManager::Fonts;
CameraComponent* FontManager::_screenSpaceCamera = nullptr;
TransformComponent* FontManager::_cameraTransform = nullptr;

bool FontManager::init(TransformComponent* cameraTransform, CameraComponent* camera)
{
	defaultFontShader->load(SHADER_FONT_DIRECTORY + "font.vert", SHADER_FONT_DIRECTORY + "font.frag");

	if (FT_Init_FreeType(&ft))
	{
		cout << "FREETYPE: Could not init FreeType Library" << endl;
		return false;
	}
	_screenSpaceCamera = camera;
	_cameraTransform = cameraTransform;

	loaded = true;
	return true;
}

FontFace* FontManager::initNewFont(const std::string & filepath, int height)
{
	return initNewFont(filepath, 0, height);
}

FontFace* FontManager::initNewFont(const std::string & filepath, int width, int height)
{
	FontFace* newFace;
	std::string hash = filepath + "_" + std::to_string(width) + "_" + std::to_string(height);
	if (Fonts.find(hash) == Fonts.end())
	{
		cout<<"NEW FONT"<<endl;
		newFace = new FontFace();
		newFace->filename = filepath;

		FT_Error error = FT_New_Face(ft, (ASSETS_FONT_DIRECTORY + filepath).c_str(), 0, &newFace->face);
		if (error)
		{
			//SAT_DEBUG_LOG_ERROR("%d", error);
			//SAT_FUNCTION_NAME();
			cout<<":FREETYPE: Failed to load font"<<endl;
			//SAT_DEBUG_LOG_WARNING("%s", ("../assets/" + ASSETS_FONT_DIRECTORY + filepath).c_str());
			return false;
		}

		FT_Set_Pixel_Sizes(newFace->face, width, height);

		newFace->init();
		if (newFace->isLoaded())
		{
			Fonts[hash] = newFace;
		}
		else
		{
			cout<<"ERROR INITIALIZING FONT"<<endl;
		}
	}
	else
	{
		newFace = Fonts.at(hash);
	}

	return newFace;
}

FontFace::FontFace()
{
}

bool FontFace::init()
{
	//SAT_DEBUG_LOG_SAFE("%s", filename.c_str());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout<<"\n:FREETYPE: Failed to load Glyph"<<endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			vec2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
			vec2((float)face->glyph->bitmap_left,  (float)face->glyph->bitmap_top),
			vec2((float)face->glyph->advance.x, (float)face->glyph->advance.y)
		};

		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	loaded = true;
	return true;
}

bool FontFace::isLoaded()
{
	return loaded;
}

void TextRenderer::init()
{
	// Setup VBO

	GLfloat xpos = 0.0f;
	GLfloat ypos = 0.0f;
	GLfloat w = 0.5f;
	GLfloat h = 0.5f;

	GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
	};

	// Set up position (vertex) attribute
	{
		VertexBufferData positionAttrib;
		positionAttrib.attributeType = AttributeLocations::VERTEX;
		//positionAttrib.attributeName = "vertex";
		positionAttrib.data = &vertices[0];
		positionAttrib.sizeOfElement = sizeof(float);
		positionAttrib.elementType = GL_FLOAT;
		positionAttrib.numElementsPerAttribute = 4;
		positionAttrib.numElements = 6 * 4;
		vao.addVBO(positionAttrib);
	}


	vao.createVAO(GL_DYNAMIC_DRAW);

	shader = FontManager::defaultFontShader;
	loaded = true;
}

void TextRenderer::update(float deltaTime)
{
	time += deltaTime;
	
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	size_t i = 0;
	lineData.clear();
	LineData line;
	line.origin = vec3(vec3::Zero);
	// Don't include newlines as actual characters
	std::string::const_iterator s;
	for (s = text.begin(); s != text.end(); ++s)
	{
		Character ch = fontface->characters[*s];
		if (*s == '\n')
		{
			//x += (ch.offset.x + ch.size.x) * scale;
			line.width = x;
			lineData.push_back(line);
			y -= (ch.advance.y / 64.0f) * scale * lineSpacing;
			x = 0.0f;
			line.origin = vec3(x,y,z);
			continue;
		}
				
		x += (ch.advance.x / 64.0f) * scale;
		++i;
	}
	line.width = x;
	lineData.push_back(line);	
	data.resize(i);

	switch (alignment)
	{
	case Left:
		x = 0.0f;
		break;
	case Center:
		x = -lineData[0].width * 0.5f;
		break;
	case Right:
		x = -lineData[0].width;
		break;
	}
	y = 0.0f;
	i = 0;
	size_t lineNumber = 0;

	for (s = text.begin(); s != text.end(); ++s)
	{
		Character ch = fontface->characters[*s];
		if (*s == '\n')
		{
			lineNumber++;
			y -= fontface->face->size->metrics.height / 64.0f;
			switch (alignment)
			{
			case Left:
				x = 0.0f;
				break;
			case Center:
				x = -lineData[lineNumber].width * 0.5f;
				break;
			case Right:
				x = -lineData[lineNumber].width;
				break;
			}
			continue;
		}

		LetterData newLetter;
		newLetter.pos.x = x + ch.offset.x * scale;
		newLetter.pos.y = y - (ch.size.y - ch.offset.y) * scale;
		newLetter.scale = ch.size * scale;

		//calculate shake offset
		newLetter.shakeOffset.x = shake.amount.x * sin(time * shake.speed.x + (x * shake.frequency.x + y * shake.frequency.y));
		newLetter.shakeOffset.y = shake.amount.y * cos(time * shake.speed.y + (x * shake.frequency.x + y * shake.frequency.y));
		newLetter.pos += newLetter.shakeOffset;
		newLetter.pos += origin;

		newLetter.textureID = ch.textureID;

		x += (ch.advance.x / 64.0f) * scale;
		data[i] = newLetter;
		++i;
	}
	
}

void TextRenderer::draw()
{
	if (!loaded)
	{
		cout<<"Text Renderer not initialized!"<<endl;
		return;
	}

	shader->bind();

	if (localSpace == ScreenSpace)
	{
		shader->sendUniformMat4("uProj",FontManager::_screenSpaceCamera->getProjection().data, false);
		shader->sendUniformMat4("uView",FontManager::_cameraTransform->getView().data, false);
	}

	glActiveTexture(GL_TEXTURE0);
	vao.bind();
	
	for (LetterData i : data)
	{
		shader->sendUniform("uTextColor", color * i.color);

		// Update VBO for each character
		GLfloat vertices[6][4] =
		{
			{ i.pos.x,		 i.pos.y + i.scale.y, 0.0, 0.0 },
			{ i.pos.x,		 i.pos.y,		0.0, 1.0 },
			{ i.pos.x + i.scale.x, i.pos.y,		1.0, 1.0 },

			{ i.pos.x,		 i.pos.y + i.scale.y, 0.0, 0.0 },
			{ i.pos.x + i.scale.x, i.pos.y,		1.0, 1.0 },
			{ i.pos.x + i.scale.x, i.pos.y + i.scale.y, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, i.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vao.getVboHandle(AttributeLocations::VERTEX));
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glUseProgram(GL_NONE);
	//shader->unbind();
	vao.unbind();
}
