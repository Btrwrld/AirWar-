//
// Created by erick on 22/03/17.
//

#ifndef AIRWAR_GAMEOBJECT_H
#define AIRWAR_GAMEOBJECT_H



#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../GameFramework/Texture.h"
#include "../GameFramework/SpriteRenderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
public:
	// Object state
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	GLfloat Rotation;
	GLboolean hasShield;
	GLshort life = 100;
	GLfloat dt =0.0;
	
	//Tag is an auxiliary variable that can describe many things
	//like the enemy, the damage of a bullet, etc
	GLushort tag = 0;
	
	
	
	// Render state
	Texture2D Sprite;
	
	// Constructor(s)
	GameObject();
	
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
	           glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
	
};

#endif //AIRWAR_GAMEOBJECT_H
