#include "Game.h"

#include <string>

//#include "SkeletalMesh.h"

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

	Framebuffer::initFrameBuffers();
	gbuffer.init(1900, 1000);
	//frameBufferOutline.addDepthTarget();
	frameBufferOutline.addColorTarget(GL_RGB8);
	frameBufferOutline.init(1900, 1000);

	frameBufferLUT.addColorTarget(GL_RGB8);
	frameBufferLUT.init(1900, 1000);

	frameBufferUI.addColorTarget(GL_RGB8);
	frameBufferUI.init(1900, 1000);

	frameBufferShadow.addDepthTarget();
	frameBufferShadow.init(8192, 8192);

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/shader.vert", "./Assets/Shaders/gBuffer.frag");
	ObjectLoader::loadShaderProgram("Player", "./Assets/Shaders/Morph.vert", "./Assets/Shaders/PassThrough.frag");
	ObjectLoader::loadShaderProgram("Water", "./Assets/Shaders/waterShader.vert", "./Assets/Shaders/waterShader.frag");
	ObjectLoader::loadShaderProgram("BBox", "./Assets/Shaders/BBox.vert", "./Assets/Shaders/BBox.frag");
	ObjectLoader::loadShaderProgram("SkeletalAnim", "./Assets/Shaders/SkeletalAnim.vert", "./Assets/Shaders/gBuffer.frag");
	ObjectLoader::loadShaderProgram("UIShader", "./Assets/Shaders/shader.vert", "./Assets/Shaders/gBuffer.frag");
	ObjectLoader::loadShaderProgram("FreeType", "./Assets/Shaders/font.vert", "./Assets/Shaders/font.frag");


	shaderGbuffer.load("./Assets/Shaders/shader.vert", "./Assets/Shaders/PassThrough - Copy.frag");
	shaderOutline.load("./Assets/Shaders/Post.vert", "./Assets/Shaders/Post.frag");
	shaderLUT.load("./Assets/Shaders/Post.vert", "./Assets/Shaders/LUT.frag");
	LUTTex = new Texture("./Assets/Textures/Warm_LUT_GDW.cube", true);
	//LUTTexVal = new Texture("./Assets/Textures/newpinkfilter.cube", true);

	toonRamp = new Texture("./Assets/Textures/toon1.png");

	ObjectLoader::loadMesh("Acorn", "./Assets/Models/acorn.obj");
	ObjectLoader::loadMesh("Background", "./Assets/Models/background.obj");
	ObjectLoader::loadMesh("Billboard", "./Assets/Models/Billboard_Unwrapped.obj");
	ObjectLoader::loadMesh("Building", "./Assets/Models/Building Model.obj");
	ObjectLoader::loadMesh("Building 1", "./Assets/Models/Building1.obj");
	ObjectLoader::loadMesh("Building 2", "./Assets/Models/Building2.obj");
	ObjectLoader::loadMesh("Building 3", "./Assets/Models/Building3.obj");
	ObjectLoader::loadMesh("Button", "./Assets/Models/button_final_unwrap.obj");
	//Change
	ObjectLoader::loadMesh("Checkpoint Squirrel", "./Assets/Models/checkpointsquirrel_unwrap.obj");
	ObjectLoader::loadMesh("Chimney", "./Assets/Models/chimney_unwrap.obj");
	//Change
	ObjectLoader::loadMesh("Clothesline", "./Assets/Models/clothesline_unwrapped_final.obj");
	ObjectLoader::loadMesh("Coin", "./Assets/Models/coin.obj");
	ObjectLoader::loadMesh("Cube", "./Assets/Models/Cube.obj");
	ObjectLoader::loadMesh("Cone", "./Assets/Models/cone.obj");
	ObjectLoader::loadMesh("Couch", "./Assets/Models/couch.obj");
	ObjectLoader::loadMesh("Table", "./Assets/Models/table.obj");
	ObjectLoader::loadMesh("Round Table", "./Assets/Models/roundtable.obj");
	ObjectLoader::loadMesh("FatBoi", "./Assets/Models/Fat_Boi_Ultimate_Rigged_Edition.obj");
	//Change
	ObjectLoader::loadMesh("Flag", "./Assets/Models/flag_final_unwrap.obj");
	//Change
	ObjectLoader::loadMesh("Garbage", "./Assets/Models/garbagecan_final_unwrap.obj");
	ObjectLoader::loadMesh("Lever", "./Assets/Models/lever.obj");
	ObjectLoader::loadMesh("Lamp", "./Assets/Models/lamp.obj");
	ObjectLoader::loadMesh("Plane", "./Assets/Models/plane.obj");
	ObjectLoader::loadMesh("Plant", "./Assets/Models/smallplant.obj");
	ObjectLoader::loadMesh("Planter", "./Assets/Models/planter.obj");
	ObjectLoader::loadMesh("Platform", "./Assets/Models/Platform.obj");
	ObjectLoader::loadMesh("Spikes", "./Assets/Models/spikes.obj");
	ObjectLoader::loadMesh("Raccoon", "./Assets/Models/raccoon_unwrap.obj");
	ObjectLoader::loadMesh("Squirrel", "./Assets/Models/squirrel_unwrap.obj");
	ObjectLoader::loadMesh("Vent", "./Assets/Models/vent.obj");

	ObjectLoader::loadMesh("Rooftop Door", "./Assets/Models/rooftopdoor.obj");

	ObjectLoader::loadMesh("Beast Mode", "./Assets/Models/BeastMode.obj");

	ObjectLoader::loadMesh("Building Top 1", "./Assets/Models/Building_Top1.obj");
	ObjectLoader::loadMesh("Building Top 2", "./Assets/Models/Building_Top2.obj");
	ObjectLoader::loadMesh("Building Top 3", "./Assets/Models/Building_Top3.obj");
	ObjectLoader::loadMesh("Building Top 4", "./Assets/Models/Building_Top4.obj");

	ObjectLoader::loadMesh("UIQuad", "./Assets/Models/UIQuad.obj");



	ObjectLoader::loadMesh("TestBoi", "./Assets/Models/Animation/Fat Boi - Animated_", 20);


	// Skeletal load test
	//ObjectLoader::loadSkeletalMesh("SkeletalBoi", "./Assets/Test Exporter/Test/Armature.nut", "./Assets/Test Exporter/Test/Anims/ArmatureAction.nutAnim");
	ObjectLoader::loadSkeletalMesh("SkeletalBoiTwo", "./Assets/Test Exporter/GOLDEN_FATBOI2/Armature.nut", "./Assets/Test Exporter/GOLDEN_FATBOI2/Anims/Run.nutAnim");
	//ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/BM_-_Run_2/Armature.nut", "./Assets/Test Exporter/BM_-_Run_2/Anims/ArmatureAction.nutAnim");
	ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/Fat_Boi_-_Review_2_/Armature.nut", "./Assets/Test Exporter/Fat_Boi_-_Review_2_/Anims/ArmatureAction.nutAnim");
	//ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/TEST_BM/Armature.nut", "./Assets/Test Exporter/TEST_BM/Anims/Action.nutAnim");



	ObjectLoader::loadTexture("Default", "./Assets/Textures/Default.png");

	ObjectLoader::loadTexture("Acorn", "./Assets/Textures/Acorn_Texture.png");
	ObjectLoader::loadTexture("Background", "./Assets/Textures/new_background.png");
	ObjectLoader::loadTexture("Background1", "./Assets/Textures/background1.png");
	ObjectLoader::loadTexture("Background2", "./Assets/Textures/background2.png");
	ObjectLoader::loadTexture("Background3", "./Assets/Textures/background3.png");
	ObjectLoader::loadTexture("Background4", "./Assets/Textures/background4.png");
	ObjectLoader::loadTexture("Building", "./Assets/Textures/Building Layout.png");
	ObjectLoader::loadTexture("Building 1 Texture 1", "./Assets/Textures/Building 1 Texture 1.png");
	ObjectLoader::loadTexture("Building 1 Texture 1 Background", "./Assets/Textures/Building 1 Texture 1 Background.png");
	ObjectLoader::loadTexture("Building 1 Texture 2", "./Assets/Textures/Building 1 Texture 2.png");
	ObjectLoader::loadTexture("Building 1 Texture 2 Background", "./Assets/Textures/Building 1 Texture 2 Background.png");
	ObjectLoader::loadTexture("Building 1 Texture 3", "./Assets/Textures/Building 1 Texture 3.png");
	ObjectLoader::loadTexture("Building 1 Texture 3 Background", "./Assets/Textures/Building 1 Texture 3 Background.png");
	ObjectLoader::loadTexture("Building 2 Texture 1", "./Assets/Textures/Building 2 Texture 1.png");
	ObjectLoader::loadTexture("Building 2 Texture 1 Background", "./Assets/Textures/Building 2 Texture 1 Background.png");
	ObjectLoader::loadTexture("Building 2 Texture 2", "./Assets/Textures/Building 2 Texture 2.png");
	ObjectLoader::loadTexture("Building 2 Texture 3", "./Assets/Textures/Building 2 Texture 3.png");
	ObjectLoader::loadTexture("Building 2 Texture 3 Background", "./Assets/Textures/Building 2 Texture 3 Background.png");
	ObjectLoader::loadTexture("Building 3 Texture 1", "./Assets/Textures/Building 3 Texture 1.png");
	ObjectLoader::loadTexture("Building 3 Texture 1 Background", "./Assets/Textures/Building 3 Texture 1 Background.png");
	ObjectLoader::loadTexture("Building 3 Texture 3", "./Assets/Textures/Building 3 Texture 3.png");
	ObjectLoader::loadTexture("Building 3 Texture 3 Background", "./Assets/Textures/Building 3 Texture 3 Background.png");
	ObjectLoader::loadTexture("Billboard", "./Assets/Textures/Billboard_Texture1.png");
	ObjectLoader::loadTexture("Love", "./Assets/Textures/Go Love.png");//valentines day
	ObjectLoader::loadTexture("Button", "./Assets/Textures/Button_Texture.png");
	ObjectLoader::loadTexture("Checkpoint Squirrel", "./Assets/Textures/squirrel checkpoint_tex.png");
	ObjectLoader::loadTexture("Chimney", "./Assets/Textures/Chimney Texture.png");
	ObjectLoader::loadTexture("Clothesline", "./Assets/Textures/clothingline_texture.png");
	ObjectLoader::loadTexture("Coin", "./Assets/Textures/Coin texture.png");
	ObjectLoader::loadTexture("Cone", "./Assets/Textures/Cone_Texture.png");
	ObjectLoader::loadTexture("Couch", "./Assets/Textures/couch texture.png");
	ObjectLoader::loadTexture("Dust", "./Assets/Textures/Dust_Trail.png");
	ObjectLoader::loadTexture("FatBoi", "./Assets/Textures/FatBoiTexture.png");
	ObjectLoader::loadTexture("Flag", "./Assets/Textures/flag tex.png");
	ObjectLoader::loadTexture("Garbage", "./Assets/Textures/garbage.png");
	ObjectLoader::loadTexture("Garbage 2", "./Assets/Textures/garbage texture.png");
	ObjectLoader::loadTexture("Lamp", "./Assets/Textures/lamp texture.png");
	ObjectLoader::loadTexture("Lever", "./Assets/Textures/Lever_Texture1.png");
	ObjectLoader::loadTexture("Planter", "./Assets/Textures/planter texture.png");
	ObjectLoader::loadTexture("Raccoon", "./Assets/Textures/raccoon_texture.png");
	ObjectLoader::loadTexture("Rooftop Door", "./Assets/Textures/roofdoor texture.png");
	ObjectLoader::loadTexture("Round Table", "./Assets/Textures/roundtable texture.png");
	ObjectLoader::loadTexture("Small Plant", "./Assets/Textures/small plant texture.png");
	ObjectLoader::loadTexture("Spikes", "./Assets/Textures/Spike Texture.png");
	ObjectLoader::loadTexture("Squirrel", "./Assets/Textures/squirrel_texture.png");
	ObjectLoader::loadTexture("Table", "./Assets/Textures/table texture.png");
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
	ObjectLoader::loadTexture("UiBackdrop", "./Assets/Textures/Fade_background.png");

	ObjectLoader::loadTexture("Water", "./Assets/Textures/water.png");
	ObjectLoader::loadTexture("Toon1", "./Assets/Textures/toon.png");
	ObjectLoader::loadTexture("Toon", "./Assets/Textures/toon1.png");
	ObjectLoader::loadTexture("Toon2", "./Assets/Textures/toon2.png");
	ObjectLoader::loadTexture("Toon3", "./Assets/Textures/toon3.png");
	ObjectLoader::loadTexture("Smooth", "./Assets/Textures/Smooth Shading.png");
	ObjectLoader::loadTexture("Normal", "./Assets/Textures/Acorn_Texture_n.png");
	ObjectLoader::loadTexture("Beast Mode", "./Assets/Textures/Beast_Mode_-_UV.png");
	ObjectLoader::loadTexture("Anim Test Tex", "./Assets/ANIM_TEST_TEX.png");

	//ui textures
	ObjectLoader::loadTexture("Menu Picture", "./Assets//Textures/Menu Picture.png");
	ObjectLoader::loadTexture("Vert black bar", "./Assets/Textures/Vertical black bar.png");
	ObjectLoader::loadTexture("start button", "./Assets/Textures/START button.png");
	ObjectLoader::loadTexture("level select button", "./Assets/Textures/LEVEL SELECT button.png");
	ObjectLoader::loadTexture("extras button", "./Assets/Textures/EXTRAS button.png");
	ObjectLoader::loadTexture("exit button", "./Assets/Textures/EXIT Button.png");

	//REGAN TEXTURES
	ObjectLoader::loadTexture("adambackground", "./Assets//Textures/adam back.png");
	ObjectLoader::loadTexture("adambackground2", "./Assets//Textures/adam back2.png");
	ObjectLoader::loadTexture("jump tut", "./Assets//Textures/space.png");
	ObjectLoader::loadTexture("switch tut", "./Assets//Textures/shift.png");

	//nutOmeter.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//nutOmeter.setMesh(ObjectLoader::getMesh("Plane"));
	//nutOmeter.setTexture(ObjectLoader::getTexture("FullNut"));
	//nutOmeter.addPhysicsBody(false);
	//nutOmeter.setWorldPosition(vec3(0.0f, -6.0f, 0.0f));
	//nutOmeter.setLocalRotationAngleZ(-3.14592f / 2.0f);
	////nutOmeter.setLocalRotationAngleY(3.14592f);
	//nutOmeter.setLocalScale(2.0f);

	//time.setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//time.setMesh(ObjectLoader::getMesh("Plane"));
	//time.setTexture(ObjectLoader::getTexture("Time"));
	//time.addPhysicsBody(false);
	//time.setWorldPosition(vec3(-13.5f, 7.5f, 0.0f));
	//time.setLocalRotationAngleZ(-3.14592f / 2.0f);
	////time.setLocalRotationAngleY(3.14592f);
	//time.setLocalScale(2.0f);

	//particleTrail = new ParticleEmitter;
	//particleTrail->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//particleTrail->setMesh(ObjectLoader::getMesh("Plane"));
	//particleTrail->setTexture(ObjectLoader::getTexture("Default"));
	//particleTrail->addPhysicsBody(false);
	//particleTrail->setWorldPosition(vec3(9.0f, 2.0f, -5.0f));
	//particleTrail->setLocalScale(vec3(1, 1, 1));
	//dynamic_cast<ParticleEmitter*>(particleTrail)->texName = "Dust";
	//dynamic_cast<ParticleEmitter*>(particleTrail)->velocity0 = vec3(-0.1f, -0.01f, -0.01f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->velocity1 = vec3(-0.1f, -0.01f, 0.01f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->massRange = vec2(1.0f, 2.0f);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->emitterPosition = player.getWorldPosition();

	//// Visual Properties
	//dynamic_cast<ParticleEmitter*>(particleTrail)->lifeRange = vec2(0.8f, 1.0f);
	//

	//dynamic_cast<ParticleEmitter*>(particleTrail)->initialize(50);
	//dynamic_cast<ParticleEmitter*>(particleTrail)->playing = false;

	//jumpParticles = new ParticleEmitter;
	//jumpParticles->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
	//jumpParticles->setMesh(ObjectLoader::getMesh("Plane"));
	//jumpParticles->setTexture(ObjectLoader::getTexture("Dust"));
	//jumpParticles->addPhysicsBody(false);
	//jumpParticles->setParent(&player);
	//jumpParticles->setLocalPosition(vec3::Zero);
	//jumpParticles->setLocalScale(vec3(0.5f));
	//jumpParticles->setLocalRotationAngleY(90.0f);

	// Physics properties
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->texName = "Dust";
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity0 = vec3(-0.1f, 0.01f, -0.01f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->velocity1 = vec3(0.1f, 0.02f, 0.01f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->massRange = vec2(1.0f, 2.0f);
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->emitterPosition = player.getWorldPosition();

	// Visual Properties
	//dynamic_cast<ParticleEmitter*>(jumpParticles)->lifeRange = vec2(0.08f, 0.1f);

	//dynamic_cast<ParticleEmitter*>(jumpParticles)->initialize(5);


	/*float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);*/
	//camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	////camera.orthographic(-10, 10, -10, 10, -100, 100);
	//camera.setWorldPosition(vec3(0.0f, 4.0f, 5.0f));
	//camera.setLocalRotationAngleX(camera.getLocalRotationAngleX() - 0.2f);

	//UICamera.orthographic(-16, 16, 9, -9, -100, 100);
	//UICamera.setWorldPosition(vec3::Zero);


	// Load all UI animation files
	UIAnimation::loadAllAnimsInFile();


	Scene* testScene = new Scene("TEST SCENE");
	//testScene->saveScene();

	sceneManager = SceneManager::getInstance();

	//sceneManager->loadScenesFromFile("./Assets/Scenes/Scenes.db");
	//sceneManager->saveScene();
	//sceneManager->loadSceneFromFile("./Assets/Scenes/Scenes2.db", "Scene2");

	sceneManager->addScene(testScene);
	sceneManager->loadOldFaithful(testScene->getName());
	//sceneManager->loadMainMenu(testScene->getName());
	_currentScene = sceneManager->getCurrentScene();


	//SceneManager* sceneManager = SceneManager::getInstance();
	//sceneManager->loadScenesFromFile("./Assets/Scenes/Scenes.db");
	//sceneManager->saveScene();

	//sceneManager->loadSceneFromFile("./Assets/Scenes/Scenes2.db", "Scene2");
	//sceneManager->loadSceneFromFile("./Assets/Scenes/$$.db", "$$");
	sceneManager->loadSceneFromFile("./Assets/Scenes/sceney.db", "sceney", true);

	sceneManager->loadSceneFromFile("./Assets/Scenes/UITest.db", "UITest", false);

	//sceneManager->loadSceneFromFile("./Assets/Scenes/Level Fun.db", "Level ");
	//REGAN LEVEL
	sceneManager->loadSceneFromFile("./Assets/Scenes/tut.db", "tut", true);

	//sceneManager->loadSceneFromFile("./Assets/Scenes/Level Fun.db", "Level ");



	//SkeletalMesh testSkeleton;
	////testSkeleton.loadFromFile("./Assets/FatBoi.dae");
	//testSkeleton.loadFromFile("./Assets/ANIM_TEST.dae");
	//testSkeleton._isSkeletal = true;

	_sound = SoundComponent::getInstance();
	//start to play the sound and save it to a channel so it can be refferenced later

	_sound->loadSound("bgSound", "SpeedRunners_Soundtrack_Level_Music_1.mp3", false);
	_sound->loadSound("jumpGrunt", "jump grunt.wav", false);
	_sound->loadSound("landingGrunt", "landing grunt.wav", false);
	_sound->loadSound("shift", "shift.wav", false);
	_sound->loadSound("acorn", "acorn collect.wav", false);
	_sound->loadSound("coin", "coin collect.wav", false);

	_sound->playSound("bgSound", _sound->getBGChannel(), true, 0.05f);

}

void Game::update()
{
	Sound::engine.Update();
	_currentScene = SceneManager::getInstance()->getCurrentScene();

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	drawTime += deltaTime;

	_currentScene->update(deltaTime);

	//dynamic_cast<ParticleEmitter*>(particleTrail)->emitterPosition = player.getWorldPosition();


	//particleTrail->getParent()->update(deltaTime);
	//player.getParent()->update(deltaTime);
	//spotLight->setPosition(vec3(player.getWorldPosition().x, 5.0f, player.getWorldPosition().z) + offse);
	//nutOmeter.getParent()->update(deltaTime);
	//time.getParent()->update(deltaTime);

	counter += deltaTime;

	//camera.setWorldPosition(MathLibCore::lerp(camera.getWorldPosition(), playerTransform->getWorldPosition() - offset, deltaTime * 3.0f));
	//camera.update(deltaTime);
	//UICamera.update(deltaTime);
}

void Game::draw()
{
	// Completely clear the Back-Buffer before doing any work.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CameraComponent* camera = EntityManager::getInstance()->getComponent<CameraComponent*>(ComponentType::Camera, EntityManager::getInstance()->getMainCamera());
	TransformComponent* cameraTrans = EntityManager::getInstance()->getComponent<TransformComponent*>(ComponentType::Transform, EntityManager::getInstance()->getMainCamera());

	gbuffer.clear();
	gbuffer.setViewport();
	gbuffer.bind();

	_currentScene->drawUI();
	_currentScene->draw();

	gbuffer.unbind();

	frameBufferShadow.clear();
	frameBufferShadow.setViewport();
	frameBufferShadow.bind();
	_currentScene->drawShadow();
	frameBufferShadow.unbind();

	mat4 biasMat4 = mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	CameraComponent* shadowCamera = EntityManager::getInstance()->getComponent<CameraComponent*>(ComponentType::Camera, EntityManager::getInstance()->getShadowCamera());

	TransformComponent* shadowcameraTrans = EntityManager::getInstance()->getComponent<TransformComponent*>(ComponentType::Transform, EntityManager::getInstance()->getShadowCamera());

	mat4 ViewToShadowClip = biasMat4 * shadowCamera->getProjection() * shadowcameraTrans->getView() * cameraTrans->getView().getInverse();
	
	//shaderGbuffer.bind();
	//
	//gbuffer.bindDepthAsTexture(0);
	//gbuffer.bindColorAsTexture(0, 1);
	//gbuffer.bindColorAsTexture(1, 2);
	//gbuffer.bindResolution();
	//frameBufferOutline.clear();
	//frameBufferOutline.bind();
	////glClear(GL_DEPTH_BUFFER_BIT);
	//////_meshRendererSystem->draw(light, spotLight);
	////_currentScene->draw();
	//
	//gbuffer.renderToFSQ();
	////gbuffer.drawFSQ();
	//frameBufferOutline.unbind();
	//
	//gbuffer.unbindTexture(2);
	//gbuffer.unbindTexture(1);
	//gbuffer.unbindTexture(0);
	//shaderGbuffer.unBind();
	//
	//
	//
	//
	//shaderOutline.bind();
	//shaderOutline.sendUniform("outline", outline);
	//frameBufferOutline.bindColorAsTexture(0, 0);
	//glViewport(0, 0, 1900, 1000);
	//
	//frameBufferLUT.clear();
	//frameBufferLUT.bind();
	//
	////frameBufferOutline.renderToFSQ();
	//frameBufferOutline.drawFSQ();
	//frameBufferLUT.unbind();
	//
	//frameBufferOutline.unbindTexture(0);//texture
	//
	//shaderOutline.unBind();
	mat4 uProjInverse = camera->getProjection().getSlowInverse();

	mat4 uViewInverse = cameraTrans->getView().getInverse();
	TransformComponent* playerTrans = _currentScene->getPlayTrans();

	shaderOutline.bind();
	shaderOutline.sendUniformMat4("uViewToShadow", ViewToShadowClip.data, false);
	shaderOutline.sendUniform("outline", outline);
	shaderOutline.sendUniformMat4("uProjInverse", uProjInverse.data, false);
	shaderOutline.sendUniformMat4("uViewInverse", uViewInverse.data, false);
	shaderOutline.sendUniform("POS", playerTrans->getLocalPosition());

	gbuffer.bindColorAsTexture(0, 0);
	gbuffer.bindColorAsTexture(1, 1);
	gbuffer.bindDepthAsTexture(2);

	toonRamp->bind(5);
	frameBufferShadow.bindDepthAsTexture(16);

	gbuffer.bindResolution();
	glViewport(0, 0, 1900, 1000);

	frameBufferLUT.clear();
	frameBufferLUT.bind();

	//frameBufferOutline.renderToFSQ();
	gbuffer.drawFSQ();
	frameBufferLUT.unbind();

	//frameBufferUI.unbind();
	toonRamp->unBind();

	gbuffer.unbindTexture(2);//texture
	gbuffer.unbindTexture(1);//texture
	gbuffer.unbindTexture(0);//texture

	shaderOutline.unBind();



	//shaderGbuffer.bind();
	//gbuffer.bindDepthAsTexture(0);
	//frameBufferLUT.bindColorAsTexture(0, 1);
	//gbuffer.bindColorAsTexture(1, 2);
	//glViewport(0, 0, 1900, 1000);
	//
	//frameBufferOutline.clear();
	//frameBufferOutline.bind();
	//
	////frameBufferLUT.renderToFSQ();
	//gbuffer.drawFSQ();
	//
	//frameBufferOutline.unbind();
	//gbuffer.unbindTexture(2);
	//frameBufferLUT.unbindTexture(1);
	//gbuffer.unbindTexture(0);
	//
	//shaderGbuffer.unBind();


	//_currentScene->drawUI();
	

	
	
	shaderLUT.bind();

	shaderLUT.sendUniform("lut", lut);
	LUTTex->bind(30);
	

	frameBufferLUT.bindColorAsTexture(0, 0);
	glViewport(0, 0, 1900, 1000);
	Framebuffer::drawFSQ();
	
	frameBufferLUT.unbindTexture(0);
	shaderLUT.unBind();

	//glDisable(GL_DEPTH_TEST);
	//nutOmeter.draw(UICamera, light, spotLight, uiCameraInverse);
	//time.draw(UICamera, light, spotLight, uiCameraInverse);

	// Draw ImGui stuff
	

	_currentScene->drawText();
	_currentScene->imguiDraw();
	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
	drawTime = 0.0f;
}


void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	_currentScene->keyboardDown(key, mouseX, mouseY);
	if (key == 'o')
		outline = !outline;
	if (key == 'r')
		shaderOutline.reload();
	if (key == 'v')
		lut = !lut;
	//if (key == '1')
		//sceneManager->loadScene("tut");
	//if (key == '1')
	//	sceneManager->loadScene("");
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	_currentScene->keyboardUp(key, mouseX, mouseY);
}

void Game::specialKeyDown(int key, int mouseX, int mouseY)
{
	_currentScene->specialKeyDown(key, mouseX, mouseY);
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	_currentScene->mouseClicked(button, state, x, y);
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
	_currentScene->mouseMoved(x, y);
}

void Game::mouseWheel(int wheel, int direction, int x, int y)
{
	_currentScene->mouseWheel(wheel, direction, x, y);
}
