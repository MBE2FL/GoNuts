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
#include "ParticleEmitter.h"

#include "GUIHelper.h"


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

	vector<GameObject> add(vector<GameObject> objectVec1, vector<GameObject> objectVec2);
	vector<GameObject> objectSetup(const string shader, const string mesh, const string texture,const bool physics,
								   const Vector3 position, const Vector3 scale, const int amount, const int startNum, float offset);
	void imguiDraw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	float drawTime = 0.0f;

private:
	// Scene Objects.
	Camera camera;
	Camera UICamera;

	GameObject player;
	GameObject coneBoi;
	float t = 0.0f;

	Vector3 p1 = Vector3(2.0f, 10.0f, -5.0f);
	Vector3 p2 = Vector3(2.0f, 9.0f, -5.0f);
	Vector3 p3 = Vector3(2.0f, 5.0f, -5.0f);
	Vector3 p4 = Vector3(2.0f, 3.0f, -5.0f);

	vector<GameObject> coins;
	vector<GameObject> platforms;
	vector<GameObject> sceneObjects;
	vector<GameObject> Background;
	vector<GameObject> upperPlatforms;

	GameObject* particleTrail;
	GameObject* jumpParticles;

	bool collided = false;
	bool sliding = false;
	bool reverse = false;

	float counter = 0.0f;

	Light* light;
	Light* spotLight;

};
