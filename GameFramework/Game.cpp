//
// Created by erick on 20/03/17.
//

#include "Game.h"

Game::Game(GLuint width, GLuint height)
		: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	
}

Game::~Game()
{
	
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("/home/erick/CLionProjects/AirWar++/shaders/sprite.vs", "/home/erick/CLionProjects/AirWar++/shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
	                                  static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Set render-specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	
	// Load textures
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/ship.png", GL_TRUE, "ship");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/block.png", GL_FALSE, "background");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/hit.png", GL_TRUE, "hit");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/deathHit.png", GL_TRUE, "death");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/jet.png", GL_TRUE, "jet");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/bomber.png", GL_TRUE, "bomber");
	
	// Initialization of the game manager
	gameManager = new GameManager(this->Width, this->Height);
	gameManager->LoadEnemies("/home/erick/CLionProjects/AirWar++/levels/one.lvl");
	
}

void Game::Update(GLfloat dt)
{
	GameObject* iterator;
	for (int i = 0; i < gameManager->getPlayerShots().sizeOf(); ++i) {
		iterator = gameManager->getPlayerShots().get(i);
		iterator->Position += iterator->Velocity * dt;
	}
	
	gameManager->ControlEnemies(dt);
	gameManager->CheckCollision();

}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		// Move player
		if (this->Keys[GLFW_KEY_A])
		{
			GameObject* Player = gameManager->getPlayer();
			GLfloat velocity = 500.0f * dt;
			
			if (Player->Position.x >= 0)
				Player->Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			GameObject* Player = gameManager->getPlayer();
			GLfloat velocity = 500.0f * dt;
			
			if (Player->Position.x <= this->Width - Player->Size.x)
				Player->Position.x += velocity;
		}
		if (this->Keys[GLFW_KEY_S])
		{
			GameObject* Player = gameManager->getPlayer();
			GLfloat velocity = 500.0f * dt;
			
			if (Player->Position.y <= this->Height - Player->Size.y)
				Player->Position.y += velocity;
		}
		if (this->Keys[GLFW_KEY_W])
		{
			GameObject* Player = gameManager->getPlayer();
			GLfloat velocity = 500.0f * dt;
			
			if (Player->Position.y >= 0)
				Player->Position.y -= velocity;
		}
		if (this->Keys[GLFW_KEY_SPACE])
		{
			this->gameManager->PressTheTrigger(dt);
		}
	}
}

void Game::Render()
{
	if(this->State == GAME_ACTIVE)
	{
		// Draw background
		Texture2D backGround;
		backGround = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(backGround, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		// Draw game elements
		gameManager->Draw(*Renderer);
		
	}
}



