//
// Created by erick on 22/03/17.
//

#ifndef AIRWAR_SPRITERENDERER_H
#define AIRWAR_SPRITERENDERER_H


#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();
	
	void DrawSprite(Texture2D &texture, glm::vec2 position,
	                glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
	                glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;
	
	void initRenderData();
};


#endif //AIRWAR_SPRITERENDERER_H
