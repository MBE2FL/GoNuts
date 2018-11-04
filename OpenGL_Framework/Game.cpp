#include "Game.h"

#include <string>

//const char* vertexShaderSource = 
//	"#version 330\n"
//	"layout(location = 0) in Vector3 mesh_position;\n"
//	"layout(location = 1) in Vector3 mesh_color;\n"
//	"uniform mat4 u_model;\n"
//	"uniform mat4 u_view;\n"
//	"uniform mat4 u_projection;\n"
//	"out Vector3 color;\n"
//	"void main()\n"
//	"{\n"
//	"	gl_Position = u_projection * u_view * u_model * vec4(mesh_position, 1.0);\n"
//	"	color = mesh_color;\n"
//	"}\n";
//
//const char* fragmentShaderSource = 
//	"#version 330\n"
//	"in Vector3 color;\n"
//	"out vec4 pixelColor;\n"
//	"void main() { pixelColor = vec4(color, 1.0f); }\n";



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

	// Load shaders
	//monkey.loadShaderProgram("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag");

	// Load mesh
	//monkey.loadMesh("./Assets/Models/Monkey.obj");

	//monkey.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//monkey.setMesh(ObjectLoader::getMesh("Monkey"));

	//monkeyTwo.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//monkeyTwo.setMesh(ObjectLoader::getMesh("Monkey"));

	//cube.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//cube.setMesh(ObjectLoader::getMesh("Cube"));

	//monkeyTwo.setPosition(Vector3(3.0f, 0.0f, 0.0f));
	//cube.setPosition(Vector3(-3.0f, 2.0f, 0.0f));


	player.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	player.setMesh(ObjectLoader::getMesh("Cube"));
	player.addPhysicsBody(true);
	player.setPosition(Vector3(0.0f, 10.0f, 0.0f));

	platOne.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	platOne.setMesh(ObjectLoader::getMesh("Platform"));
	platOne.addPhysicsBody(false);
	platOne.setPosition(Vector3(0.0f, -2.0f, 0.0f));
	//platOne.setScale(0.4f);

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	camera.setPosition(Vector3(0.0f, 0.0f, 5.0f));
	//camera.setRotationAngleX(-45.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	player.update(deltaTime);
	platOne.update(deltaTime);


	bool colliding = player.checkCollisions(platOne);
	if (colliding && !collided)
	{
		//player.addForce(Vector2(0.0f, 20.0f));
		player.useGravity(false);
		//player.setVelocity(Vector2::Zero);
		float ySpeed = player.getPhysicsBody().getVelocity().y;
		player.addForce(Vector2(0.0f, -ySpeed/deltaTime *1.2f));

		collided = true;
	}
	else if (!colliding && collided)
	{
		player.useGravity(true);
		collided = false;
	}

	//monkey.setRotationAngleY(TotalGameTime * 45.0f);
	//monkeyTwo.setRotationAngleY(TotalGameTime * -25.0f);
	//cube.setRotationAngleY(TotalGameTime * 25.0f);

	camera.update(deltaTime);
	//monkey.update(deltaTime);
	//monkeyTwo.update(deltaTime);
	//cube.update(deltaTime);
}

void Game::draw()
{
	// Completely clear the Back-Buffer before doing any work.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//monkey.draw(camera);
	//monkeyTwo.draw(camera);
	//cube.draw(camera);

	player.draw(camera);
	platOne.draw(camera);
	   

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
	if (key == 'd' && player.getPhysicsBody().getVelocity().x < 5.0f)
	{
		player.addForce(Vector2(20.0f, 0.0f));
	}
	else if (key == 'a' && player.getPhysicsBody().getVelocity().x > -5.0f)
	{
		player.addForce(Vector2(-20.0f, 0.0f));
	}
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
