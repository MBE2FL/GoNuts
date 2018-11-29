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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag");
	ObjectLoader::loadShaderProgram("Player", "./Assets/Shaders/Morph.vert", "./Assets/Shaders/PassThrough.frag");

	ObjectLoader::loadMesh("FatBoi", "./Assets/Models/Fat_Boi_Ultimate_Rigged_Edition.obj");
	ObjectLoader::loadMesh("FatBoi2", "./Assets/Models/Very_Happy_Boi.obj");
	ObjectLoader::loadMesh("FatBoi3", "./Assets/Models/Not_Happy_Boi.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");	
	ObjectLoader::loadMesh("Platform2", "./Assets/Models/monkey.obj");
	
	
	ObjectLoader::loadTexture("Default", "./Assets/Textures/Default.png");
	ObjectLoader::loadTexture("Background", "./Assets/Textures/background.png");
	ObjectLoader::loadTexture("FatBoi", "./Assets/Textures/FatBoiTexture.png");
	//UI Textures
	ObjectLoader::loadTexture("FullNut", "./Assets/Textures/Nut_Final.png");
	ObjectLoader::loadTexture("Time", "./Assets/Textures/Time.png");

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


	ObjectLoader::loadMesh("TestBoi", "./Assets/Models/Animation/Fat Boi - Animated_", 20);

	player.setShaderProgram(ObjectLoader::getShaderProgram("Player"));
	player.Animated(true);
	player.setMesh(ObjectLoader::getMesh("TestBoi1"));
	//player.addMesh(ObjectLoader::getMesh("FatBoi2"));
	//player.addMesh(ObjectLoader::getMesh("FatBoi3"));
	//player.addMesh(ObjectLoader::getMesh("FatBoi"));
	player.addMesh("TestBoi", 19);
	player.setTexture(ObjectLoader::getTexture("FatBoi"));
	player.addPhysicsBody(true);
	player.setPosition(Vector3(-3.0f, 8.0f, -5.0f));
	player.setScale(0.2f);

	nutOmeter.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	nutOmeter.setMesh(ObjectLoader::getMesh("Plane"));
	nutOmeter.setTexture(ObjectLoader::getTexture("FullNut"));
	nutOmeter.addPhysicsBody(false);
	nutOmeter.setPosition(Vector3(0.0f, -6.0f, 0.0f));
	nutOmeter.setRotationAngleZ(3.14592f / 2.0f);
	nutOmeter.setRotationAngleY(3.14592f);
	nutOmeter.setScale(2.0f);

	time.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	time.setMesh(ObjectLoader::getMesh("Plane"));
	time.setTexture(ObjectLoader::getTexture("Time"));
	time.addPhysicsBody(false);
	time.setPosition(Vector3(-13.5f, 7.5f, 0.0f));
	time.setRotationAngleZ(3.14592f / 2.0f);
	time.setRotationAngleY(3.14592f);
	time.setScale(2.0f);

	coneBoi.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	coneBoi.setMesh(ObjectLoader::getMesh("Coin"));
	coneBoi.setTexture(ObjectLoader::getTexture("Default"));
	coneBoi.addPhysicsBody(false);
	coneBoi.setPosition(Vector3(4.0f, 6.0f, -5.0f));
	
	particleTrail = new ParticleEmitter;
	particleTrail->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	particleTrail->setMesh(ObjectLoader::getMesh("Plane"));
	particleTrail->setTexture(ObjectLoader::getTexture("Default"));
	particleTrail->addPhysicsBody(false);
	particleTrail->setPosition(Vector3(9.0f, 2.0f, -5.0f));
	particleTrail->setScale(Vector3(1, 1, 1));
	

	dynamic_cast<ParticleEmitter*>(particleTrail)->initialize(50);
	dynamic_cast<ParticleEmitter*>(particleTrail)->playing = false;

	jumpParticles = new ParticleEmitter;
	jumpParticles->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	jumpParticles->setMesh(ObjectLoader::getMesh("Plane"));
	jumpParticles->setTexture(ObjectLoader::getTexture("Default"));
	jumpParticles->addPhysicsBody(false);
	jumpParticles->setPosition(Vector3(0));
	jumpParticles->setScale(Vector3(0.1f));

	// Physics properties
	dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity0 = Vector3(-0.1f, 0.01f, -0.01f);
	dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity1 = Vector3(0.1f, 0.02f, 0.01f);
	dynamic_cast<ParticleEmitter*>(jumpParticles)->massRange = Vector2(1.0f, 2.0f);
	dynamic_cast<ParticleEmitter*>(jumpParticles)->emitterPosition = player.getPosition();

	// Visual Properties
	dynamic_cast<ParticleEmitter*>(jumpParticles)->lifeRange = Vector2(0.08f, 0.1f);

	dynamic_cast<ParticleEmitter*>(jumpParticles)->initialize(1);


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
	light->setSpecular(Vector3(0.f, 0.f, 0.f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	spotLight = new Light();
	spotLight->setPosition(Vector3(2.0f, 0.0f, 0.0f));
	spotLight->setAmbient(Vector3(1.0f, 1.0f, 1.0f));
	spotLight->setDiffuse(Vector3(0.7f, 0.3f, 0.2f));
	spotLight->setSpecular(Vector3(1.0f, 0.1f, 0.1f));
	spotLight->setSpecularExp(100.0f);
	spotLight->setAttenuationConstant(1.0f);
	spotLight->setAttenuationLinear(0.1f);
	spotLight->setAttenuationQuadratic(0.01f);

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

	UICamera.orthographic(-16, 16, 9, -9, -100, 100);
	UICamera.setPosition(Vector3::Zero);
}

void Game::update()
{
	if (!reverse)
		t += 0.01f;
	if (reverse)
		t -= 0.01f;

	if (t >= 1)
		reverse = true;
	if (t <= 0)
		reverse = false;
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	drawTime += deltaTime;

	dynamic_cast<ParticleEmitter*>(particleTrail)->emitterPosition = player.getPosition();
	dynamic_cast<ParticleEmitter*>(jumpParticles)->emitterPosition = Vector3(player.getPosition().x,
		player.getPosition().y - 1.f, player.getPosition().z);

	particleTrail->update(deltaTime);
	jumpParticles->update(deltaTime);
	player.update(deltaTime);
	nutOmeter.update(deltaTime);
	time.update(deltaTime);

	coneBoi.setPosition((MathLibCore::catmull(p1, p2,p3, p4, t)));

	coneBoi.update(deltaTime);
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
		{
			dynamic_cast<ParticleEmitter*>(jumpParticles)->playing = true;
			break;
		}
		else
		{
			dynamic_cast<ParticleEmitter*>(jumpParticles)->playing = false;
		}
	}

	
	if (player.getPhysicsBody()->getVelocity().x < 10.0f)
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

	Vector3 offset(-6, -1.5f, -8);
	camera.setPosition(MathLibCore::lerp( camera.getPosition(), player.getPosition() - offset, deltaTime * 3));
	camera.update(deltaTime);
	UICamera.update(deltaTime);
}

void Game::draw()
{
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
		glDisable(GL_BLEND);//MAKE SURE TO PUT ALL OPAQUE OBJECTS AFTER THIS, NO TRANSPARENT/TRANSLUCENT!!!!!!!




		//platforms[0].draw(UICamera, light, spotLight);
		particleTrail->draw(UICamera, light, spotLight);

		// Draw game objects
		particleTrail->draw(camera, light, spotLight);
		jumpParticles->draw(camera, light, spotLight);

		coneBoi.draw(camera, light, spotLight);
		//for (unsigned int i = 0; i < dynamic_cast<ParticleEmitter*>(footEmitter)->getNumParticles(); i++)
		//{
		//	dynamic_cast<ParticleEmitter*>(footEmitter)->m_pParticles[i]->draw(camera, light);
		//}
		player.draw(camera, light, spotLight);
		coneBoi.draw(camera, light, spotLight);

		for (unsigned int i = 0; i < coins.size(); i++)
		{
			coins[i].draw(camera, light, spotLight);
		}
		for (unsigned int i = 0; i < Background.size(); i++)
		{
			Background[i].draw(camera, light, spotLight);
		}
		for (unsigned int i = 0; i < sceneObjects.size(); i++)
		{
			sceneObjects[i].draw(camera, light, spotLight);
		}
		for (unsigned int i = 0; i < platforms.size(); i++)
		{
			platforms[i].draw(camera, light, spotLight);
		}
		for (unsigned int i = 0; i < upperPlatforms.size(); i++)
		{
			upperPlatforms[i].draw(camera, light, spotLight);
		}
		glEnable(GL_BLEND);//MAKE SURE TO PUT ALL TRANSPARENT/TRANSLUCENT OBJECTS AFTER THIS NO OPAQUE!!!!
		nutOmeter.draw(UICamera, light, spotLight);
		time.draw(UICamera, light, spotLight);



		// Update imgui draw data
		glUseProgram(GL_NONE);
#ifdef _DEBUG

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
		glutSwapBuffers();
		drawTime = 0.0f;
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
		ImGui::Text("T Value: %f", player.getanimation().getT());

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

		if (ImGui::CollapsingHeader("ConeBoi Catmull Settings:"))
		{
			ImGui::DragFloat3("p1 Position: ", &p1.x);
			ImGui::DragFloat3("p2 Position: ", &p2.x);
			ImGui::DragFloat3("p3 Position: ", &p3.x);
			ImGui::DragFloat3("p4 Position: ", &p4.x);
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
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
	if (key == 'w' && collided && !sliding)
	{
		player.addForce(Vector2(0.0f, 350.0f));
	}
	if (key == 's' && collided && !sliding)
	{
		player.setScale(0.1f);
		player.setPosition(Vector3(player.getPosition().x, player.getPosition().y - 0.4f, player.getPosition().z));
		sliding = true;
	}
	if (key == 'i')//up
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, LP.y, --LP.z));
	}
	if (key == 'k')//down
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, LP.y, ++LP.z));
	}
	if (key == 'j')//left
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(--LP.x, LP.y, LP.z));
	}
	if (key == 'l')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(++LP.x, LP.y, LP.z));
	}if (key == 'u')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, ++LP.y, LP.z));
	}if (key == 'o')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, --LP.y, LP.z));
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
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
#ifdef _DEBUG

#endif
	if (key == 's' && collided)
	{
		//if(collided)
		player.setPosition(Vector3(player.getPosition().x, player.getPosition().y + 0.4f, player.getPosition().z));
		sliding = false;
		player.setScale(0.2f);
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
#ifdef _DEBUG

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
#endif
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
