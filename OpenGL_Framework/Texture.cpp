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
Texture::Texture(const std::string & file, const bool LUT)
{
	if (!LUT)
		this->load(file);
	else
		load3D(file);
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

	glGenTextures(1, &this->_Handle);
	this->bind();
	glTextureStorage2D(this->_Handle, 1, this->_InternalFormat, this->sizeX, this->sizeY);

	glTextureSubImage2D(this->_Handle, 0, // We are editing the first and only layer in memory
		0, 0, // No offset
		this->sizeX, this->sizeY, // the dimensions of our image loaded
		GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
		textureData); // Pointer to the texture data

	glTextureParameteri(this->_Handle, GL_TEXTURE_MIN_FILTER, this->_FilterMin);
	glTextureParameteri(this->_Handle, GL_TEXTURE_MAG_FILTER, this->_FilterMag);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_S, this->_WrapU);
	glTextureParameteri(this->_Handle, GL_TEXTURE_WRAP_T, this->_WrapV);

	this->unBind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::load3D(const std::string & file)
{
	std::ifstream LUTfile(file.c_str());
	if (LUTfile.is_open()) {
		while (!LUTfile.eof()) {
			std::string LUTline;
			getline(LUTfile, LUTline);
			if (LUTline.empty()) continue;

			RGB line;
			if (sscanf_s(LUTline.c_str(), "%f %f %f",
				&line.r, &line.g, &line.b) == 3)
			{
				LUTVec.push_back(line);
			}
		}
	}
	else
		return false;

	if (LUTVec.size() != (pow(64.0, 3.0)))
		return false;
	_Target = GL_TEXTURE_3D;
	_InternalFormat = GL_RGB;
	glEnable(GL_TEXTURE_3D);

	glGenTextures(1, &_Handle);
	glBindTexture(GL_TEXTURE_3D, _Handle);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 64, 64, 64, 0, GL_RGB, GL_FLOAT, &LUTVec[0]);

	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	return true;
}

bool Texture::unload()
{
	if (this->_Handle != 0)
	{
		glDeleteTextures(1, &this->_Handle);
		return true;
	}
	return false;
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_Handle);
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

string Texture::getName() const
{
	return _name;
}

void Texture::setName(const string & name)
{
	_name = name;
}

int Texture::getSizeX() const
{
	return sizeX;
}

int Texture::getSizeY() const
{
	return sizeY;
}
