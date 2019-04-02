#include "Game.h"
#include "imgui/imgui.h"
#include "UIImage.h"

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
	srand(0);
	// OpenGL will not draw triangles hidden behind other geometry
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	Framebuffer::initFrameBuffers();
	gbuffer.init(windowWidth, windowHeight);
	//frameBufferOutline.addDepthTarget();
	frameBufferOutline.addColorTarget(GL_RGB8);
	frameBufferOutline.init(windowWidth, windowHeight);

	frameBufferLUT.addColorTarget(GL_RGB8);
	frameBufferLUT.init(windowWidth, windowHeight);

	frameBufferUI.addColorTarget(GL_RGB8);
	frameBufferUI.init(windowWidth, windowHeight);

	frameBufferShadow.addDepthTarget();
	frameBufferShadow.init(8192, 8192);


	lights.reserve(1000);
	for (int i = 0; i < 1000; ++i)
	{
		lights.push_back(new Light());
		lights[i]->init();
		lights[i]->color = vec4(1.f,1.f,0,1.f);//vec4(1.f, 0.8f, 0.3f, 1.0f);
		lights[i]->constantAtten = 1.0f;
		lights[i]->linearAtten = 0.7f;
		lights[i]->quadAtten = 1.8f;

	}

	// Load shaders and mesh
	ObjectLoader::loadShaderProgram("Normal", "./Assets/Shaders/shader.vert", "./Assets/Shaders/gBuffer.frag");
	ObjectLoader::loadShaderProgram("Player", "./Assets/Shaders/Morph.vert", "./Assets/Shaders/PassThrough.frag");
	ObjectLoader::loadShaderProgram("Water", "./Assets/Shaders/waterShader.vert", "./Assets/Shaders/waterShader.frag");
	ObjectLoader::loadShaderProgram("BBox", "./Assets/Shaders/BBox.vert", "./Assets/Shaders/BBox.frag");
	ObjectLoader::loadShaderProgram("SkeletalAnim", "./Assets/Shaders/SkeletalAnim.vert", "./Assets/Shaders/gBuffer.frag");
	ObjectLoader::loadShaderProgram("UIShader", "./Assets/Shaders/shader.vert", "./Assets/Shaders/UI.frag");
	ObjectLoader::loadShaderProgram("FreeType", "./Assets/Shaders/font.vert", "./Assets/Shaders/font.frag");
	ObjectLoader::loadShaderProgram("Discard", "./Assets/Shaders/discard.vert", "./Assets/Shaders/discard.frag");


	shaderGbuffer.load("./Assets/Shaders/shader.vert", "./Assets/Shaders/PassThrough - Copy.frag");
	shaderOutline.load("./Assets/Shaders/Post.vert", "./Assets/Shaders/Post.frag");
	shaderLUT.load("./Assets/Shaders/Post.vert", "./Assets/Shaders/LUT.frag");

	shaderDeferred.load("./Assets/Shaders/shader.vert", "./Assets/Shaders/deferred.frag");

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
	ObjectLoader::loadMesh("Radio", "./Assets/Models/Radio.obj");

	ObjectLoader::loadMesh("Rooftop Door", "./Assets/Models/rooftopdoor.obj");

	ObjectLoader::loadMesh("Beast Mode", "./Assets/Models/BeastMode.obj");

	ObjectLoader::loadMesh("Building Top 1", "./Assets/Models/Building_Top1.obj");
	ObjectLoader::loadMesh("Building Top 2", "./Assets/Models/Building_Top2.obj");
	ObjectLoader::loadMesh("Building Top 3", "./Assets/Models/Building_Top3.obj");
	ObjectLoader::loadMesh("Building Top 4", "./Assets/Models/Building_Top4.obj");

	ObjectLoader::loadMesh("UIQuad", "./Assets/Models/UIQuad.obj");

	//Road stuff things
	ObjectLoader::loadMesh("Awning", "./Assets/Models/Awning.obj");
	ObjectLoader::loadMesh("Bench", "./Assets/Models/Bench.obj");
	ObjectLoader::loadMesh("Fire Hydrant", "./Assets/Models/Fire Hydrant.obj");
	ObjectLoader::loadMesh("Road", "./Assets/Models/Road.obj");
	ObjectLoader::loadMesh("Stop Sign", "./Assets/Models/Stop Sign.obj");
	ObjectLoader::loadMesh("Street Light", "./Assets/Models/Street Light.obj");
	ObjectLoader::loadMesh("Traffic Obstacle 1", "./Assets/Models/Traffic Obstacle 1.obj");
	ObjectLoader::loadMesh("Traffic Obstacle 2", "./Assets/Models/Traffic Obstacle 2.obj");

	ObjectLoader::loadMesh("TestBoi", "./Assets/Models/Animation/Fat Boi - Animated_", 20);


	// Skeletal load test
	//ObjectLoader::loadSkeletalMesh("SkeletalBoi", "./Assets/Test Exporter/Test/Armature.nut", "./Assets/Test Exporter/Test/Anims/ArmatureAction.nutAnim");
	ObjectLoader::loadSkeletalMesh("SkeletalBoiTwo", "./Assets/Test Exporter/GOLDEN_FATBOI2/Armature.nut", "./Assets/Test Exporter/GOLDEN_FATBOI2/Anims/Run.nutAnim");
	//ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/BM_-_Run_2/Armature.nut", "./Assets/Test Exporter/BM_-_Run_2/Anims/ArmatureAction.nutAnim");
	//ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/Fat_Boi_-_Review_2_/Armature.nut", "./Assets/Test Exporter/Fat_Boi_-_Review_2_/Anims/ArmatureAction.nutAnim");
	ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/TEST_BM/Armature.nut", "./Assets/Test Exporter/TEST_BM/Anims/Action.nutAnim");
	//ObjectLoader::loadSkeletalMesh("SkeletalBeast", "./Assets/Test Exporter/TEST_BB/Armature.nut", "./Assets/Test Exporter/TEST_BB/Anims/Action.nutAnim");



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
	ObjectLoader::loadTexture("Radio", "./Assets/Textures/Radio.png");

	//Some Road Textures
	ObjectLoader::loadTexture("Awning", "./Assets/Textures/Awning.png");
	ObjectLoader::loadTexture("Bench", "./Assets/Textures/Bench.png");
	ObjectLoader::loadTexture("Fire Hydrant", "./Assets/Textures/Fire Hydrant.png");
	ObjectLoader::loadTexture("Road", "./Assets/Textures/Road.png");
	ObjectLoader::loadTexture("Stop Sign", "./Assets/Textures/Stop Sign.png");
	ObjectLoader::loadTexture("Street Light", "./Assets/Textures/Street Light.png");
	ObjectLoader::loadTexture("Traffic Obstacle", "./Assets/Textures/Traffic Obstacle.png");

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
	ObjectLoader::loadTexture("Back", "./Assets/Textures/back.png");
	ObjectLoader::loadTexture("Dialogue", "./Assets/Textures/Talking.png");
	ObjectLoader::loadTexture("Space Press", "./Assets/Textures/space press.png");

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
	ObjectLoader::loadTexture("Scoreboard", "./Assets//Textures/Highscore.png");
	ObjectLoader::loadTexture("Vert black bar", "./Assets/Textures/Vertical black bar.png");
	ObjectLoader::loadTexture("start button", "./Assets/Textures/START button.png");
	ObjectLoader::loadTexture("level select button", "./Assets/Textures/LEVEL SELECT button.png");
	ObjectLoader::loadTexture("extras button", "./Assets/Textures/EXTRAS button.png");
	ObjectLoader::loadTexture("exit button", "./Assets/Textures/EXIT Button.png");
	ObjectLoader::loadTexture("UI Nut", "./Assets/Textures/nut.png");
	ObjectLoader::loadTexture("UIcoin", "./Assets/Textures/UIcoin.png");
	ObjectLoader::loadTexture("Title Screen", "./Assets/Textures/title screen.png");
	ObjectLoader::loadTexture("Press Any Button", "./Assets/Textures/press any button.png");
	ObjectLoader::loadTexture("Exit Button Hover", "./Assets/Textures/EXIT Button Hover.png");
	ObjectLoader::loadTexture("EXTRAS button Hover", "./Assets/Textures/EXTRAS button Hover.png");
	ObjectLoader::loadTexture("LEVEL SELECT button Hover", "./Assets/Textures/LEVEL SELECT button Hover.png");
	ObjectLoader::loadTexture("START button Hover", "./Assets/Textures/START button Hover.png");
	ObjectLoader::loadTexture("Horizontal black bar", "./Assets/Textures/Horizontal black bar.png");
	ObjectLoader::loadTexture("Bam", "./Assets/Textures/Bam.png");
	ObjectLoader::loadTexture("Bang", "./Assets/Textures/Bang.png");
	ObjectLoader::loadTexture("Click 1", "./Assets/Textures/click 1.png");
	ObjectLoader::loadTexture("Click 2", "./Assets/Textures/click 2.png");
	ObjectLoader::loadTexture("Crash", "./Assets/Textures/Crash.png");
	ObjectLoader::loadTexture("Credits Button Hover", "./Assets/Textures/Credits Button hover.png");
	ObjectLoader::loadTexture("Credits Button", "./Assets/Textures/Credits Button.png");
	ObjectLoader::loadTexture("Leaderboard Button hover", "./Assets/Textures/Leaderboard Button hover.png");
	ObjectLoader::loadTexture("Leaderboard Button", "./Assets/Textures/Leaderboard Button.png");
	ObjectLoader::loadTexture("Play Button", "./Assets/Textures/PLAY button.png");
	ObjectLoader::loadTexture("Play Button Hover", "./Assets/Textures/PLAY button hover.png");
	ObjectLoader::loadTexture("Ring", "./Assets/Textures/Ring.png");
	ObjectLoader::loadTexture("Roar", "./Assets/Textures/Roar.png");
	ObjectLoader::loadTexture("Woosh", "./Assets/Textures/Woosh.png");
	ObjectLoader::loadTexture("Tutorial button hover", "./Assets/Textures/TUTORIAL button hover.png");
	ObjectLoader::loadTexture("Tutorial button", "./Assets/Textures/TUTORIAL button.png");
	ObjectLoader::loadTexture("Next", "./Assets/Textures/Next.png");
	ObjectLoader::loadTexture("Next Level Button", "./Assets/Textures/Next level button.png");
	ObjectLoader::loadTexture("Next Level Button Hover", "./Assets/Textures/Next level button hover.png");
	ObjectLoader::loadTexture("Return Button Hover", "./Assets/Textures/Return button hover.png");
	ObjectLoader::loadTexture("Return Button", "./Assets/Textures/Return button.png");
	ObjectLoader::loadTexture("Replay Button", "./Assets/Textures/REPLAY button.png");
	ObjectLoader::loadTexture("Replay Button Hover", "./Assets/Textures/REPLAY button hover.png");
	ObjectLoader::loadTexture("Back Hover", "./Assets/Textures/Back Hover.png");
	ObjectLoader::loadTexture("Next Hover", "./Assets/Textures/Next Hover.png");
	ObjectLoader::loadTexture("Right Arrow Hover", "./Assets/Textures/Right Arrow Hover.png");
	ObjectLoader::loadTexture("Right Arrow", "./Assets/Textures/Right Arrow.png");
	ObjectLoader::loadTexture("Left Arrow", "./Assets/Textures/Left Arrow.png");
	ObjectLoader::loadTexture("Left Arrow Hover", "./Assets/Textures/Left Arrow Hover.png");
	ObjectLoader::loadTexture("Coin Don't Have", "./Assets/Textures/coin don't have.png");
	ObjectLoader::loadTexture("level select background", "./Assets/Textures/level select background.png");
	ObjectLoader::loadTexture("level selection highlight", "./Assets/Textures/level selection highlight.png");

	//REGAN TEXTURES
	ObjectLoader::loadTexture("adambackground", "./Assets//Textures/adam back3.png");
	ObjectLoader::loadTexture("adambackground2", "./Assets//Textures/adam back2.png");
	ObjectLoader::loadTexture("jump tut", "./Assets//Textures/space.png");
	ObjectLoader::loadTexture("switch tut", "./Assets//Textures/shift.png");


	// Load all UI animation files
	UIAnimation::loadAllAnimsInFile();


	Scene* testScene = new Scene("TEST SCENE", true);
	//testScene->saveScene();

	sceneManager = SceneManager::getInstance();

	
	sceneManager->addScene(testScene);
	sceneManager->loadOldFaithful(testScene->getName());
	_currentScene = sceneManager->getCurrentScene();


	sceneManager->loadSceneFromFile("./Assets/Scenes/sceney.db", "sceney", true);

	sceneManager->loadSceneFromFile("./Assets/Scenes/UITest.db", "UITest", false);

	sceneManager->loadSceneFromFile("./Assets/Scenes/Scoreboard.db", "Scoreboard", true, 1);

	sceneManager->loadSceneFromFile("./Assets/Scenes/GROUND.db", "Ground", false);

	//sceneManager->loadSceneFromFile("./Assets/Scenes/Level Fun.db", "Level ");
	//REGAN LEVEL

	sceneManager->loadSceneFromFile("./Assets/Scenes/tut.db", "tut", true);

	sceneManager->loadScene("UITest");
	_currentScene = sceneManager->getCurrentScene();


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
	if (_currentScene->getName() == "UITest")
	{
		outline = false;
		UICanvas* canvas = _currentScene->getUISystem()->getCanvas("Canvui");
		UIImage* image = canvas->getImage("Start");
		if (image->clicked())
		{
			image->setClicked(false);
			sceneManager->loadScene("tut");
			_currentScene = sceneManager->getCurrentScene();
			outline = true;
		}
		else if (canvas->getImage("Exit")->clicked())
		{
			exit(0);
		}
		else if (canvas->getImage("Extras")->clicked())
		{
			
		}
		else if (canvas->getImage("LevelSelect")->clicked())
		{
			
		}
	}

	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	drawTime += deltaTime;

	_currentScene->update(deltaTime);

	counter += deltaTime;

	
	for (int i = 0; i < (int)lights.size(); ++i)
	{
		lights[i]->setLocalPosition(vec3(
			sin(i + TotalGameTime) * 0.1f + ((i /64)),
			sin(TotalGameTime) * 0.1f + (i / 64),
			cos(i + TotalGameTime) * 0.1f + (i % 4)) * vec3(60, 40, -20) - vec3(30, 25, 0));
		//lights[i]->setLocalPosition(vec3(0, 5, -5));
	}
	lights[lights.size() - 1]->setLocalPosition(vec3(0, 5, -5)); 

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

	//_currentScene->drawUI();
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
	glViewport(0, 0, windowWidth, windowHeight);

	frameBufferLUT.clear();

	//frameBufferOutline.renderToFSQ();
	frameBufferLUT.renderToFSQ();
	frameBufferLUT.unbind();

	shaderDeferred.bind();
	shaderDeferred.sendUniformMat4("uProjInverse", uProjInverse.data, false);
	frameBufferLUT.bindColorAsTexture(0, 0);
	if (deferred)
	{
		TransformComponent transform;
		for (int i = 0; i < (int)lights.size(); ++i)
		{
			lights[i]->bind();
			lights[i]->position = cameraTrans->getView() * vec4(lights[i]->getLocalPosition(), 1.0f);
			lights[i]->update(0.0f);

			
			transform.setLocalPosition(lights[i]->getLocalPosition());
			transform.setLocalScale(lights[i]->radius);
			transform.update(updateTimer->getElapsedTimeSeconds());
			shaderDeferred.sendUniformMat4("uModel", transform.getLocalToWorldMatrix().data, false);
			shaderDeferred.sendUniformMat4("uView", cameraTrans->getView().data, false);
			shaderDeferred.sendUniformMat4("uProj", camera->getProjection().data, false);
			shaderDeferred.sendUniform("uLightColor", lights[i]->color);
			shaderDeferred.sendUniform("uLightPosition", lights[i]->position);
			shaderDeferred.sendUniform("uLightDirection", lights[i]->direction);
			shaderDeferred.sendUniform("uLightAttenuation", vec4(lights[i]->constantAtten, lights[i]->linearAtten, lights[i]->quadAtten, lights[i]->radius));

			frameBufferLUT.renderSphere();
		}
	}
	
	//gbuffer.drawFSQ();


	//frameBufferUI.unbind();
	toonRamp->unBind();

	gbuffer.unbindTexture(2);//texture
	gbuffer.unbindTexture(1);//texture
	gbuffer.unbindTexture(0);//texture

	shaderOutline.unBind();

	_currentScene->drawUI();

	shaderLUT.bind();

	shaderLUT.sendUniform("lut", lut);
	shaderLUT.sendUniform("totalGameTime", TotalGameTime);
	shaderLUT.sendUniform("screenShake", EntityManager::getInstance()->getComponent<Collider*>(ComponentType::Collider, playerTrans->getEntity())->screenShake);
	LUTTex->bind(30);
	

	frameBufferLUT.bindColorAsTexture(0, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	Framebuffer::drawFSQ();
	
	frameBufferLUT.unbindTexture(0);
	shaderLUT.unBind();	

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
	{
		shaderOutline.reload();
		shaderDeferred.reload();
	}
	if (key == 'v')
		lut = !lut;

	//if (key == '1')
	//	sceneManager->loadScene("tut");
	if (key == 'd')
		deferred = !deferred;
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

void Game::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	CameraComponent* camera = EntityManager::getInstance()->getComponent<CameraComponent*>(ComponentType::Camera, EntityManager::getInstance()->getMainCamera());

	camera->setPerspective(60.0f, aspect, 1.0f, 1000.0f);
	glViewport(0, 0, w, h);
	
	frameBufferOutline.reshape(w, h);
	frameBufferLUT.reshape(w, h);
	gbuffer.reshape(w, h);
	frameBufferUI.reshape(w, h);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = static_cast<float>(windowWidth);
	io.DisplaySize.y = static_cast<float>(windowHeight);
}
