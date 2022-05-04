#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Shader adsShader();
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\Zuccarello.obj");
	mesh3.loadModel("..\\res\\bookshelf.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	reflectShader.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	adsShader.init("..\\res\\adsLighting.vert", "..\\res\\adsLighting.frag");

	geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		//collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkGeo()
{
	float randX = ((float)rand() / (RAND_MAX));
	float randY = ((float)rand() / (RAND_MAX));
	float randZ = ((float)rand() / (RAND_MAX));
	// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	geoShader.setFloat("randColourX", randX);
	geoShader.setFloat("randColourY", randY);
	geoShader.setFloat("randColourZ", randZ);
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
}

void MainGame::linkRimLighting()
{
	glm::vec3 camDir;
	camDir = mesh1.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkReflect()
{
	reflectShader.setMat4("model", transform.GetModel());
	reflectShader.setMat4("view", myCamera.getView());
	reflectShader.setMat4("projection", myCamera.getProjection());
	reflectShader.setInt("skybox", 0);
	reflectShader.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::linkADS()
{	
	adsShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	adsShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	adsShader.setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
	adsShader.setVec3("viewPos", myCamera.getPos());
	adsShader.setMat4("projection", myCamera.getProjection());
	adsShader.setMat4("view", myCamera.getView());
	adsShader.setMat4("model", transform.GetModel());
}



void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour
	
	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture

	// Mesh 1 Reflector
	transform.SetPos(glm::vec3(-2, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.3, 0.3, 0.3));

	reflectShader.Bind();
	linkReflect();
	texture.Bind(0);
	reflectShader.Update(transform, myCamera);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);

	// Mesh 2 Exploder
	transform.SetPos(glm::vec3(0.0, -1.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	geoShader.Bind();
	linkGeo();
	texture.Bind(0);
	geoShader.Update(transform, myCamera);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);

	// Tester Mesh
	transform.SetPos(glm::vec3(2.0, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.3, 0.3, 0.3));

	adsShader.Bind();
	linkADS();
	texture1.Bind(0);
	adsShader.Update(transform, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.62f);


	//// Mesh 3
	//transform.SetPos(glm::vec3(2.0, -sinf(counter), 0.0));
	//transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	//transform.SetScale(glm::vec3(0.3, 0.3, 0.3));

	//rimShader.Bind();
	//linkRimLighting();
	//texture.Bind(0);
	//rimShader.Update(transform, myCamera);
	//mesh2.draw();
	//mesh2.updateSphereData(*transform.GetPos(), 0.62f);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);	
	

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();


	_gameDisplay.swapBuffer();	
	

} 