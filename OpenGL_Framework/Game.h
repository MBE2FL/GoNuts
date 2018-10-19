#pragma once

//#include "ShaderProgram.h"
//#include "Mesh.h"
#include "GameObject.h"
#include "ObjectLoader.h"

#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Timer.h"
#include "Transform.h"
#include "Camera.h"


#define WINDOW_WIDTH			1900
#define WINDOW_HEIGHT			1000
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

private:
	// Scene Objects.
	Camera camera;
	//GameObject monkey;
	//GameObject monkeyTwo;
	//GameObject cube;

	GameObject player;
	GameObject platOne;
	bool collided = false;

	//vector<GameObject> platform;


	// OpenGL Handles
	//unsigned int VBO = GL_NONE;
	//unsigned int VAO = GL_NONE;
	//unsigned int shaderProgram = GL_NONE;
	//unsigned int vertShader = GL_NONE;
	//unsigned int fragShader = GL_NONE;
};
