#pragma once
#include <vector>
#include <GL/glew.h>
#include "VertexBufferObject.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include <iostream>
using std::cout;
using std::endl;
/*
  /////////////////
 // Framebuffer //
/////////////////

FrameBuffers are a collection of buffers (usually textures) used as a method of 
storing the results of rendering. OpenGL has 2 different kinds of framebuffers,
one that we've been using this whole time! The Default Framebuffer, used to 
represent a window or display device, and user-created Framebuffer Objects.
"FBOs" reference images from either Textures or Renderbuffers, and are not 
directly visible. In order to view them, we must draw/blit the framebuffer
to the Default Framebuffer. In order to draw it, we create a quad that covers 
the entire screen, and sample from the FBO like a texture using a simple
passthrough shader. A Framebuffer can hold any number of color attachments 
and an optional depth attachment. No color and only depth is also allowed.
They can also contain a stencil buffer, or even a merged depth/stencil.

More info about Framebuffers can be read here
https://www.khronos.org/opengl/wiki/Framebuffer

Limitations of Framebuffers can be found using glGetIntegerv(GL_MACRO, &integer)
	-Framebuffers have a maximum number of draw buffers
		GL_MAX_DRAW_BUFFERS
*/

struct DepthTarget
{
	~DepthTarget();
	void unload();
	Texture _Tex;
};

struct ColorTarget
{
	~ColorTarget();
	void unload();
	std::vector<Texture> _Tex;
	std::vector<GLenum> _Format;
	std::vector<GLenum> _Buffer;
	unsigned int _NumAttachments = 0;
};

class Framebuffer
{
public:
	~Framebuffer();
	void unload();
	void init(unsigned width, unsigned height);
	void init();
	void setSize(unsigned width, unsigned height);
	void addDepthTarget();
	void addColorTarget(GLenum format);
	void reshape(unsigned width, unsigned height);

	void setViewport() const;
	void bind() const;
	void unbind() const;
	void renderToFSQ() const;
	void bindResolution();
	
	void bindColorAsTexture(unsigned colorBuffer, int textureSlot) const;
	void bindDepthAsTexture(int textureSlot) const;
	void unbindTexture(int textureSlot) const;

	void backbuffer();
	bool checkFBO();

	void clear();

	GLuint _FBO;
	DepthTarget _Depth;
	ColorTarget _Color;
	bool _IsInit = false;
	bool _DepthActive = false;

	GLenum _Filter = GL_NEAREST;
	GLenum _Wrap = GL_CLAMP_TO_EDGE;

	unsigned _Width = 0;
	unsigned _Height = 0;	
	
	GLbitfield clearFlag = 0;

	static void initFrameBuffers();
	static void drawFSQ();
protected:
	UniformBuffer _ResolutionUBO;
	static int _MaxColorAttachments;
	static bool _IsInitFrameBuffers;
	static VertexArrayObject FullScreenQuad;
};

class GBuffer : public Framebuffer
{
public:
	GBuffer();
	void setDebugNames();

	std::vector<Texture*> textures;
};