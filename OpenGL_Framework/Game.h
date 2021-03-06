#pragma once

//#include "ShaderProgram.h"
//#include "Mesh.h"
#include "GameObject.h"
#include "ObjectLoader.h"

//#include <windows.h>
//#include "GL/glew.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

#include "Timer.h"
#include "Transform.h"
//#include "Camera.h"
//#include "ParticleEmitter.h"
//#include "UniformBuffer.h"
#include "FrameBuffer.h"

//#include "GUIHelper.h"


#include "SceneManager.h"

#include "SoundComponent.h"

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/*void imguiDraw();*/

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void specialKeyDown(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);
	void mouseWheel(int wheel, int direction, int x, int y);
	void reshapeWindow(int w, int h);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	float drawTime = 0.0f;

	vec3 offse = vec3(0,18,-25.882f);

	int windowWidth = WINDOW_WIDTH;
	int windowHeight = WINDOW_HEIGHT;

private:

	float t = 0.0f;

	SoundComponent* _sound;
	//UI
	//GameObject nutOmeter;
	//GameObject time;

	//GameObject* particleTrail;
	//GameObject* jumpParticles;

	bool outline = true;

	float counter = 0.0f;


	Scene* _currentScene;

	Entity* _testEntity;
	Entity* _testEntity2;


	Framebuffer frameBufferOutline;
	Framebuffer frameBufferLUT;
	GBuffer		gbuffer;

	Framebuffer frameBufferShadow;

	Framebuffer frameBufferUI;

	ShaderProgram shaderGbuffer;
	ShaderProgram shaderOutline;
	ShaderProgram shaderLUT;
	ShaderProgram shaderDeferred;

	Texture* LUTTex;
	Texture* LUTTexVal;
	Texture* toonRamp;

	vector<Texture*> introVec;

	SceneManager* sceneManager;


	vector<Light*> lights;
	Mesh* UIMesh;

	vec4* color = new vec4[2000];
	vec4* position = new vec4[2000];
	vec4* dir = new vec4[2000];
	vec4* attenuation = new vec4[2000];


	bool lut = true;
	bool deferred = false;
	bool gameStart = false;
	bool soundbool = false;

	float frameTime = 0.0f;
	int frameNum = 24;
};
