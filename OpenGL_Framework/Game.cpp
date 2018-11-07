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

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag");
	//ObjectLoader::loadMesh("Monkey", "./Assets/Models/Monkey.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");
	//ObjectLoader::loadMesh("Platform", "./Assets/Models/roof tile.obj");
	//ObjectLoader::loadMesh("Border", "./Assets/Models/roof board.obj");
	//ObjectLoader::loadMesh("BorderEdge", "./Assets/Models/roof board edge.obj");

	player.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	player.setMesh(ObjectLoader::getMesh("Cube"));
	player.addPhysicsBody(true);
	player.setPosition(Vector3(-3.0f, -1.0f, 0.0f));

	for (float i = 0.0f; i < 20; i++)
	{
		plat = new GameObject;
		plat->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
		plat->setMesh(ObjectLoader::getMesh("Platform"));
		plat->addPhysicsBody(false);
		plat->setPosition(Vector3(i * 9.0f, -2.0f, 0.0f));

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
	for (unsigned int i = 0; i < 20; i++)
	{
		platforms[i].update(deltaTime);
	}
	//platOne.update(deltaTime);
	//platTwo.update(deltaTime);
	for (unsigned int i = 0; i < 20; i++)
	{
		collided = player.checkCollisions(platforms[i]);
		if (collided)
			break;
	}
	
	
	if (player.getPhysicsBody()->getVelocity().x < 5.0f)
		player.addForce(Vector2(20.0f, 0.0f));

	if (player.getPosition().y < -4.0f || player.getPhysicsBody()->getVelocity().x < 0.0f)
	{
		player.setPosition(Vector3(-3.0f, -0.99f, 0.0f));
		player.setVelocity(Vector2(0.0f, 0.0f));
	}
	
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

	Vector3 offset(-3, -1.5f, -4);
	camera.setPosition(player.getPosition() - offset);
	camera.update(deltaTime);

}

void Game::draw()
{
	// Completely clear the Back-Buffer before doing any work.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	player.draw(camera);
	for (unsigned int i = 0; i < 20; i++)
	{
		platforms[i].draw(camera);
	}
	   

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
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
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

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
}
