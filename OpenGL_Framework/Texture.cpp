#include "Texture.h"

Texture::~Texture()
{
	unload();
}

bool Texture::load(const string & file)
{
	texObj = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (texObj == GL_NONE)
	{
		cout << "Texture failed to load.\n" << SOIL_last_result() << endl;
		return false;
	}

	//Modify the texture
	glBindTexture(GL_TEXTURE_2D, texObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //u axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //v axis

	return true;
}

void Texture::unload()
{
	if (texObj != GL_NONE)
	{
		// Remove data from GPU
		glDeleteTextures(1, &texObj);
		texObj = GL_NONE;
	}
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texObj);
}

void Texture::unBind()
{
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture::bind(const int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	bind();
}

void Texture::unBind(const int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	unBind();
}
