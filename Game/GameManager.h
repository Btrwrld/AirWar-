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

#include <glm/glm.hpp>
#include <GL/glew.h>

//The class that manipulates the enemies
class GameManager {
	
public:
	// Constructor
	GameManager(GLshort width, GLshort height);
	void CheckCollision();
	
	// Render player and enemies
	void Draw(SpriteRenderer &renderer);
	
	//Enemy-related actions
	void LoadEnemies(const GLchar *file);
	void ControlEnemies(GLfloat dt);
	
	
	//PLayer-related actions
	void PressTheTrigger(GLfloat dt);
	
	//Getters
	DoublyLinkedList<GameObject> &getEnemies() ;
	DoublyLinkedList<GameObject> &getEnemyShots() ;
	GameObject *getPlayer() ;
	DoublyLinkedList<GameObject> &getPlayerShots() ;
	GLfloat* getDPlayerShot();

private:
	// Enemy management
	DoublyLinkedList<GameObject> Enemies;
	DoublyLinkedList<GameObject> enemyShots;
	GLfloat dEnemyShot = 0.0;
	
	
	
	// Player management
	GameObject *Player = nullptr;
	DoublyLinkedList<GameObject> playerShots;
	GLushort playerLifes = 3;
	GLfloat dPlayerShot = 0.0; //Time from the last shot
	GLshort damagePerShot = 25;
	
	
	// Shot management
	const glm::vec2 INITIAL_SHOT_VELOCITY = glm::vec2(0.0f, -350.0f);// Initial velocity of the shot
	// Radius of the shot
	const GLfloat SHOT_RADIUS = 5.5f;
	
	
	//Draw pipeline management
	Queue<GameObject> animations;
	
	//const GLfloat PLAYER_VELOCITY = 500.0f;
	
	
	GLshort Width, Height;
};


#endif //AIRWAR_AI_H
