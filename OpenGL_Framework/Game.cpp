#include "Game.h"

#include <string>

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;

	//monkey.unLoad();
}

void Game::initializeGame()
{
	// OpenGL will not draw triangles hidden behind other geometry
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag");
	//ObjectLoader::loadMesh("Monkey", "./Assets/Models/Monkey.obj");
	ObjectLoader::loadMesh("FatBoi", "./Assets/Models/Fat_Boi_Ultimate_Rigged_Edition.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");
	ObjectLoader::loadMesh("Platform2", "./Assets/Models/monkey.obj");
	ObjectLoader::loadTexture("Default", "./Assets/Textures/Default.png");
	//ObjectLoader::loadMesh("Platform", "./Assets/Models/roof tile.obj");
	//ObjectLoader::loadMesh("Border", "./Assets/Models/roof board.obj");
	//ObjectLoader::loadMesh("BorderEdge", "./Assets/Models/roof board edge.obj");
	ObjectLoader::loadMesh("Acorn", "./Assets/Models/acorn.obj");
	ObjectLoader::loadMesh("Coin", "./Assets/Models/coin.obj");
	ObjectLoader::loadMesh("Cone", "./Assets/Models/cone.obj");
	ObjectLoader::loadMesh("Spikes", "./Assets/Models/spikes.obj");
	ObjectLoader::loadMesh("Vent", "./Assets/Models/vent.obj");


	player.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	player.setMesh(ObjectLoader::getMesh("Cube"));
	player.setTexture(ObjectLoader::getTexture("Default"));
	player.addPhysicsBody(true);
	player.setPosition(Vector3(-3.0f, 8.0f, -5.0f));


	plat = new GameObject;
	plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	plat->setMesh(ObjectLoader::getMesh("Acorn"));
	plat->setTexture(ObjectLoader::getTexture("Default"));	
	plat->setPosition(Vector3(0.0f, 4.0f, -5.0f));
	sceneObjects.push_back(*plat);

	plat = new GameObject;
	plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	plat->setMesh(ObjectLoader::getMesh("Coin"));
	plat->setTexture(ObjectLoader::getTexture("Default"));
	plat->setPosition(Vector3(2.0f, 4.5f, -5.0f));
	sceneObjects.push_back(*plat);

	plat = new GameObject;
	plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	plat->setMesh(ObjectLoader::getMesh("Cone"));
	plat->setTexture(ObjectLoader::getTexture("Default"));
	plat->setPosition(Vector3(4.0f, 4.0f, -5.0f));
	sceneObjects.push_back(*plat);

	plat = new GameObject;
	plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	plat->setMesh(ObjectLoader::getMesh("Spikes"));
	plat->setTexture(ObjectLoader::getTexture("Default"));
	plat->setPosition(Vector3(6.0f, 4.0f, -5.0f));
	sceneObjects.push_back(*plat);

	plat = new GameObject;
	plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	plat->setMesh(ObjectLoader::getMesh("Vent"));
	plat->setTexture(ObjectLoader::getTexture("Default"));
	plat->setPosition(Vector3(8.0f, 4.0f, -5.0f));
	sceneObjects.push_back(*plat);

	for (float i = 0.0f; i < 20; i++)
	{
		plat = new GameObject;
		plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
		plat->setMesh(ObjectLoader::getMesh("Platform"));
		plat->setTexture(ObjectLoader::getTexture("Default"));
		plat->addPhysicsBody(false);
		plat->setPosition(Vector3(i * 9.0f, 2.0f, -5.0f));
		plat->setScale(Vector3(1, 1, 2));

		platforms.push_back(*plat);
	}

	for (float i = 5.0f; i < 10; i++)
	{
		plat = new GameObject;
		plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
		plat->setMesh(ObjectLoader::getMesh("Platform"));
		plat->setTexture(ObjectLoader::getTexture("Default"));
		plat->addPhysicsBody(false);
		plat->setPosition(Vector3(i * 9.0f, 3.5f, -5.0f));
		plat->setScale(Vector3(0.5, 1, 1));

		platforms.push_back(*plat);
	}

	for (float i = 15.0f; i < 20; i++)
	{
		plat = new GameObject;
		plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
		plat->setMesh(ObjectLoader::getMesh("Platform"));
		plat->setTexture(ObjectLoader::getTexture("Default"));
		plat->addPhysicsBody(false);
		plat->setPosition(Vector3(i * 9.0f, 3.5f, -5.0f));
		plat->setScale(Vector3(0.5, 1, 1));

		platforms.push_back(*plat);
	}


	//platOne.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//platOne.setMesh(ObjectLoader::getMesh("Platform"));
	//platOne.addPhysicsBody(false);
	//platOne.setPosition(Vector3(0.0f, -2.0f, 0.0f));
	//
	//platTwo.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//platTwo.setMesh(ObjectLoader::getMesh("Platform"));
	//platTwo.addPhysicsBody(false);
	//platTwo.setPosition(Vector3(9.0f, -2.0f, 0.0f));

	//platOne.setScale(0.4f);


	light = new Light();
	light->setPosition(Vector3(4.0f, 0.0f, 0.0f));
	light->setAmbient(Vector3(1.0f, 1.0f, 1.0f));
	light->setDiffuse(Vector3(0.7f, 0.1f, 0.2f));
	light->setSpecular(Vector3(1.0f, 0.1f, 0.1f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);


	Matrix44 test;
	test.mV[0] = 1; test.mV[1] = 3; test.mV[2] = 4; test.mV[3] = 4; test.mV[4] = 5; test.mV[5] = 8; test.mV[6] = 7;
	test.mV[7] = 8; test.mV[8] = 9; test.mV[9] = 10; test.mV[10] = 11; test.mV[11] = 12; test.mV[12] = 13; test.mV[13] = 14;
	test.mV[14] = 15; test.mV[15] = 16;

	test.GetInverse();

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

	player.update(deltaTime);
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].update(deltaTime);
		sceneObjects[i].setRotationAngleY(TotalGameTime * 1.05f);
	}
	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		platforms[i].update(deltaTime);
	}
	//platOne.update(deltaTime);
	//platTwo.update(deltaTime);
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
		player.setPosition(Vector3(-3.0f, 3, -5.0f));
		player.setVelocity(Vector2(0.0f, 0.0f));
	}
	
	counter += deltaTime;
	player.setRotationAngleY(counter);
	//std::cout << deltaTime << std::endl;
	//if (colliding && !collided)
	//{
	//	//player.addForce(Vector2(0.0f, 20.0f));
	//	player.useGravity(false);
	//	//player.setVelocity(Vector2::Zero);
	//	if (col == 1)
	//	{
	//		float ySpeed = player.getPhysicsBody()->getVelocity().y;
	//		player.addForce(Vector2(0.0f, -ySpeed / deltaTime * 1.2f));
	//	}
	//	else
	//	{
	//		float xSpeed = player.getPhysicsBody()->getVelocity().x;
	//		player.addForce(Vector2(-xSpeed / deltaTime * 1.2f, 0.0f));
	//	}

	//	collided = true;
	//}
	//else if (!colliding && collided)
	//{
	//	player.useGravity(true);
	//	collided = false;
	//}

	Vector3 offset(-3, -1.5f, -8);
	camera.setPosition(player.getPosition() - offset);
	camera.update(deltaTime);

}

void Game::draw()
{
	// Completely clear the Back-Buffer before doing any work.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// New imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	// Update imgui widgets
	imguiDraw();

	// Render imgui
	ImGui::Render();

	// Draw game objects
	player.draw(camera, light);
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].draw(camera, light);
	}
	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		platforms[i].draw(camera, light);
	}
	   
	// Update imgui draw data
	glUseProgram(GL_NONE);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
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
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;

	switch(key)
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
//	case 'd':
//		player.addForce(Vector2(5.0f, 0.0f));
//		break;
//	case 'a':
//		player.addForce(Vector2(-5.0f, 0.0f));
//		break;
//	case 'w':
//		player.addForce(Vector2(0.0f, 10.0f));
//		break;
	}
	//if (key == 'd' && player.getPhysicsBody()->getVelocity().x < 5.0f)
	//{
	//	player.addForce(Vector2(20.0f, 0.0f));
	//}
	//if (key == 'a' && player.getPhysicsBody()->getVelocity().x > -5.0f)
	//{
	//	player.addForce(Vector2(-20.0f, 0.0f));
	//}
	if (key == 'w' && collided)
	{
		player.addForce(Vector2(0.0f, 100.0f));
	}
	if (key == 's')
	{
		player.setScale(Vector3(1, 0.5, 1));
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;

	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}

	if (key == 's')
	{
		player.setScale(1);
		player.setPosition(Vector3(player.getPosition().x, 2.8f, player.getPosition().z));
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
