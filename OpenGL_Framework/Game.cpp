#include "Game.h"

#include <string>

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

void Game::initializeGame()
{
	// OpenGL will not draw triangles hidden behind other geometry
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag");

	ObjectLoader::loadMesh("FatBoi", "./Assets/Models/Fat_Boi_Ultimate_Rigged_Edition.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");
	ObjectLoader::loadMesh("Platform2", "./Assets/Models/monkey.obj");
	ObjectLoader::loadTexture("Default", "./Assets/Textures/Default.png");
	ObjectLoader::loadTexture("Background", "./Assets/Textures/background.png");

	//ObjectLoader::loadMesh("Platform", "./Assets/Models/roof tile.obj");
	//ObjectLoader::loadMesh("Border", "./Assets/Models/roof board.obj");
	//ObjectLoader::loadMesh("BorderEdge", "./Assets/Models/roof board edge.obj");
	ObjectLoader::loadMesh("Acorn", "./Assets/Models/acorn.obj");
	ObjectLoader::loadMesh("Coin", "./Assets/Models/coin.obj");
	ObjectLoader::loadMesh("Cone", "./Assets/Models/cone.obj");
	ObjectLoader::loadMesh("Spikes", "./Assets/Models/spikes.obj");
	ObjectLoader::loadMesh("Vent", "./Assets/Models/vent.obj");
	ObjectLoader::loadMesh("Background", "./Assets/Models/background.obj");
	ObjectLoader::loadMesh("Plane", "./Assets/Models/plane.obj");

	player.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	player.setMesh(ObjectLoader::getMesh("FatBoi"));
	player.setTexture(ObjectLoader::getTexture("Default"));
	player.addPhysicsBody(true);
	player.setPosition(Vector3(-3.0f, 8.0f, -5.0f));
	player.setScale(0.2f);
	
	footEmitter = new ParticleEmitter;
	footEmitter->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	footEmitter->setMesh(ObjectLoader::getMesh("Plane"));
	footEmitter->setTexture(ObjectLoader::getTexture("Default"));
	footEmitter->addPhysicsBody(false);
	footEmitter->setPosition(Vector3(9.0f, 2.0f, -5.0f));
	footEmitter->setScale(Vector3(1, 1, 1));
	
	// Physics properties
	dynamic_cast<ParticleEmitter*>(footEmitter)->velocity0 = Vector3(-0.1f, -0.01f, -0.0001f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->velocity1 = Vector3(-0.1f, -0.01f, 0.0001f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->massRange = Vector2(1.0f, 2.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->emitterPosition = player.getPosition();

	// Visual Properties
	dynamic_cast<ParticleEmitter*>(footEmitter)->colorBegin0 = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->colorBegin1 = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->colorEnd0 = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->colorEnd1 = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->lifeRange = Vector2(0.5f, 1.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->sizeRange = Vector2(15.0f, 25.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->sizeBegin = Vector2(5.0f, 10.0f);
	dynamic_cast<ParticleEmitter*>(footEmitter)->sizeEnd = Vector2(50.0f, 100.0f);

	dynamic_cast<ParticleEmitter*>(footEmitter)->interpolateColor = true;

	// Create the particles
	dynamic_cast<ParticleEmitter*>(footEmitter)->initialize(50);


	Background = objectSetup("Normal", "Background", "Background", false, Vector3(100.0f, -5.0f, -20.0f), Vector3(25, 25, 1), 20, 0, 0);
	//					shader,    mesh, texture,  physics, position,                    scale,         #Objects, startNum(i), offset
	coins = objectSetup("Normal", "Coin", "Default", false, Vector3(35.0f, 4.0f, -5.0f), Vector3(1,1,1), 1,       0,           0);

	coins = add(coins, objectSetup("Normal", "Acorn", "Default", false, Vector3(2.0f, 4.0f, -5.0f), Vector3(1, 1, 1), 3, 0, 0));

	coins = add(coins, objectSetup("Normal", "Acorn", "Default", false, Vector3(2.0f, 4.0f, -5.0f), Vector3(1, 1, 1), 3, 0, 37.0f));

	coins = add(coins, objectSetup("Normal", "Coin", "Default", false, Vector3(2.0f, 4.5f, -5.0f), Vector3(1, 1, 1), 3, 0, 6.0f));

	sceneObjects = objectSetup("Normal", "Cone", "Default", false, Vector3(2.0f, 4.0f, -5.0f), Vector3(1, 1, 1), 3, 0, 12.0f);

	sceneObjects = objectSetup("Normal", "Spikes", "Default", false, Vector3(2.0f, 4.0f, -5.0f), Vector3(1, 1, 1), 3, 0, 18.0f);

	sceneObjects = add(sceneObjects, objectSetup("Normal", "Vent", "Default", false, Vector3(3.0f, 4.0f, -5.0f), Vector3(1, 1, 1), 3, 0, 24.0f));

	platforms = objectSetup("Normal", "Platform", "Default", false, Vector3(9.0f, 2.0f, -5.0f), Vector3(1, 1, 2), 20, 0, 0);

	upperPlatforms = objectSetup("Normal", "Platform", "Default", false, Vector3(9.0f, 4.2f, -5.0f), Vector3(0.4f, 1, 1), 10, 5, 0);

	upperPlatforms = add(upperPlatforms, objectSetup("Normal", "Platform", "Default", false, Vector3(9.0f, 4.2f, -5.0f), Vector3(0.4f, 1, 1), 20, 15, 0));

	light = new Light();
	light->setPosition(Vector3(4.0f, 0.0f, 0.0f));
	light->setAmbient(Vector3(1.0f, 1.0f, 1.0f));
	light->setDiffuse(Vector3(0.7f, 0.1f, 0.2f));
	light->setSpecular(Vector3(1.0f, 0.1f, 0.1f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	//Matrix44 test;
	//test.mV[0] = 1; test.mV[1] = 3; test.mV[2] = 4; test.mV[3] = 4; test.mV[4] = 5; test.mV[5] = 8; test.mV[6] = 7;
	//test.mV[7] = 8; test.mV[8] = 9; test.mV[9] = 10; test.mV[10] = 11; test.mV[11] = 12; test.mV[12] = 13; test.mV[13] = 14;
	//test.mV[14] = 15; test.mV[15] = 16;
	//test.GetInverse();

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	//camera.orthographic(-10, 10, -10, 10, -100, 100);
	camera.setPosition(Vector3(0.0f, 0.0f, 5.0f));
	camera.setRotationAngleX(camera.getRotationAngleX() - 0.2f);
}

void Game::update()
{

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	drawTime += deltaTime;

	dynamic_cast<ParticleEmitter*>(footEmitter)->emitterPosition = player.getPosition();
	footEmitter->update(deltaTime);
	player.update(deltaTime);
	for (unsigned int i = 0; i < coins.size(); i++)
	{
		coins[i].update(deltaTime); 
		player.checkCoinCollisions(coins[i]);
	}
	for (unsigned int i = 0; i < Background.size(); i++)
	{
		Background[i].update(deltaTime);
	}
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].update(deltaTime);
		sceneObjects[i].setRotationAngleY(TotalGameTime * 1.25f + i);
	}
	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		platforms[i].update(deltaTime);
	}
	for (unsigned int i = 0; i < upperPlatforms.size(); i++)
	{
		upperPlatforms[i].update(deltaTime);
		player.checkCollisions(upperPlatforms[i]);
	}
	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		collided = player.checkCollisions(platforms[i]);
		if (collided)
			break;
	}

	
	if (player.getPhysicsBody()->getVelocity().x < 5.0f)
		player.addForce(Vector2(20.0f, 0.0f));

	if (player.getPosition().y < -4.0f || player.getPhysicsBody()->getVelocity().x < 0.0f)
	{
		player.setPosition(Vector3(-3.0f, 4, -5.0f));
		player.setVelocity(Vector2(0.0f, 0.0f));
		player.setScale(0.2f);
		sliding = false;
	}
	
	counter += deltaTime;
	player.setRotationAngleY(90.0f);

	Vector3 offset(-3, -1.5f, -8);
	camera.setPosition(MathLibCore::lerp( camera.getPosition(), player.getPosition() - offset, deltaTime * 3));
	camera.update(deltaTime);
}

void Game::draw()
{
	//std::cout << "HELLO WORLD" << std::endl;

	//if (drawTime >= 0.033f)
	{
		//std::cout << drawTime << std::endl;
		// Completely clear the Back-Buffer before doing any work.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _DEBUG

		// New imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplFreeGLUT_NewFrame();

		// Update imgui widgets
		imguiDraw();

		// Render imgui
		ImGui::Render();
#endif

		// Draw game objects
		footEmitter->draw(camera, light);

		//for (unsigned int i = 0; i < dynamic_cast<ParticleEmitter*>(footEmitter)->getNumParticles(); i++)
		//{
		//	dynamic_cast<ParticleEmitter*>(footEmitter)->m_pParticles[i]->draw(camera, light);
		//}
		player.draw(camera, light);

		for (unsigned int i = 0; i < coins.size(); i++)
		{
			coins[i].draw(camera, light);
		}
		for (unsigned int i = 0; i < Background.size(); i++)
		{
			Background[i].draw(camera, light);
		}
		for (unsigned int i = 0; i < sceneObjects.size(); i++)
		{
			sceneObjects[i].draw(camera, light);
		}
		for (unsigned int i = 0; i < platforms.size(); i++)
		{
			platforms[i].draw(camera, light);
		}
		for (unsigned int i = 0; i < upperPlatforms.size(); i++)
		{
			upperPlatforms[i].draw(camera, light);
		}

		// Update imgui draw data
		glUseProgram(GL_NONE);
#ifdef _DEBUG

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
		glutSwapBuffers();
		drawTime = 0.0f;
	}
}

vector<GameObject> Game::add(vector<GameObject> objectVec1, vector<GameObject> objectVec2)
{
	for (unsigned int i = 0; i < objectVec2.size(); i++)
		objectVec1.push_back(objectVec2[i]);

	return objectVec1;
}

vector<GameObject> Game::objectSetup( const string shader, const string mesh,const string texture, const bool physics,
const Vector3 position, const Vector3 scale, const int amount, const int startNum, float offset)
{
	GameObject* object;
	vector<GameObject> objectVec;

	for (int i = startNum; i < amount; i++)
	{
		object = new GameObject;
		object->setShaderProgram(ObjectLoader::getShaderProgram(shader));
		object->setMesh(ObjectLoader::getMesh(mesh));
		object->setTexture(ObjectLoader::getTexture(texture));
		object->addPhysicsBody(physics);
		object->setPosition(Vector3(i * position.x + offset, position.y, position.z));
		object->setScale(scale);
		objectVec.push_back(*object);
	}
	return objectVec;
}

void Game::imguiDraw()
{
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		Vector3 pos = camera.getPosition();
		ImGui::Text("Camera Position: (%f, %f, %f)", pos.x, pos.y, pos.z);
		pos = player.getPosition();
		ImGui::Text("Player Pos: (%f, %f, %f)", pos.x, pos.y, pos.z);

		// Light settings
		if (ImGui::CollapsingHeader("Light Settings:"))
		{
			// Position settings
			Vector3 position = light->getPosition();
			ImGui::DragFloat3("Light Position: ", &position.x, 0.5f);
			light->setPosition(position);
			// Ambient settings
			Vector3 ambient = light->getAmbient();
			ImGui::ColorEdit3("Ambient Colour: ", &ambient.x);
			light->setAmbient(ambient);
			// Diffuse settings
			Vector3 diffuse = light->getDiffuse();
			ImGui::ColorEdit3("Diffuse Colour: ", &diffuse.x);
			light->setDiffuse(diffuse);
			// Specular settings
			Vector3 specular = light->getSpecular();
			ImGui::ColorEdit3("Specular Colour: ", &specular.x);
			light->setSpecular(specular);
			// Specular exponent settings
			float specularExp = light->getSpecularExp();
			ImGui::SliderFloat("Specular Exp: ", &specularExp, 0.0f, 250.0f);
			light->setSpecularExp(specularExp);
			// Attenuation constant settings
			float attenuationConstant = light->getAttenuationConstant();
			ImGui::SliderFloat("Attenuation Constant: ", &attenuationConstant, 0.0f, 20.0f);
			light->setAttenuationConstant(attenuationConstant);
			// Attenuation linear settings
			float attenuationLinear = light->getAttenuationLinear();
			ImGui::SliderFloat("Attenuation Linear: ", &attenuationLinear, 0.0f, 5.0f);
			light->setAttenuationLinear(attenuationLinear);
			// Attenuation quadratic settings
			float attenuationQuadratic = light->getAttenuationQuadratic();
			ImGui::SliderFloat("Attenuation Quadratic: ", &attenuationQuadratic, 0.0f, 5.0f);
			light->setAttenuationQuadratic(attenuationQuadratic);
		}


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;
#endif

	switch (key)
	{
	case 32:
		if (camera.getProjType() == ProjectionType::Perspective)
			camera.orthographic(-10.0f, 10.0f, 10.0f, -10.0f, -100.0f, 100.0f);
		else
		{
			float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
			camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
		}
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
	if (key == 'w' && collided && !sliding)
	{
		player.addForce(Vector2(0.0f, 100.0f));
	}
	if (key == 's' && collided)
	{
		player.setScale(0.1f);
		player.setPosition(Vector3(player.getPosition().x, 3.0f, player.getPosition().z));
		sliding = true;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;
#endif

	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
#ifdef _DEBUG

#endif
	if (key == 's' && collided)
	{
		//if(collided)
		player.setPosition(Vector3(player.getPosition().x, 3.5f, player.getPosition().z));
		sliding = false;
		player.setScale(0.2f);
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	//io.MouseDown[button] = state;

	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = true;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = true;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = true;
			break;
		}
	}
	else
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = false;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = false;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = false;
			break;
		}
	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
}
