//
// Created by erick on 26/03/17.
//

#ifndef AIRWAR_AI_H
#define AIRWAR_AI_H


#include <vector>
#include <fstream>
#include <sstream>

#include "GameObject.h"
#include "../GameFramework/SpriteRenderer.h"
#include "../GameFramework/ResourceManager.h"
#include "../DataStructures/DoublyLinkedList.h"
#include "PowerUp.h"
#include "../DataStructures/Stack.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

//The class that manipulates the enemies
class GameManager {
	
public:
	// Constructor
	GameManager(GLshort width, GLshort height);
	
	// Colision management
	void CheckCollision();
	void CheckPlayerCollision();
	void CheckEnemiesCollisions();
	
	// Render player and enemies
	void Draw(SpriteRenderer &renderer);
	
	//Movement and state update related
	void updateShots(GLfloat dt);
	void updatePowerUps(GLfloat dt);
	
	//Enemy-related actions
	void generateEnemies();
	void spawnEnemy();
	void ControlEnemies(GLfloat dt, GLushort enemy);
	
	//PLayer-related actions
	void PressTheTrigger(GLfloat dt);
	void ActivatePowerUp();
	
	DoublyLinkedList<PowerUp> fieldPowerUps;
	
	//Getters
	DoublyLinkedList<GameObject> &getEnemies() ;
	DoublyLinkedList<GameObject> &getEnemyShots() ;
	GameObject *getPlayer() ;
	DoublyLinkedList<GameObject> &getPlayerShots() ;
	GLshort getPlayerLifes() const;
	Queue<GLushort> &getEnemySpawn() ;
	GLushort getPoints() const;
	GLushort getPlayerPowerUp();
	
	GLushort level= 1;

private:
	// Enemy management
	DoublyLinkedList<GameObject> Enemies;//Enemies on the game
	Queue<GLushort> enemySpawn;
	DoublyLinkedList<GameObject> enemyShots;
	
	
	// Player management
	GameObject *Player = nullptr;
	DoublyLinkedList<GameObject> playerShots;
	GLshort playerLifes = 3;
	GLushort points = 0;
	
	
	// Shot management
	const glm::vec2 INITIAL_SHOT_VELOCITY = glm::vec2(0.0f, -350.0f);// Initial velocity of the shot
	// Radius of the shot
	const GLfloat SHOT_RADIUS = 5.5f;
	
	
	// Animation management
	Queue<GameObject> animations;
	
	//Collision management
	glm::vec4 getTruePosition(GameObject* enemy);
	bool CheckPlayerCollisionAux(GameObject* iterator);
	bool CheckEnemiesCollisionsAux(GameObject* auxIterator, glm::vec4 enemyPosition );
	
	//PowerUp management
	Stack<PowerUp> playerPowerUps;

	

	
	
	GLushort Width, Height;
};


#endif //AIRWAR_AI_H
