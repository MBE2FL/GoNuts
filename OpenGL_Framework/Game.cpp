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
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough - Copy.frag");
	ObjectLoader::loadShaderProgram("Player", "./Assets/Shaders/Morph.vert", "./Assets/Shaders/PassThrough.frag");
	ObjectLoader::loadShaderProgram("Water", "./Assets/Shaders/waterShader.vert", "./Assets/Shaders/waterShader.frag");

	ObjectLoader::loadMesh("Acorn", "./Assets/Models/acorn.obj");
	ObjectLoader::loadMesh("Background", "./Assets/Models/background.obj");
	ObjectLoader::loadMesh("Billboard", "./Assets/Models/Billboard_Unwrapped.obj");
	ObjectLoader::loadMesh("Building", "./Assets/Models/Building Model.obj");
	ObjectLoader::loadMesh("Button", "./Assets/Models/button_final_unwrap.obj");
	ObjectLoader::loadMesh("Chair", "./Assets/Models/chair_final_unwrap.obj");
	ObjectLoader::loadMesh("Checkpoint Squirrel", "./Assets/Models/checkpointsquirrel_unwrap.obj");
	ObjectLoader::loadMesh("Chimney", "./Assets/Models/chimney_unwrap.obj");
	ObjectLoader::loadMesh("Clothesline", "./Assets/Models/clothesline_unwrapped_final.obj");
	ObjectLoader::loadMesh("Coin", "./Assets/Models/coin.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Cone", "./Assets/Models/cone.obj");
	ObjectLoader::loadMesh("FatBoi", "./Assets/Models/Fat_Boi_Ultimate_Rigged_Edition.obj");
	ObjectLoader::loadMesh("Flag", "./Assets/Models/flag_final_unwrap.obj");
	ObjectLoader::loadMesh("Garbage", "./Assets/Models/garbagecan_final_unwrap.obj");
	ObjectLoader::loadMesh("Lever", "./Assets/Models/lever.obj");
	ObjectLoader::loadMesh("Plane", "./Assets/Models/plane.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");
	ObjectLoader::loadMesh("Spikes", "./Assets/Models/spikes.obj");
	ObjectLoader::loadMesh("Lamp", "./Assets/Models/Street Lamp_final_unwrap.obj");
	ObjectLoader::loadMesh("Raccoon", "./Assets/Models/raccoon_unwrap.obj");
	ObjectLoader::loadMesh("Squirrel", "./Assets/Models/squirrel_unwrap.obj");
	ObjectLoader::loadMesh("Table", "./Assets/Models/table_final_unwrap.obj");
	ObjectLoader::loadMesh("Vent", "./Assets/Models/vent.obj");


	ObjectLoader::loadMesh("TestBoi", "./Assets/Models/Animation/Fat Boi - Animated_", 20);

	ObjectLoader::loadTexture("Default", "./Assets/Textures/Default.png");

	ObjectLoader::loadTexture("Acorn", "./Assets/Textures/Acorn_Texture.png");
	ObjectLoader::loadTexture("Background", "./Assets/Textures/new_background.png");
	ObjectLoader::loadTexture("Building", "./Assets/Textures/Building Layout.png");
	ObjectLoader::loadTexture("Billboard", "./Assets/Textures/Billboard_Texture.png");
	ObjectLoader::loadTexture("Billboard1", "./Assets/Textures/Billboard_Texture1.png");
	ObjectLoader::loadTexture("Button", "./Assets/Textures/Button_Texture.png");
	ObjectLoader::loadTexture("Chair", "./Assets/Textures/chair tex.png");
	ObjectLoader::loadTexture("Checkpoint Squirrel", "./Assets/Textures/squirrel checkpoint_tex.png");
	ObjectLoader::loadTexture("Chimney", "./Assets/Textures/Chimney Texture.png");
	ObjectLoader::loadTexture("Clothesline", "./Assets/Textures/clothingline_texture.png");
	ObjectLoader::loadTexture("Coin", "./Assets/Textures/coin_tex.png");
	ObjectLoader::loadTexture("Cone", "./Assets/Textures/Cone_Texture.png");
	ObjectLoader::loadTexture("Dust", "./Assets/Textures/Dust_Trail.png");
	ObjectLoader::loadTexture("FatBoi", "./Assets/Textures/FatBoiTexture.png");
	ObjectLoader::loadTexture("Flag", "./Assets/Textures/flag tex.png");
	ObjectLoader::loadTexture("Garbage", "./Assets/Textures/garbage.png");
	ObjectLoader::loadTexture("Lamp", "./Assets/Textures/Lamp Post Texture.png");
	ObjectLoader::loadTexture("Lever", "./Assets/Textures/Lever_Texture.png");
	ObjectLoader::loadTexture("Lever1", "./Assets/Textures/Lever_Texture1.png");
	ObjectLoader::loadTexture("Raccoon", "./Assets/Textures/raccoon_texture.png");
	ObjectLoader::loadTexture("Spikes", "./Assets/Textures/Spike Texture.png");
	ObjectLoader::loadTexture("Squirrel", "./Assets/Textures/squirrel_texture.png");
	ObjectLoader::loadTexture("Table", "./Assets/Textures/Table_Texture.jpg");
	ObjectLoader::loadTexture("Vent", "./Assets/Textures/Vent_Texture.png");
	//UI Textures
	ObjectLoader::loadTexture("Nut0", "./Assets/Textures/Nut_0.png");
	ObjectLoader::loadTexture("Nut10", "./Assets/Textures/Nut_10.png");
	ObjectLoader::loadTexture("Nut20", "./Assets/Textures/Nut_20.png");
	ObjectLoader::loadTexture("Nut30", "./Assets/Textures/Nut_30.png");
	ObjectLoader::loadTexture("Nut40", "./Assets/Textures/Nut_40.png");
	ObjectLoader::loadTexture("Nut50", "./Assets/Textures/Nut_50.png");
	ObjectLoader::loadTexture("Nut60", "./Assets/Textures/Nut_60.png");
	ObjectLoader::loadTexture("Nut70", "./Assets/Textures/Nut_70.png");
	ObjectLoader::loadTexture("Nut80", "./Assets/Textures/Nut_80.png");
	ObjectLoader::loadTexture("Nut90", "./Assets/Textures/Nut_90.png");
	ObjectLoader::loadTexture("Nut100", "./Assets/Textures/Nut_100.png");
	ObjectLoader::loadTexture("FullNut", "./Assets/Textures/Nut_Final.png");
	ObjectLoader::loadTexture("Time", "./Assets/Textures/Time.png");

	ObjectLoader::loadTexture("Water", "./Assets/Textures/water.png");
	ObjectLoader::loadTexture("Toon1", "./Assets/Textures/toon.png");
	ObjectLoader::loadTexture("Toon", "./Assets/Textures/toon1.png");
	ObjectLoader::loadTexture("Toon2", "./Assets/Textures/toon2.png");
	ObjectLoader::loadTexture("Toon3", "./Assets/Textures/toon3.png");
	ObjectLoader::loadTexture("Smooth", "./Assets/Textures/Smooth Shading.png");
	ObjectLoader::loadTexture("Normal", "./Assets/Textures/Acorn_Texture_n.png");

	//nutOmeter.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//nutOmeter.setMesh(ObjectLoader::getMesh("Plane"));
	//nutOmeter.setTexture(ObjectLoader::getTexture("FullNut"));
	//nutOmeter.addPhysicsBody(false);
	//nutOmeter.setWorldPosition(Vector3(0.0f, -6.0f, 0.0f));
	//nutOmeter.setLocalRotationAngleZ(-3.14592f / 2.0f);
	////nutOmeter.setLocalRotationAngleY(3.14592f);
	//nutOmeter.setLocalScale(2.0f);

	//time.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//time.setMesh(ObjectLoader::getMesh("Plane"));
	//time.setTexture(ObjectLoader::getTexture("Time"));
	//time.addPhysicsBody(false);
	//time.setWorldPosition(Vector3(-13.5f, 7.5f, 0.0f));
	//time.setLocalRotationAngleZ(-3.14592f / 2.0f);
	////time.setLocalRotationAngleY(3.14592f);
	//time.setLocalScale(2.0f);

	//particleTrail = new ParticleEmitter;
	//particleTrail->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//particleTrail->setMesh(ObjectLoader::getMesh("Plane"));
	//particleTrail->setTexture(ObjectLoader::getTexture("Default"));
	//particleTrail->addPhysicsBody(false);
	//particleTrail->setWorldPosition(Vector3(9.0f, 2.0f, -5.0f));
	//particleTrail->setLocalScale(Vector3(1, 1, 1));
	//dynamic_cast<ParticleEmitter*>(particleTrail)->texName = "Dust";
	//dynamic_cast<ParticleEmitter*>(particleTrail)->velocity0 = Vector3(-0.1f, -0.01f, -0.01f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->velocity1 = Vector3(-0.1f, -0.01f, 0.01f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->massRange = Vector2(1.0f, 2.0f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->emitterPosition = player.getWorldPosition();

	//// Visual Properties
	//dynamic_cast<ParticleEmitter*>(particleTrail)->lifeRange = Vector2(0.8f, 1.0f);
	//

	//dynamic_cast<ParticleEmitter*>(particleTrail)->initialize(50);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->playing = false;

	//jumpParticles = new ParticleEmitter;
	//jumpParticles->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//jumpParticles->setMesh(ObjectLoader::getMesh("Plane"));
	//jumpParticles->setTexture(ObjectLoader::getTexture("Dust"));
	//jumpParticles->addPhysicsBody(false);
	//jumpParticles->setParent(&player);
	//jumpParticles->setLocalPosition(Vector3::Zero);
	//jumpParticles->setLocalScale(Vector3(0.5f));
	//jumpParticles->setLocalRotationAngleY(90.0f);

	// Physics properties
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->texName = "Dust";
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity0 = Vector3(-0.1f, 0.01f, -0.01f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity1 = Vector3(0.1f, 0.02f, 0.01f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->massRange = Vector2(1.0f, 2.0f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->emitterPosition = player.getWorldPosition();

	// Visual Properties
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->lifeRange = Vector2(0.08f, 0.1f);

	//dynamic_cast<ParticleEmitter*>(jumpParticles)->initialize(5);

	light = new Light();
	light->setPosition(Vector3(4.0f, 0.0f, 0.0f));
	light->setAmbient(Vector3(0.60f, .60f, 0.60f));
	//light->setAmbient(Vector3(0));
	light->setDiffuse(Vector3(0.7f, 0.7f, 0.7f));
	//light->setDiffuse(Vector3(0));
	light->setSpecular(Vector3(0.70f, 0.7f, 0.7f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	spotLight = new Light();
	spotLight->setPosition(Vector3(-3.2f, 30.0f, -28.0f));
	spotLight->setAmbient(Vector3(1.0f, 1.0f, 1.0f));
	spotLight->setDiffuse(Vector3(1));
	spotLight->setSpecular(Vector3(1.0f, 0.1f, 0.1f));
	spotLight->setSpecularExp(100.0f);
	spotLight->setAttenuationConstant(0.1f);
	spotLight->setAttenuationLinear(0.01f);
	spotLight->setAttenuationQuadratic(0.01f);


	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	//camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	////camera.orthographic(-10, 10, -10, 10, -100, 100);
	//camera.setWorldPosition(Vector3(0.0f, 4.0f, 5.0f));
	//camera.setLocalRotationAngleX(camera.getLocalRotationAngleX() - 0.2f);

	//UICamera.orthographic(-16, 16, 9, -9, -100, 100);
	//UICamera.setWorldPosition(Vector3::Zero);


	_entityManager = EntityManager::getInstance();
	_transformSystem = new TransformSystem(_entityManager);
	_meshRendererSystem = new MeshRendererSystem(_entityManager);
	_physicsSystem = new PhysicsSystem(_entityManager);
	_entityFactory = EntityFactory::getInstance();

#ifdef _DEBUG
	_guiHelper = GUIHelper::getInstance();
#endif

	//Entity* test = _entityFactory->createEmpty();
	//TransformComponent* trans = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, test);
	//bool blah = trans->isRoot();
	//blah = !blah;

	Entity* mainCamera = _entityFactory->createPerspectiveCamera(Vector3(0.0f, 4.0f, 5.0f), 60.0f, aspect, 1.0f, 1000.0f);
	_mainCameraTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, mainCamera);

	Entity* player = _entityFactory->createPlayer(Vector3(-3.0f, 10.0f, -5.0f), Vector3(0.2f));
	_playerTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, player);
	_playerPhysicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, player);
	EntityManager::setPlayerTransform(_playerTransform);

	Entity* entity = _entityFactory->createCoin(Vector3(2.0f, 4.0f, -5.0f), Vector3::One);
	_entityFactory->createCoin(Vector3(2.0f, 0.0f, 0.0f), Vector3::One, entity);

	_entityFactory->createBackgrounds(20, Vector3(100.0f, -5.0f, -20.0f), Vector3(25.0f, 25.0f, 1.0f));

	_entityFactory->createPlatforms(15, Vector3(14.0f, -2.0f, -5.0f));

	_entityFactory->createCoins(1, Vector3(35.0f, 4.0f, -5.0f), Vector3::One, 0);
	_entityFactory->createCoins(2, Vector3(42.0f, 4.5f, -5.0f), Vector3::One, 35.0f);
	_entityFactory->createAcorns(3, Vector3(2.0f, 4.0f, -5.0f), Vector3::One, 12.0f);
	_entityFactory->createAcorns(3, Vector3(2.0f, 4.0f, -5.0f), Vector3::One, 54.0f);

	_entityFactory->createCones(2, Vector3(14.0f, 2.6f, -5.0f), Vector3::One, 70.0f);
	_entityFactory->createSpikes(2, Vector3(14.0f, 2.8f, -5.0f), Vector3::One, 28.0f);
	_entityFactory->createVents(2, Vector3(15.0f, 2.95f, -5.0f), Vector3::One, 98.0f);
	_entityFactory->createTopPlatforms(5, Vector3(14.0f, 4.2f, -5.0f), Vector3(0.4f, 1, 1), 125.0f);
}

void Game::update()
{
	collided = false;
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

	if (_playerTransform->getLocalPosition().y < -6.0f)
	{
		_playerTransform->setWorldPosition(Vector3(-3.0f, 8.0f, -5.0f));
		_playerPhysicsBody->setVelocity(Vector3::Zero);
		_playerTransform->setLocalScale(0.2f);
	}

	_transformSystem->update(FIXED_DELTA_TIME);
	_physicsSystem->update(FIXED_DELTA_TIME);



	//dynamic_cast<ParticleEmitter*>(particleTrail)->emitterPosition = player.getWorldPosition();



	//particleTrail->getParent()->update(deltaTime);
	//player.getParent()->update(deltaTime);
	//spotLight->setPosition(Vector3(player.getWorldPosition().x, 5.0f, player.getWorldPosition().z) + offse);
	//nutOmeter.getParent()->update(deltaTime);
	//time.getParent()->update(deltaTime);

	counter += deltaTime;

	Vector3 offset(-6, -1.5f, -8);
	_mainCameraTransform->setWorldPosition(MathLibCore::lerp(_mainCameraTransform->getWorldPosition(),
		_playerTransform->getWorldPosition() - offset, deltaTime * 3.0f));
	//camera.setWorldPosition(MathLibCore::lerp(camera.getWorldPosition(), playerTransform->getWorldPosition() - offset, deltaTime * 3.0f));
	//camera.update(deltaTime);
	//UICamera.update(deltaTime);
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
	//glDisable(GL_BLEND);//MAKE SURE TO PUT ALL OPAQUE OBJECTS AFTER THIS, NO TRANSPARENT/TRANSLUCENT!!!!!!!


	//Matrix44 cameraInverse = camera.getLocalToWorldMatrix().GetInverse(camera.getWorldRotation(), camera.getWorldPosition());
	//Matrix44 uiCameraInverse = UICamera.getLocalToWorldMatrix().GetInverse(UICamera.getWorldRotation(), UICamera.getWorldPosition());

	_meshRendererSystem->draw(light, spotLight);

	//glEnable(GL_BLEND);//MAKE SURE TO PUT ALL TRANSPARENT/TRANSLUCENT OBJECTS AFTER THIS NO OPAQUE!!!!


	//nutOmeter.draw(UICamera, light, spotLight, uiCameraInverse);
	//time.draw(UICamera, light, spotLight, uiCameraInverse);



	// Update imgui draw data
	glUseProgram(GL_NONE);
#ifdef _DEBUG

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
	drawTime = 0.0f;
}

void Game::imguiDraw()
{
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		Vector3 pos; //= camera.getWorldPosition();
		//ImGui::Text("Camera Position: (%f, %f, %f)", pos.x, pos.y, pos.z);
		//ImGui::DragFloat3("Camera Position: ", &pos.x, 0.5f);
		//camera.setWorldPosition(pos);
		pos = _playerTransform->getWorldPosition();
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


			//Vector3 spotPositiion = spotLight->getPosition();
			ImGui::SliderFloat3("Spot Position: ", &offse.x, -60.f, 60.f);
			//spotLight->setPosition(spotPositiion);
		}

		_guiHelper->draw();


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;

	if (io.WantCaptureKeyboard)
	{
		io.AddInputCharacter(key);
		return;
	}
#endif

	switch (key)
	{
	case 32:
		/*if (camera.getProjType() == ProjectionType::Perspective)
			camera.orthographic(-10.0f, 10.0f, 10.0f, -10.0f, -100.0f, 100.0f);
		else
		{
			float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
			camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
		}*/
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
	if (key == 'w'  && !sliding && _playerPhysicsBody->getCanJump())
	{
		_playerPhysicsBody->addForce(Vector3(0, 350.0f, 0.0f));
	}
	if (key == 's' && _playerPhysicsBody->getCanJump())
	{
		if (!slideCheck)
		{
			slideCheck = true;
			sliding = true;
			_playerTransform->setLocalScale(0.1f);
			_playerTransform->setWorldPosition(Vector3(_playerTransform->getLocalPosition().x,
				_playerTransform->getLocalPosition().y - 0.38f, _playerTransform->getWorldPosition().z));
		}
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

	if (io.WantCaptureKeyboard)
		return;
#endif

	switch (key)
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
	if (key == 's')
	{
		if (sliding)
		{
			slideCheck = false;
			_playerTransform->setWorldPosition(Vector3(_playerTransform->getWorldPosition().x,
				_playerTransform->getWorldPosition().y + 0.45f, _playerTransform->getWorldPosition().z));
			sliding = false;
			_playerTransform->setLocalScale(0.2f);
		}
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	//io.MouseDown[button] = state;

	if (state == GLUT_DOWN)
	{
		switch (button)
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
