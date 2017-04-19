//
// Created by erick on 20/03/17.
//

#ifndef AIRWAR_GAME_H
#define AIRWAR_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "../Game/GameManager.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "TextRenderer.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_PAUSED,
	GAME_LOST,
	GAME_WIN,
};

class Game {
private:
	// Game-related State data
	SpriteRenderer*  Renderer = nullptr;
	GameManager*     gameManager = nullptr;

public:
	// Game state
	GameState  State;
	GLboolean  Keys[1024];
	GLushort   Width, Height;
	
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	
	
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};


#endif //AIRWAR_GAME_H
