#include "Framebuffer.h"
#include "IO.h"

bool Framebuffer::_IsInitFrameBuffers = false;
VertexArrayObject Framebuffer::FullScreenQuad;
int Framebuffer::_MaxColorAttachments = 0;

DepthTarget::~DepthTarget()
{
	unload();
}

void DepthTarget::unload()
{
	glDeleteTextures(1, &_Tex._Handle);
}

ColorTarget::~ColorTarget()
{
	unload();
}

void ColorTarget::unload()
{
	glDeleteTextures(_NumAttachments, &_Tex[0]._Handle);
}

Framebuffer::~Framebuffer()
{
	unload();
}

void Framebuffer::unload()
{
	glDeleteFramebuffers(1, &_FBO);
	_IsInit = false;
}

void Framebuffer::init(unsigned width, unsigned height)
{
	setSize(width, height);
	init();	
}

void Framebuffer::init()
{
	//SAT_ASSERT(_Width > 0 && _Height > 0, "Width and/or Height not initialized!");
	//SAT_ASSERT(_IsInit == false, "Framebuffer already initialized!");

	glGenFramebuffers(1, &_FBO);
	if (!_ResolutionUBO.isInit()) 
	{
		_ResolutionUBO.allocateMemory(sizeof(vec4)); 
	}
	_ResolutionUBO.sendVector(vec4((float)_Width, (float)_Height, 1.0f / _Width, 1.0f / _Height), 0);

	//SAT_DEBUG_LOG("%d x %d", _Width, _Height);
	//SAT_DEBUG_LOG("This Framebuffer contains...");
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_DepthActive)
	{
		//SAT_DEBUG_LOG("1\tDepth Attachment");
		clearFlag |= GL_DEPTH_BUFFER_BIT;
		// Create depth texture
		_Depth._Tex._Target = GL_TEXTURE_2D;

		glGenTextures(1, &_Depth._Tex._Handle);
		glBindTexture(_Depth._Tex._Target, _Depth._Tex._Handle);
		glTexStorage2D(_Depth._Tex._Target, 1, GL_DEPTH_COMPONENT24, _Width, _Height);

		// Set texture parameters for depth
		glTextureParameteri(_Depth._Tex._Handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_Depth._Tex._Handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(_Depth._Tex._Handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_Depth._Tex._Handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _Depth._Tex._Handle, 0);
		//setOpenGLName(GL_TEXTURE, _Depth._Tex._Handle, "Framebuffer Depth");
	}

	if (_Color._NumAttachments)
	{
		//SAT_DEBUG_LOG("%d\tColor Attachments", _Color._NumAttachments);
		clearFlag |= GL_COLOR_BUFFER_BIT;
		GLuint* textureHandles = new GLuint[_Color._NumAttachments];

		// Generate 
		glGenTextures(_Color._NumAttachments, textureHandles);
		for (unsigned int i = 0; i < _Color._NumAttachments; ++i)
		{
			_Color._Tex[i]._Handle = textureHandles[i];
			_Color._Tex[i]._Target = GL_TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, _Color._Tex[i]._Handle);
			glTexStorage2D(GL_TEXTURE_2D, 1, _Color._Format[i], _Width, _Height);

			// Set texture parameters
			glTextureParameteri(_Color._Tex[i]._Handle, GL_TEXTURE_MIN_FILTER, _Filter);
			glTextureParameteri(_Color._Tex[i]._Handle, GL_TEXTURE_MAG_FILTER, _Filter);
			glTextureParameteri(_Color._Tex[i]._Handle, GL_TEXTURE_WRAP_S, _Wrap);
			glTextureParameteri(_Color._Tex[i]._Handle, GL_TEXTURE_WRAP_T, _Wrap);

			// Bind texture to the FBO
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _Color._Tex[i]._Handle, 0);
			//setOpenGLName(GL_TEXTURE, _Color._Tex[i]._Handle, "Framebuffer Color #" + std::to_string(i));
		}
		delete textureHandles;
	}
	unbind();
	//SAT_DEBUG_LOG_SAFE("Framebuffer Initialized!");
	_IsInit = true;
}

void Framebuffer::setSize(unsigned width, unsigned height)
{
	_Width = width;
	_Height = height;
}

void Framebuffer::addDepthTarget()
{
	//SAT_ASSERT(_DepthActive == false, "Depth target already initialized!");
	if (_Depth._Tex._Handle)
	{
		_Depth.unload();
	}
	_DepthActive = true;
}

void Framebuffer::addColorTarget(GLenum format)
{
	//SAT_ASSERT(!_IsInit, "Framebuffer alreading initialized!");
	_Color._Tex.resize(_Color._NumAttachments+1);
	_Color._Format.push_back(format);
	_Color._Buffer.push_back(GL_COLOR_ATTACHMENT0 + _Color._NumAttachments);
	_Color._NumAttachments++;	
}

void Framebuffer::reshape(unsigned width, unsigned height)
{
	//SAT_ASSERT(_IsInit, "Framebuffer not initialized!");
	setSize(width, height);
	unload();
	_Depth.unload();
	_Color.unload();
	init();
}

void Framebuffer::setViewport() const
{
	glViewport(0, 0, _Width, _Height);
}

void Framebuffer::bind() const
{	
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	if (_Color._NumAttachments)
		glDrawBuffers(_Color._NumAttachments, &_Color._Buffer[0]);
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::renderToFSQ() const
{
	//SAT_ASSERT(_IsInitFrameBuffers, "Full Screen Quad not initialized!");
	setViewport();
	bind();
	FullScreenQuad.draw();
	unbind();
}

void Framebuffer::bindResolution()
{
	_ResolutionUBO.bind(4U);
}

void Framebuffer::bindColorAsTexture(unsigned colorBuffer, int textureSlot) const
{
	//SAT_ASSERT(_IsInit, "Framebuffer not initialized!");
	//SAT_ASSERT(colorBuffer < _Color._Tex.size(), "Framebuffer not initialized!");
	_Color._Tex[colorBuffer].bind(textureSlot);
}

void Framebuffer::bindDepthAsTexture(int textureSlot) const
{
	_Depth._Tex.bind(textureSlot);
}

void Framebuffer::unbindTexture(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Framebuffer::backbuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, _Width, _Height, 0, 0, _Width, _Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);	
}

bool Framebuffer::checkFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//SAT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer not complete!");
		return false;
	}
	else
	{
		//PrintInfo();
		return true;
	}
}

void Framebuffer::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(clearFlag);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::initFrameBuffers()
{
	float VBO_DATA[] = 
	{
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,

		0.f, 0.f,	//one half of screen (triangle coordinates)
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,	//other half of screen
		0.f, 1.f,
		1.f, 0.f
	};

	VertexBufferData vertexData, uvData;
	
	vertexData.attributeType = VERTEX;
	vertexData.elementType = GL_FLOAT;
	vertexData.numElementsPerAttribute = 3;
	vertexData.numElements = 6 * vertexData.numElementsPerAttribute;
	vertexData.sizeOfElement = sizeof(float);
	vertexData.data = &VBO_DATA[0];

	uvData.attributeType = TEXCOORD;
	uvData.elementType = GL_FLOAT;
	uvData.numElementsPerAttribute = 2;
	uvData.numElements = 6 * uvData.numElementsPerAttribute;
	uvData.sizeOfElement = sizeof(float);
	uvData.data = &VBO_DATA[vertexData.numElements];

	FullScreenQuad.addVBO(vertexData);
	FullScreenQuad.addVBO(uvData);
	FullScreenQuad.createVAO();

	_IsInitFrameBuffers = true;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_MaxColorAttachments);
}

void Framebuffer::drawFSQ()
{
	//SAT_ASSERT(_IsInitFrameBuffers, "Full Screen Quad not initialized!");
	if (!_IsInitFrameBuffers)
		cout << "Full Screen Quad not initialized!" << endl;
	FullScreenQuad.draw();
}

GBuffer::GBuffer()
{
	addDepthTarget();
	addColorTarget(GL_RGB8);
	addColorTarget(GL_RGB8);
	addColorTarget(GL_R11F_G11F_B10F);

	textures.push_back(&_Depth._Tex);
	textures.push_back(&_Color._Tex[0]);
	textures.push_back(&_Color._Tex[1]);
	textures.push_back(&_Color._Tex[2]);
}

void GBuffer::setDebugNames()
{
	//setOpenGLName(GL_TEXTURE, _Depth._Tex.getID(), "GBuffer Depth");
	//setOpenGLName(GL_TEXTURE, _Color._Tex[0].getID(), "GBuffer Albedo");
	//setOpenGLName(GL_TEXTURE, _Color._Tex[1].getID(), "GBuffer Normal");
	//setOpenGLName(GL_TEXTURE, _Color._Tex[2].getID(), "GBuffer Emissive");
}
