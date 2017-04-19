//
// Created by erick on 20/03/17.
//

#include "Game.h"
GLfloat dtA = 0.0;
GLfloat dspawn = 0.0;

TextRenderer* textRenderer;

Game::Game(GLuint width, GLuint height)
		: State(GAME_ACTIVE), Width(width), Height(height)
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
	// Bkgrnd
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/background.jpeg", GL_FALSE, "background");
	
	// Ammo
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/missile.png", GL_TRUE, "missile");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/laser.png", GL_TRUE, "laser");
	
	// Explosions animations
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/hit.png", GL_TRUE, "hit");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/deathHit.png", GL_TRUE, "death");
	
	// Ships
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/jet.png", GL_TRUE, "jet");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/bomber.png", GL_TRUE, "bomber");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/tower.png", GL_TRUE, "tower");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/missileTower.png", GL_TRUE, "missileTower");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/kamikaze.png", GL_TRUE, "kamikaze");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/tank.png", GL_TRUE, "tank");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/ally.png", GL_TRUE, "ally");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/boss.png", GL_TRUE, "boss");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/ship.png", GL_TRUE, "ship");
	
	// PowerUps
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/shield.png", GL_TRUE, "shield");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/missilePU.png", GL_TRUE, "missilePU");
	ResourceManager::LoadTexture("/home/erick/CLionProjects/AirWar++/textures/laserPU.png", GL_TRUE, "laserPU");
	
	// Initialization of the game manager
	gameManager = new GameManager(this->Width, this->Height);
	gameManager->generateEnemies();
	gameManager->spawnEnemy();
	
	textRenderer = new TextRenderer(this->Width, this->Height);
	textRenderer->Load("/home/erick/CLionProjects/AirWar++/fonts/OCRAEXT.TTF", 24);
	this->State = GAME_PAUSED;

	
}

void Game::Update(GLfloat dt)
{
	
	switch (this->State) {
		
		case GAME_ACTIVE: {
			
			gameManager->updateShots(dt);
			
			for (int i = 0; i < gameManager->getEnemies().sizeOf(); ++i) {
				gameManager->ControlEnemies(dt, i);
			}
			if ((1 >= (int)( dt - dspawn)% 5 >= -1) &&
			    (gameManager->getEnemies().sizeOf() < 5) &&
			    (gameManager->getEnemySpawn().sizeOf() > 0)) {
				
				gameManager->spawnEnemy();
			}
			else{
				dspawn += dt;
			}
			gameManager->CheckPlayerCollision();
			gameManager->CheckEnemiesCollisions();
			
			gameManager->updatePowerUps(dt);
			
			break;
		}
		
		
		default:
			break;
			
	}


}


void Game::ProcessInput(GLfloat dt) {
	
	
	switch (this->State) {
		
		case (GAME_ACTIVE):{
			// Move player
			if (this->Keys[GLFW_KEY_A]) {
				GameObject *Player = gameManager->getPlayer();
				GLfloat velocity = 500.0f * dt;
				
				if (Player->Position.x >= 0)
					Player->Position.x -= velocity;
			}
			else if (this->Keys[GLFW_KEY_D]) {
				GameObject *Player = gameManager->getPlayer();
				GLfloat velocity = 500.0f * dt;
				
				if (Player->Position.x <= this->Width - Player->Size.x)
					Player->Position.x += velocity;
			}
			if (this->Keys[GLFW_KEY_S]) {
				GameObject *Player = gameManager->getPlayer();
				GLfloat velocity = 500.0f * dt;
				
				if (Player->Position.y <= this->Height - Player->Size.y)
					Player->Position.y += velocity;
			}
			else if (this->Keys[GLFW_KEY_W]) {
				GameObject *Player = gameManager->getPlayer();
				GLfloat velocity = 500.0f * dt;
				
				if (Player->Position.y >= 0)
					Player->Position.y -= velocity;
			}
			if (this->Keys[GLFW_KEY_SPACE]) {
				this->gameManager->PressTheTrigger(dt);
			}
			if (this->Keys[GLFW_KEY_Q]) {
				this->gameManager->ActivatePowerUp();
			}
			if (((dt - dtA) >= 0.5) && (this->Keys[GLFW_KEY_ENTER])) {
				this->State = GAME_PAUSED;
				dtA = 0.0;
			}
			else {
				dtA -= dt;
			}
			break;
			
		}
		
		case(GAME_PAUSED): {
			
			if (((dt - dtA) >= 0.5) && (this->Keys[GLFW_KEY_ENTER])) {
				this->State = GAME_ACTIVE;
				dtA = 0.0;
			}
			else {
				dtA -= dt;
			}
			break;
		}
		
		case(GAME_WIN): {
			if (this->Keys[GLFW_KEY_ENTER]) {
				this->State = GAME_ACTIVE;
				gameManager->generateEnemies();
				gameManager->spawnEnemy();
				gameManager->spawnEnemy();
				gameManager->level += 1;
			}
			break;
		}
		
		case(GAME_LOST): {
			if (this->Keys[GLFW_KEY_ENTER]) {
				delete gameManager;
				
				gameManager = new GameManager(this->Width, this->Height);
				gameManager->generateEnemies();
				gameManager->spawnEnemy();
				
				this->State = GAME_ACTIVE;
			}
			break;
		}
	}
}

void Game::Render()
{

	if((this->State == GAME_PAUSED) || (this->State == GAME_ACTIVE)) {
		
		// Draw background
		Texture2D backGround;
		backGround = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(backGround, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		// Draw game elements
		gameManager->Draw(*Renderer);
		
		std::stringstream lifes, level, points, powerUp;
		lifes << gameManager->getPlayerLifes();
		level << gameManager->level;
		points << gameManager->getPoints();
		switch (gameManager->getPlayerPowerUp()) {
			case 1: {
				powerUp << "Escudo";
				break;
			}
			case 2: {
				powerUp << "Misiles";
				break;
			}
			case 3: {
				powerUp << "Laser";
				break;
			}
			default:
			{
				powerUp << "N/A";
				break;
			}
		}
		
		textRenderer->RenderText("Vidas:" + lifes.str() + "\nNivel:" + level.str() + "\nPuntos:" + points.str() + "\nPowerUp:" + powerUp.str()
				, 5.0f, 5.0f, 1.0f );
		
		
		if (this->State == GAME_PAUSED) {
			textRenderer->RenderText("Presione ENTER para continuar", Width / 3, Height / 2, 1.0f);
		}
	}
	
	else if(this->State == GAME_WIN){
		textRenderer->RenderText(
				"Nivel superado!!!", 420.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		textRenderer->RenderText(
				"Presiona ENTER para continuar.", 250.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}
	
	else if(this->State == GAME_LOST){
		textRenderer->RenderText(
				"Has muerto :(", 420.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		textRenderer->RenderText(
				"Presiona ENTER para volver a intentarlo.", 200.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}
	
	
	if(gameManager->getPlayerLifes() <= 0){
		this->State = GAME_LOST;
	}
	
	
	if(gameManager->getEnemies().sizeOf() == 0){
		this->State = GAME_WIN;
	}
}



