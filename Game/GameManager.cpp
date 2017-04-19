//
// Created by erick on 26/03/17.
//

#include "GameManager.h"

GameManager::GameManager(GLshort width, GLshort height){
	
	this->Width = width;
	this->Height = height;
	//Player setUp                      w/2 - playerSize.x/2    h - playerSize.y
	Player = new GameObject( glm::vec2( width / 2 - 100.0f / 2, height - 100.0f), //Player position
	                         glm::vec2(100.0f, 100.f),                            //Player size
	                         ResourceManager::GetTexture("ship"));                //Player texture
	Player->life = 500;
	Player->tag = 1;
	Player->Color = glm::vec3(0.0f , 1.0f , 0.0);
}

///     Colision detection and position calculation      \\\*
void GameManager::CheckPlayerCollision() { // AABB - AABB collision
	GameObject *iterator = nullptr;
	
	//Colision detection for player and enemy shots
	if(!this->Player->hasShield){
		for (int i = 0; i < this->enemyShots.sizeOf(); i++) {
			
			iterator = this->enemyShots.get(i);
			
			// Calls tha auxilary function tho check the axis collision
			if (this->CheckPlayerCollisionAux(iterator)) {
				
				Player->life -= iterator->tag * (9 + this->level);
				
				Texture2D auxText;
				if (Player->life <= 0) {
					auxText = ResourceManager::GetTexture("death");
					playerLifes -= 1;
					
					//Dormir el tread un tiempo
					
				} else {
					auxText = ResourceManager::GetTexture("hit");
					
				}
				iterator->tag = 0;
				this->animations.push(*new GameObject(iterator->Position,
				                                      glm::vec2(50.0f, 50.f),
				                                      auxText));
			}
			
		}
	}
	
	PowerUp *PUiterator = nullptr;
	//Colision detection for player powerUps
	for (int i = 0; i < this->fieldPowerUps.sizeOf(); i++) {
		
		//Iterator gets the value of the enemy shot
		PUiterator = this->fieldPowerUps.get(i);
		
		// Collision only if on both axes
		if ( this->CheckPlayerCollisionAux( PUiterator ) ) {
			
			this->playerPowerUps.push_back( *PUiterator );
			this->fieldPowerUps.remove(i);
		}
		
	}
}

bool GameManager::CheckPlayerCollisionAux(GameObject* iterator){
	
	// Collision x-axis?
	bool collisionX = Player->Position.x + Player->Size.x >= iterator->Position.x &&
	                  iterator->Position.x + iterator->Size.x >= Player->Position.x;
	// Collision y-axis?
	bool collisionY = Player->Position.y + Player->Size.y >= iterator->Position.y &&
	                  iterator->Position.y + iterator->Size.y >= Player->Position.y;
	
	return collisionX && collisionY;
}


void GameManager::CheckEnemiesCollisions() { // AABB - AABB collision
	GameObject *iterator = nullptr;
	GameObject *auxIterator = nullptr;
	
	//Colision detection for enemies and player bullets
	for (int i = 0; i < this->Enemies.sizeOf(); i++) {
		//Iterator takes the value of the limits of the enemy
		iterator = this->Enemies.get(i);
		if(iterator->tag !=7) {
			glm::vec4 enemyPosition = getTruePosition(iterator);
			
			for (int j = 0; j < this->playerShots.sizeOf(); j++) {
				
				//AuxIterator takes the value of a shot
				auxIterator = this->playerShots.get(j);
				
				// Calls tha auxilary function tho check the axis collision
				if (CheckEnemiesCollisionsAux(auxIterator, enemyPosition)) {
					
					iterator->life -= auxIterator->tag * (29 + this->level);
					Texture2D auxText;
					if (iterator->life <= 0) {
						this->points += iterator->tag;
						
						short powerUpProb = rand() % 12;
						PowerUp *power = nullptr;
						switch (powerUpProb) {
							
							case 12: {
								// 12 represents a laser
								power = new PowerUp(3, glm::vec3(1.0f), 30.f,
								                    glm::vec2(iterator->Position.x, iterator->Position.y),
								                    ResourceManager::GetTexture("laserPU"));
								break;
							}
							case 11: {
								// 11 represents missiles
								power = new PowerUp(2, glm::vec3(1.0f), 30.f,
								                    glm::vec2(iterator->Position.x, iterator->Position.y),
								                    ResourceManager::GetTexture("missilePU"));
								break;
							}
							case 10: {
								// 10 represents the shield
								power = new PowerUp(1, glm::vec3(1.0f), 30.f,
								                    glm::vec2(iterator->Position.x, iterator->Position.y),
								                    ResourceManager::GetTexture("shield"));
								break;
							}
							default:
								break;
						}
						
						if (power != nullptr) {
							this->fieldPowerUps.addLast(*power);
						}
						
						auxText = ResourceManager::GetTexture("death");
					} else {
						auxText = ResourceManager::GetTexture("hit");
					}
					auxIterator->tag = 0;
					this->animations.push(*new GameObject(auxIterator->Position,
					                                      glm::vec2(50.0f, 50.f),
					                                      auxText));
				}
			}
		}
		
	}
	
	
	//Colision detection for player crashing with enemies
	if(!this->Player->hasShield) {
		for (int i = 0; i < this->Enemies.sizeOf(); i++) {
			if (this->Enemies.get(i)->tag != 7) {
				glm::vec4 enemyPosition = getTruePosition(this->Enemies.get(i));
				
				if (CheckEnemiesCollisionsAux(this->Player, enemyPosition)) {
					
					Texture2D auxText;
					auxText = ResourceManager::GetTexture("death");
					
					Player->life = 0;
					playerLifes -= 1;
					
					this->Enemies.get(i)->life = 0;
					
					//Dormir el tread un tiempo
					
					this->animations.push(*new GameObject(glm::vec2(enemyPosition.x, enemyPosition.z),
					                                      glm::vec2(enemyPosition.x - enemyPosition.y,
					                                                enemyPosition.z - enemyPosition.w),
					                                      auxText));
					
					this->animations.push(*new GameObject(Player->Position,
					                                      Player->Size,
					                                      auxText));
				}
			}
		}
	}
}

bool GameManager::CheckEnemiesCollisionsAux(GameObject* auxIterator, glm::vec4 enemyPosition ){
	
	// Collision x-axis?
	bool collisionX = auxIterator->Position.x + auxIterator->Size.x >= enemyPosition.x &&
	                  enemyPosition.y >= auxIterator->Position.x;
	// Collision y-axis?
	bool collisionY = auxIterator->Position.y + auxIterator->Size.y >= enemyPosition.z &&
	                  enemyPosition.w >= auxIterator->Position.y;
	return collisionX && collisionY;
}


glm::vec4 GameManager::getTruePosition(GameObject* enemy){
	glm::vec4 position;
	
	switch(enemy->tag) {
		//Format:
			//position = (Xmin, Xmax, Ymin, Ymax)
		case 1: {
			//The enemy is a jet
			position = glm::vec4(enemy->Position.x + 125, enemy->Position.x + 162, enemy->Position.y + 15, enemy->Position.y + 70);
			
			break;
		}
		
		case 2: {
			//The enemy is a bomber
			position = glm::vec4(enemy->Position.x + 125, enemy->Position.x + 162, enemy->Position.y + 15, enemy->Position.y + 73);
			
			break;
		}
		
		case 3: {
			//The enemy is a tower
			position = glm::vec4(enemy->Position.x + 20, enemy->Position.x + 80, enemy->Position.y, enemy->Position.y + 160);
			break;
		}
		
		case 4: {
			//The enemy is a missile tower
			position = glm::vec4(enemy->Position.x + 10, enemy->Position.x + 90, enemy->Position.y, enemy->Position.y + 165);
			break;
		}
		
		case 5: {
			//The enemy is a kamikaze
			position = glm::vec4(enemy->Position.x + 275, enemy->Position.x + 310, enemy->Position.y + 110, enemy->Position.y + 175);
			
			break;
		}
		
		case 6: {
			//The enemy is a tank
			position = glm::vec4(enemy->Position.x + 130, enemy->Position.x + 265, enemy->Position.y + 90, enemy->Position.y + 210);
			break;
		}
		
		case 8: {
			//The enemy is the boss
			position = glm::vec4(enemy->Position.x + 250, enemy->Position.x + 450, enemy->Position.y + 50, enemy->Position.y + 430);
			
			break;
		}
	}
	
	return position;
}





void GameManager::Draw(SpriteRenderer &renderer)
{
	
	//Dibujar primero disparos, luego animaciones y finalmente naves para evitar
	//inconsistencias en la vista del juego
	GameObject* iterator;
	for (int i = 0; i < this->playerShots.sizeOf() ; i++) {
		
		iterator = this->playerShots.get(i);
		if((iterator->Position.y < 0.0f) || (iterator->tag == 0)){
			this->playerShots.remove(i);
		}
		else{
			iterator->Draw(renderer);
		}
	}
	
	for (int i = 0; i < this->enemyShots.sizeOf() ; i++) {
		
		iterator = this->enemyShots.get(i);
		if((iterator->Position.y > this->Height) || (iterator->tag == 0)){
			this->enemyShots.remove(i);
		}
		else{
			iterator->Draw(renderer);
		}
	}
	
	for (int i = 0; i < this->Enemies.sizeOf() ; i++) {
		iterator = this->Enemies.get(i);
		if(iterator->life <= 0){
			this->Enemies.remove(i);
		}
		else{
			iterator->Draw(renderer);
		}
		
	}
	
	if(this->Player->life <= 0) {
		Player->Position = glm::vec2(this->Width / 2 - 100.0f / 2, this->Height - 100.0f);
		Player->life = 475 + 25*this->level;
		// Player looses the PU
		for (int i = 0; i < this->playerPowerUps.sizeOf(); i++){
			this->playerPowerUps.pop_back();
		}
	}
	
	this->Player->Draw(renderer);
	
	for (int i = 0; i < this->animations.sizeOf() ; i++) {
		this->animations.get()->Draw(renderer);
		animations.dequeue();
	}
	
	for (int i = 0; i < this->fieldPowerUps.sizeOf() ; i++) {
		this->fieldPowerUps.get(i)->Draw(renderer);
	}

}



///           Updates and movements      \\\*
void GameManager::updateShots(GLfloat dt) {
	
	GameObject* iterator = nullptr;
	for (int i = 0; i < this->playerShots.sizeOf(); ++i) {
		iterator = this->playerShots.get(i);
		iterator->Position += iterator->Velocity * dt;
	}
	
	for (int i = 0; i < enemyShots.sizeOf(); i++) {
		iterator = enemyShots.get(i);
		iterator->Position -= iterator->Velocity * dt;
	}
	
}


void GameManager::updatePowerUps(GLfloat dt) {
	
	if(this->playerPowerUps.sizeOf() > 0) {
		PowerUp *actual = this->playerPowerUps.peek();
		if (actual->Activated) {
			if (actual->Duration <= 0) {
				
				Player->tag = 1;
				Player->hasShield = GL_FALSE;
				Player->Color = glm::vec3(0.0f, 1.0f, 0.0f);
				
				this->playerPowerUps.pop_back();
			} else {
				actual->Duration -= dt;
				if (Player->hasShield) {
					Player->Color = glm::vec3(1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
}


///            Enemy-related             \\\*
void GameManager::ControlEnemies(GLfloat dt, GLushort enemyPos)
{
	
	
	GameObject* enemy = this->Enemies.get(enemyPos);
		
	switch(enemy->tag){
		
		case 1:{
			//The enemy is a jet so it should behave like one
			//Shoot the player if it is on range
			if(     (enemy->Position.x + 163 >= Player->Position.x) &&
					(this->Player->Position.x + 100 >= enemy->Position.x + 125) &&
					((dt - enemy->dt) >= 0.3f)){
				
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 125, enemy->Position.y + 70) ,
				                                  glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2), ResourceManager::GetTexture("face"),
				                                  glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
				shot->tag = 1;
				enemyShots.addLast(*shot);
				
				
				shot = new GameObject(glm::vec2(enemy->Position.x + 162,enemy->Position.y + 70),
				                      glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2), ResourceManager::GetTexture("face"),
				                      glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
				shot->tag = 1;
				enemyShots.addLast(*shot);
				
				
				enemy->dt = dt;
				
			}
			else {
				if (this->Player->Position.x - 100 < enemy->Position.x) {
					enemy->Position.x -= enemy->Velocity.x * dt;
				} else if (this->Player->Position.x - 100 > enemy->Position.x) {
					enemy->Position.x += enemy->Velocity.x * dt;
				}
				
				if (this->Player->Position.y - enemy->Position.y > 300) {
					enemy->Position.y += enemy->Velocity.y * dt;
				} else if ((this->Player->Position.y - enemy->Position.y < 600) && (enemy->Position.y > 0)) {
					enemy->Position.y -= enemy->Velocity.y * dt;
				}
				
				enemy->dt -= dt;
			}
			
			
			break;}
		
		case 2:{
			//The enemy behaves as a bomber
			if(     (enemy->Position.x + 163 >= Player->Position.x) &&
			        (this->Player->Position.x + 100 >= enemy->Position.x + 125) &&
			        ((dt - enemy->dt) >= 5)){
				
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 90, enemy->Position.y + 70) ,
				                                  glm::vec2(40 * 2, 20 * 2), ResourceManager::GetTexture("missile"),
				                                  glm::vec3(1.0f), glm::vec2(0.0f, -200.0f));
				shot->tag = 4;
				shot->Rotation = 3.14;
				enemyShots.addLast(*shot);
				
				
				shot = new GameObject(glm::vec2(enemy->Position.x + 132, enemy->Position.y + 70),
				                      glm::vec2(40 * 2, 20 * 2), ResourceManager::GetTexture("missile"),
				                      glm::vec3(1.0f), glm::vec2(0.0f, -200.0f));
				shot->tag = 4;
				shot->Rotation = 3.14;
				enemyShots.addLast(*shot);
				
				
				enemy->dt = dt;
				
			}
			else {
				if (this->Player->Position.x - 100 < enemy->Position.x) {
					enemy->Position.x -= enemy->Velocity.x * dt;
				} else if (this->Player->Position.x - 100 > enemy->Position.x) {
					enemy->Position.x += enemy->Velocity.x * dt;
				}
				
				
				if (this->Player->Position.y > enemy->Position.y) {
					enemy->Position.y += enemy->Velocity.y * dt;
				} else if ((this->Player->Position.y < enemy->Position.y) && (enemy->Position.y > 0)) {
					enemy->Position.y -= enemy->Velocity.y * dt;
				}
				
				enemy->dt -= dt;
			}
			break;}
		
		case 3:{
			//The enemy behaves as a tower
			
			if((dt - enemy->dt) >= 0.7f){
				GLfloat xVelocity, yVelocity, velocity;
				velocity = sqrtf(this->Player->Position.x*this->Player->Position.x +
						                 this->Player->Position.y*this->Player->Position.y);
				if (this->Player->Position.x > enemy->Position.x)
					xVelocity = this->Player->Position.x/velocity * -100 - 300;
				else
					xVelocity = this->Player->Position.x/velocity * +100 +300;
				
				if (this->Player->Position.y > enemy->Position.y)
					yVelocity = this->Player->Position.y/velocity * -100 -300;
				else
					yVelocity = this->Player->Position.y/velocity * 100 +300;
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 50, enemy->Position.y + 65) ,
				                                  glm::vec2(5.5f * 2, 5.5f * 2), ResourceManager::GetTexture("face"),
				                                  glm::vec3(1.0f), glm::vec2( xVelocity, yVelocity));
				shot->tag = 3;
				enemyShots.addLast(*shot);
				
				
				enemy->dt = dt;
				
			}
			else{
				enemy->dt -= dt;
			}
			
			
			break;}
		
		case 4:{
			//The enemy behaves as a missile tower
			
			if((dt - enemy->dt) >= 0.7f) {
				GLfloat xVelocity, yVelocity, velocity, rotationAngle;
				velocity = sqrtf(this->Player->Position.x * this->Player->Position.x +
				                 this->Player->Position.y * this->Player->Position.y);
				
				if (this->Player->Position.x > enemy->Position.x) {
					xVelocity = this->Player->Position.x / velocity * -100 - 300;
					rotationAngle = 3.14 / 2;
				} else {
					xVelocity = this->Player->Position.x / velocity * +100 + 300;
					rotationAngle = -3.14 / 2;
				}
				
				if (this->Player->Position.y > enemy->Position.y){
					yVelocity = this->Player->Position.y / velocity * -100 - 300;
					
					if (rotationAngle > 0)
						rotationAngle += 3.14 / 4;
					else
						rotationAngle -= 3.14 / 4;
				}
				else {
					yVelocity = this->Player->Position.y / velocity * 100 + 300;
					if (rotationAngle > 0)
						rotationAngle -= 3.14 / 4;
					else
						rotationAngle = 3.14 / 4;
				}
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 50, enemy->Position.y + 65) ,
				                                  glm::vec2(40 * 2, 20 * 2), ResourceManager::GetTexture("missile"),
				                                  glm::vec3(1.0f), glm::vec2( xVelocity, yVelocity));
				shot->tag = 4;
				shot->Rotation = rotationAngle;
				enemyShots.addLast(*shot);
				
				
				enemy->dt = dt;
				
			}
			else{
				enemy->dt -= dt;
			}
			break;}
		
		case 5:{
			//The enemy behaves as a kamikaze
			glm::vec4 kamikazePos = getTruePosition(enemy);
			if(this->Player->Position.x  < kamikazePos.x){
				enemy->Position.x -= enemy->Velocity.x * dt;
			}
			else if(this->Player->Position.x  > kamikazePos.y){
				enemy->Position.x += enemy->Velocity.x * dt;
			}
			
			if(this->Player->Position.y  > kamikazePos.z){
				enemy->Position.y += enemy->Velocity.y * dt;
			}
			else if(( this->Player->Position.y  < kamikazePos.w) && (kamikazePos.z > 0)) {
				enemy->Position.y -= enemy->Velocity.y * dt;
			}
			break;}
		
		case 6:{
			//The enemy behaves as a tank
			
			if(     (enemy->Position.x + 220 >= Player->Position.x) &&
			        (this->Player->Position.x + 100 >= enemy->Position.x + 170) &&
			        ((dt - enemy->dt) >= 0.3f)){
				
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 125, enemy->Position.y + 70) ,
				                                  glm::vec2(10 * 2, 10 * 2), ResourceManager::GetTexture("laser"),
				                                  glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
				shot->tag = 6;
				enemyShots.addLast(*shot);
				
				
				shot = new GameObject(glm::vec2(enemy->Position.x + 162, enemy->Position.y + 70),
				                      glm::vec2(10 * 2, 10 * 2), ResourceManager::GetTexture("laser"),
				                      glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
				shot->tag = 6;
				enemyShots.addLast(*shot);
				
				
				enemy->dt = dt;
				
			}
			else {
				if (this->Player->Position.x - 100 < enemy->Position.x) {
					enemy->Position.x -= enemy->Velocity.x * dt;
				} else if (this->Player->Position.x - 100 > enemy->Position.x) {
					enemy->Position.x += enemy->Velocity.x * dt;
				}
				
				if (this->Player->Position.y - enemy->Position.y > 600) {
					enemy->Position.y += enemy->Velocity.y * dt;
				} else if ((this->Player->Position.y - enemy->Position.y < 600) && (enemy->Position.y > 0)) {
					enemy->Position.y -= enemy->Velocity.y * dt;
				}
				
				enemy->dt -= dt;
			}
			
			
			break;}
		
		case 7:{
			//The enemy behaves as an ally
			GameObject* realEnemy = this->Enemies.get(0);
			if(     (enemy->Position.x + 220 >= realEnemy->Position.x) &&
			        (realEnemy->Position.x + 100 >= enemy->Position.x + 170) &&
			        ((dt - enemy->dt) >= 0.3f)){
				
				GameObject shot = *new GameObject(glm::vec2(enemy->Position.x + 4 , enemy->Position.y + 30), glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
				                                  ResourceManager::GetTexture("face"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;//                   Texture                            ,  color           velocity
				this->playerShots.addLast(shot);
				
				
				
				shot = *new GameObject( glm::vec2(enemy->Position.x + 84.5 , enemy->Position.y + 30), glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
				                        ResourceManager::GetTexture("face"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;
				this->playerShots.addLast(shot);
				enemy->dt = dt;
				
				enemy->life -=1;
			}
			else {
				if (realEnemy->Position.x - 100 < enemy->Position.x) {
					enemy->Position.x -= enemy->Velocity.x * dt;
				}
				else if (realEnemy->Position.x - 100 > enemy->Position.x) {
					enemy->Position.x += enemy->Velocity.x * dt;
				}
				enemy->dt -= dt;
				
				
				if (realEnemy->Position.y - enemy->Position.y > 600) {
					enemy->Position.y += enemy->Velocity.y * dt;
				} else if ((realEnemy->Position.y - enemy->Position.y < 600) && (enemy->Position.y > 0)) {
					enemy->Position.y -= enemy->Velocity.y * dt;
				}
			}
			if (enemy->life ==0){
				Texture2D auxText;
				auxText = ResourceManager::GetTexture("death");
				animations.push(*new GameObject(enemy->Position,
				                                enemy->Size,
				                                auxText));
			}
			break;
		}
		
		case 8:{
			//The enemy behaves as boss
			
			if(     (enemy->Position.x + 220 >= Player->Position.x) &&
			        (this->Player->Position.x + 100 >= enemy->Position.x + 170) &&
			        ((dt - enemy->dt) >= 0.3f)){
				
				GameObject* shot = new GameObject(glm::vec2(enemy->Position.x + 300, this->Enemies.get(0)->Position.y + 380) ,
				                                  glm::vec2(10 * 2, 10 * 2), ResourceManager::GetTexture("laser"),
				                                  glm::vec3(1.0f), glm::vec2(0.0f, -300.0f));
				shot->tag = 8;
				this->enemyShots.addLast(*shot);
				
				
				shot = new GameObject(glm::vec2(enemy->Position.x + 390, this->Enemies.get(0)->Position.y + 380),
				                      glm::vec2(10 * 2, 10 * 2), ResourceManager::GetTexture("laser"),
				                      glm::vec3(1.0f), glm::vec2(0.0f, -300.0f));
				shot->tag = 8;
				this->enemyShots.addLast(*shot);
				
				enemy->dt = dt;
				
			}
			else{
				if(this->Player->Position.x - 100 < enemy->Position.x){
					enemy->Position.x -= enemy->Velocity.x * dt;
				}
				if(this->Player->Position.x - 100 > enemy->Position.x){
					enemy->Position.x += enemy->Velocity.x * dt;
				}
				enemy->dt -= dt;
			}
			
			if(this->Player->Position.y - enemy->Position.y > 600){
				enemy->Position.y += enemy->Velocity.y * dt;
			}
			else if(( this->Player->Position.y - enemy->Position.y < 600) && (enemy->Position.y > 0)){
				enemy->Position.y -= enemy->Velocity.y * dt;
			}
			
			break;}
		
		default:{
			enemy = nullptr;
			break;
		}
	}
}


void GameManager::generateEnemies()
{
	for (int i = 0; i < 100; ++i) {
		if((i == 25) || (i == 50) || (i == 75)) {
			enemySpawn.push(7);
			i += 1;
		}
		
		enemySpawn.push((rand() % 5) + 1);
	}
	enemySpawn.push(8);
}


void GameManager::spawnEnemy() {
	
	GameObject *enemy;
	

	switch (*this->enemySpawn.get()){
		
		case 1:{
			//A jet is generated
			enemy = new GameObject (glm::vec2(700 % 1100, 50), glm::vec2(300, 100), ResourceManager::GetTexture("jet"),
			                        glm::vec3(1.0f), glm::vec2(125.0f + this->level*25));
			enemy->tag = 1;
			enemy->life = 50 + this->level*25;
			break;}
		
		case 2:{
			//A bomber is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 50), glm::vec2(300, 100), ResourceManager::GetTexture("bomber"),
			                        glm::vec3(1.0f), glm::vec2(50.0f + this->level*25));
			enemy->tag = 2;
			enemy->life = 200 + this->level*25;
			break;}
		
		case 3:{
			//A tower is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 50), glm::vec2(100, 150), ResourceManager::GetTexture("tower"),
			                        glm::vec3(1.0f), glm::vec2(0.0f));
			enemy->tag = 3;
			enemy->life = 250 + this->level*25;
			break;}
		
		case 4:{
			//A missile tower is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 50), glm::vec2(100, 175), ResourceManager::GetTexture("missileTower"),
			                        glm::vec3(1.0f), glm::vec2(0.0f));
			enemy->tag = 4;
			enemy->life = 250 + this->level*25;
			break;}
		
		case 5:{
			//A kamikaze is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(600, 300), ResourceManager::GetTexture("kamikaze"),
			                        glm::vec3(1.0f), glm::vec2(150.0f + this->level*25));
			enemy->tag = 5;
			enemy->life = 50 + this->level*25;
			break;}
		
		case 6:{
			//A tank is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(400, 300), ResourceManager::GetTexture("tank"),
			                        glm::vec3(1.0f), glm::vec2(25.0f + this->level*25 ));
			enemy->tag = 6;
			enemy->life = 300 + this->level*25;
			break;}
		
		case 7:{
			//A ally is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("ally"),
			                       glm::vec3(1.0f), glm::vec2(400.0f - this->level*5));
			enemy->tag = 7;
			break;}
		
		case 8:{
			//A boss is generated
			enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(700, 500), ResourceManager::GetTexture("boss"),
			                        glm::vec3(1.0f), glm::vec2(200.0f + this->level*25));
			enemy->tag = 8;
			enemy->life = 500 + this->level*100;
			break;}
		
		default:{
			enemy = nullptr;
			break;}
	
	}
	
	
	
	if(enemy != nullptr){
		this->Enemies.addLast(*enemy);
	}
	else{
		std::cout << "ERROR: Se esta intentando agregar un enemigo que no existe. GameManager::LoadEnemies";
		exit(EXIT_FAILURE);
	}
	
	this->enemySpawn.dequeue();
	
}




///            Player Actions             \\\*
void GameManager::PressTheTrigger(GLfloat dt) {
	
	if((dt - this->Player->dt) >= 0.2f) {
		switch(this->Player->tag) {
			
			case 2: {
				//                                  Position                                                 ,   size
				GameObject shot = *new GameObject(glm::vec2(Player->Position.x + 25, Player->Position.y ),
				                                  glm::vec2(40 * 2, 20 * 2),
				                                  ResourceManager::GetTexture("missile"), glm::vec3(1.0f),
				                                  INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;//                   Texture                            ,  color           velocity
				this->playerShots.addLast(shot);
				
				
				shot = *new GameObject(glm::vec2(Player->Position.x - 5, Player->Position.y),
				                       glm::vec2(40 * 2, 20 * 2),
				                       ResourceManager::GetTexture("missile"), glm::vec3(1.0f),
				                       INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;
				this->playerShots.addLast(shot);
				break;
			}
			case 3: {
				//                                  Position                                                 ,   size
				GameObject shot = *new GameObject(glm::vec2(Player->Position.x + 30, Player->Position.y + 30),
				                                  glm::vec2(10 * 2, 10 * 2),
				                                  ResourceManager::GetTexture("laser"), glm::vec3(1.0f),
				                                  INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;//                   Texture                            ,  color           velocity
				this->playerShots.addLast(shot);
				
				
				shot = *new GameObject(glm::vec2(Player->Position.x + 60, Player->Position.y + 30),
				                       glm::vec2(10 * 2, 10 * 2),
				                       ResourceManager::GetTexture("laser"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;
				this->playerShots.addLast(shot);
				break;
				
			}
			default: {
				//                                  Position                                                 ,   size
				GameObject shot = *new GameObject(glm::vec2(Player->Position.x + 30, Player->Position.y + 30),
				                                  glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
				                                  ResourceManager::GetTexture("face"), glm::vec3(1.0f),
				                                  INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;//                   Texture                            ,  color           velocity
				this->playerShots.addLast(shot);
				
				
				shot = *new GameObject(glm::vec2(Player->Position.x + 60, Player->Position.y + 30),
				                       glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
				                       ResourceManager::GetTexture("face"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
				shot.tag = Player->tag;
				this->playerShots.addLast(shot);
				break;
			}
			
			
		}
		this->Player->dt = dt;
	}
	else{
		this->Player->dt -= dt;
	}
}


void GameManager::ActivatePowerUp() {
	PowerUp* actual = this->playerPowerUps.peek();
	
	if(actual->Type == 3){
		Player->tag = 3;
	}
	
	else if(actual->Type == 2){
		Player->tag = 2;
	}
	
	else{
		Player->hasShield = GL_TRUE;
	}
	
	actual->Activated = GL_TRUE;
}













































DoublyLinkedList<GameObject> &GameManager::getEnemies() {
	return Enemies;
}

DoublyLinkedList<GameObject> &GameManager::getEnemyShots()  {
	return enemyShots;
}

GameObject *GameManager::getPlayer()  {
	return Player;
}

DoublyLinkedList<GameObject> &GameManager::getPlayerShots()  {
	return playerShots;
}

GLshort GameManager::getPlayerLifes() const {
	return playerLifes;
}

Queue<GLushort> &GameManager::getEnemySpawn() {
	return enemySpawn;
}

GLushort GameManager::getPoints() const {
	return points;
}

GLushort GameManager::getPlayerPowerUp() {
	if (this->playerPowerUps.sizeOf() < 0) {
		return playerPowerUps.peek()->Type;
	}
	else{
		return 0;
	}
}


