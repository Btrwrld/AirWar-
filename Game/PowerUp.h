//
// Created by erick on 18/04/17.
//

#ifndef AIRWAR_POWERUP_H
#define AIRWAR_POWERUP_H


#include <glm/vec2.hpp>
#include "GameObject.h"

const glm::vec2 SIZE(50, 50);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	// PowerUp State
	GLushort Type;
	GLfloat     Duration = 30.0f;
	GLboolean   Activated = false;
	// Constructor
	PowerUp(GLushort type, glm::vec3 color, GLfloat duration,
	        glm::vec2 position, Texture2D texture)
			: GameObject(position, SIZE, texture, color, VELOCITY),
			  Type(type), Duration(duration), Activated()
	{ }
};
#endif //AIRWAR_POWERUP_H
