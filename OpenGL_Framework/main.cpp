
#include <windows.h>
#include <iostream>
#include "Game.h"
#include <GL/freeglut.h>

Game *theGame;

/* function DisplayCallbackFunction(void)
 * Description:
 *  - this is the openGL display routine
 *  - this draws the sprites appropriately
 */
void DisplayCallbackFunction(void)
{
	theGame->draw();
}

/* function void KeyboardCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is pressed
 */
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardDown(key, x, y);

}
/* function void KeyboardUpCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is lifted
 */
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardUp(key, x, y);
}

/* function TimerCallbackFunction(int value)
 * Description: 
 *  - this is called many times per second
 *  - this enables you to animate things
 *  - no drawing, just changing the state
 *  - changes the frame number and calls for a redisplay
 *  - FRAME_DELAY_SPRITE is the number of milliseconds to wait before calling the timer again
 */
void TimerCallbackFunction(int value)
{
	theGame->update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY_SPRITE, TimerCallbackFunction, 0);
}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	theGame->mouseClicked(button,state, x, y);
	glutPostRedisplay();
}

/* function MouseMotionCallbackFunction()
 * Description:
 *   - this is called when the mouse is clicked and moves
 */
void MouseMotionCallbackFunction(int x, int y)
{
	theGame->mouseMoved(x, y);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	//glutInitContextVersion(4, 2);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("OpenGL Framework");

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW could not be initialized." << endl;
		system("pause");
		return 0;
	}
	//glewInit(); // gl* functions are captured from the drivers here.

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	
	/* init the game */
	theGame = new Game();
	theGame->initializeGame();

	/* start the game */
	glutMainLoop();
	return 0;
}