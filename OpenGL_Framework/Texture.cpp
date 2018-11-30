#include "Texture.h"

//Texture::~Texture()
//{
//	unload();
//}
//
//bool Texture::load(const string & file)
//{
//	texObj = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
//
//	if (texObj == GL_NONE)
//	{
//		cout << "Texture failed to load.\n" << SOIL_last_result() << endl;
//		return false;
//	}
//
//	//Modify the texture
//	glBindTexture(GL_TEXTURE_2D, texObj);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //u axis
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //v axis
//
//	return true;
//}
//
//void Texture::unload()
//{
//	if (texObj != GL_NONE)
//	{
//		// Remove data from GPU
//		glDeleteTextures(1, &texObj);
//		texObj = GL_NONE;
//	}
//}
//
//void Texture::bind()
//{
//	glBindTexture(GL_TEXTURE_2D, texObj);
//}
//
//void Texture::unBind()
//{
//	glBindTexture(GL_TEXTURE_2D, GL_NONE);
//}
//
//void Texture::bind(const int textureSlot)
//{
//	glActiveTexture(GL_TEXTURE0 + textureSlot);
//	bind();
//}
//
//void Texture::unBind(const int textureSlot)
//{
//	glActiveTexture(GL_TEXTURE0 + textureSlot);
//	unBind();
//}
//
Texture::Texture(const std::string & file)
{
	this->load(file);
}

Texture::~Texture()
{
	this->unload();
}

bool Texture::load(const std::string & file)
{
	this->filename = file;

	unsigned char* textureData;
	textureData = SOIL_load_image((this->filename).c_str(),
		&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		printf("TEXTURE BROKE: %s", this->filename.c_str());
		return false;
	}

	// If the texture is 2D, set it to be a 2D texture;
	_Target = GL_TEXTURE_2D;
	_InternalFormat = GL_RGBA8;

	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTextureStorage2D(this->_TexHandle, 1, this->_InternalFormat, this->sizeX, this->sizeY);

	glTextureSubImage2D(this->_TexHandle, 0, // We are editing the first and only layer in memory
		0, 0, // No offset
		this->sizeX, this->sizeY, // the dimensions of our image loaded
		GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
		textureData); // Pointer to the texture data

	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_FilterMin);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_FilterMag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_WrapU);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_WrapV);

	this->unBind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::unload()
{
	if (this->_TexHandle != 0)
	{
		glDeleteTextures(1, &this->_TexHandle);
		return true;
	}
	return false;
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TexHandle);
}

void Texture::bind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->bind();
}

void Texture::unBind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unBind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->unBind();
}
