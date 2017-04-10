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
	Player->life = 300;                                                           //Player life
	
}

void GameManager::CheckCollision() { // AABB - AABB collision
	GameObject *iterator = nullptr;
	
	//Colision detection for player and enemy shots
	for (int i = 0; i < this->enemyShots.sizeOf(); i++) {
		
		//Iterator gets the value of the enemy shot
		iterator = this->enemyShots.get(i);
		
		// Collision x-axis?
		bool collisionX = Player->Position.x + Player->Size.x >= iterator->Position.x &&
		                  iterator->Position.x + iterator->Size.x >= Player->Position.x;
		// Collision y-axis?
		bool collisionY = Player->Position.y + Player->Size.y >= iterator->Position.y &&
		                  iterator->Position.y + iterator->Size.y >= Player->Position.y;
		
		// Collision only if on both axes
		if (collisionX && collisionY) {
			
			Player->life -= iterator->tag * 30;
			
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
	
	
	GameObject *auxIterator = nullptr;
	//Colision detection for enemies and player bullets
	for (int i = 0; i < this->Enemies.sizeOf(); i++) {
		//Iterator takes the value of an enemy
		iterator = this->Enemies.get(i);
		
		
		for (int j = 0; j < this->playerShots.sizeOf(); j++) {
			
			//AuxIterator takes the value of a shot
			auxIterator = this->playerShots.get(j);
			
			// Collision x-axis?
			bool collisionX = auxIterator->Position.x + auxIterator->Size.x >= iterator->Position.x &&
			                  iterator->Position.x + iterator->Size.x >= auxIterator->Position.x;
			// Collision y-axis?
			bool collisionY = auxIterator->Position.y + auxIterator->Size.y >= iterator->Position.y &&
			                  iterator->Position.y + iterator->Size.y >= auxIterator->Position.y;
			
			
			// Collision only if on both axes
			if (collisionX && collisionY) {
				
				iterator->life -= this->damagePerShot;
				Texture2D auxText;
				if (iterator->life <= 0) {
					auxText = ResourceManager::GetTexture("death");
				} else {
					//va hit
					auxText = ResourceManager::GetTexture("hit");
				}
				auxIterator->tag = 0;
				this->animations.push(*new GameObject(iterator->Position,
				                                      glm::vec2(50.0f, 50.f),
				                                      auxText));
			}
		}
		
	}
	
	
	//Colision detection for player crashing with enemies
	for (int i = 0; i < this->Enemies.sizeOf(); i++) {
		iterator = this->Enemies.get(i);
		
		// Collision x-axis?
		bool collisionX = Player->Position.x + Player->Size.x >= iterator->Position.x &&
		                  iterator->Position.x + iterator->Size.x >= Player->Position.x;
		// Collision y-axis?
		bool collisionY = Player->Position.y + Player->Size.y >= iterator->Position.y &&
		                  iterator->Position.y + iterator->Size.y >= Player->Position.y;
		
		// Collision only if on both axes
		if (collisionX && collisionY) {
			
			Texture2D auxText;
			auxText = ResourceManager::GetTexture("death");
			
			playerLifes -= 1;
			Player->life = 0;
			
			iterator->life = 0;
			
			//Dormir el tread un tiempo
			
			this->animations.push(*new GameObject(iterator->Position,
			                                      iterator->Size,
			                                      auxText));
			
			this->animations.push(*new GameObject(Player->Position,
			                                      Player->Size,
			                                      auxText));
		}
	}
	
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
		Player->life = 300;
	}
	this->Player->Draw(renderer);
	
	for (int i = 0; i < this->animations.sizeOf() ; i++) {
		this->animations.get()->Draw(renderer);
		animations.dequeue();
	}

}






///            Enemy Actions             \\\*
void GameManager::ControlEnemies(GLfloat dt)
{
	
	
	GameObject* iterator = nullptr;
	for (int i = 0; i < enemyShots.sizeOf(); i++) {
		iterator = enemyShots.get(i);
		iterator->Position -= iterator->Velocity * dt;
	}
	
	
	
	for (int i = 0; i < this->Enemies.sizeOf() ; i++) {
		
		//In this case iterator wil haave the data of the enemies
		iterator = this->Enemies.get(i);
		
		switch(iterator->tag){
			
			case 10:
				//The enemy is a jet so it should behave like one
				//Shoot the player if it is on range
				if((    (this->Player->Position.x +100 <= iterator->Position.x + 125) ||
						(this->Player->Position.x <= iterator->Position.x + 162)) &&
						((dt - this->dEnemyShot) >= 0.3f)){
					
					
					GameObject* shot = new GameObject(glm::vec2(iterator->Position.x + 125, this->Enemies.get(0)->Position.y + 70) ,
					                                  glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2), ResourceManager::GetTexture("face"),
					                                  glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
					shot->tag = 1;
					enemyShots.addLast(*shot);
					
					
					shot = new GameObject(glm::vec2(iterator->Position.x + 162, this->Enemies.get(0)->Position.y + 70),
					                      glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2), ResourceManager::GetTexture("face"),
					                      glm::vec3(1.0f), glm::vec2(0.0f, -400.0f));
					shot->tag = 1;
					enemyShots.addLast(*shot);
					
					
					this->dEnemyShot = dt;
					
				}
				else{
					if(this->Player->Position.x - 100 < iterator->Position.x){
						iterator->Position.x -= iterator->Velocity.x * dt;
					}
					if(this->Player->Position.x - 100 > iterator->Position.x){
						iterator->Position.x += iterator->Velocity.x * dt;
					}
					this->dEnemyShot -= dt;
				}
				
				if(this->Player->Position.y - iterator->Position.y > 600){
					iterator->Position.y += iterator->Velocity.y * dt;
				}
				else if(( this->Player->Position.y - iterator->Position.y < 600) && (iterator->Position.y > 0)){
					iterator->Position.y -= iterator->Velocity.y * dt;
				}
				
				
				break;
			
			case 2:
				//The enemy behaves as a bomber
				
				break;
			
			case 3:
				//Se genera un jet
				
				break;
			
			case 4:
				//Se genera un jet
				
				break;
			
			case 5:
				//Se genera un jet
				
				break;
			
			case 6:
				//Se genera un jet
				
				break;
			
			case 7:
				//Se genera un jet
				
				break;
			
			case 8:
				//Se genera el boss
				
				break;
			
			default:
				iterator = nullptr;
				break;
			
		}
	}
		
		
}


void GameManager::LoadEnemies(const GLchar *file)
{
	// Load from file
	Queue<GLshort> data = ResourceManager::LoadEnemies(file);
	
	while(data.sizeOf() > 0) {
		GameObject* enemy;
		
		switch (*data.get()){
			
			case 1:
				//A jet is generated
				enemy = new GameObject (glm::vec2(700 % 1100, 50), glm::vec2(300, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(300.0f));
				enemy->tag = 1;
				break;
			
			case 2:
				//A bomber is generated
				enemy = new GameObject (glm::vec2(500 % 1100, 50), glm::vec2(300, 100), ResourceManager::GetTexture("bomber"),
				                        glm::vec3(1.0f), glm::vec2(400.0f));
				enemy->tag = 2;
				break;
			
			case 3:
				//A tower is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 3;
				break;
			
			case 4:
				//A missile tower is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 4;
				break;
			
			case 5:
				//A kamikaze is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 5;
				break;
			
			case 6:
				//Custom1 is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 6;
				break;
			
			case 7:
				//Custom2 is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("jet"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 7;
				break;
			
			case 8:
				//The level boss is generated
				enemy = new GameObject (glm::vec2(rand() % 1100, 2), glm::vec2(100, 100), ResourceManager::GetTexture("boss"),
				                        glm::vec3(1.0f), glm::vec2(650.0f));
				enemy->tag = 8;
				break;
			
			default:
				enemy = nullptr;
				break;
			
		}
		
		if(enemy != nullptr){
			this->Enemies.addLast(*enemy);
		}
		else{
			std::cout << "ERROR: Se esta intentando agregar un enemigo que no existe. GameManager::LoadEnemies";
			exit(EXIT_FAILURE);
		}
		data.dequeue();
		
	}
}






///            Player Actions             \\\*
void GameManager::PressTheTrigger(GLfloat dt) {
	
	if((dt - this->dPlayerShot) >= 0.2f) {
		
		
		//                                  Position                                                 ,   size
		GameObject shot = *new GameObject(glm::vec2(Player->Position.x + 4 , Player->Position.y + 30), glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
		                                  ResourceManager::GetTexture("face"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
		shot.tag = 1;//                   Texture                            ,  color           velocity
		this->playerShots.addLast(shot);
		
		
		
		shot = *new GameObject( glm::vec2(Player->Position.x + 84.5 , Player->Position.y + 30), glm::vec2(SHOT_RADIUS * 2, SHOT_RADIUS * 2),
		                        ResourceManager::GetTexture("face"), glm::vec3(1.0f), INITIAL_SHOT_VELOCITY);
		shot.tag = 1;
		this->playerShots.addLast(shot);
		
		
		this->dPlayerShot = dt;
		
	}
	else{
		this->dPlayerShot -= dt;
	}
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

GLfloat* GameManager::getDPlayerShot() {
	return &dPlayerShot;
}
