#pragma once

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include "Box2D\Common\b2Settings.h"

#include "LevelEditor.h"
#include "MathMethods.h"
#include "Block.h"

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Player
{
public:
	Player(sf::Vector2f spawn, sf::View* view1, LevelEditor* editor, MathMethods* m);
	~Player();
	void Update(sf::RenderWindow &w, float dt);
	void Draw(sf::RenderWindow &w);
	void SetPlaying(bool p);
	bool GetPlaying();
	void SetPosition(sf::Vector2f pos);
	sf::Vector2f GetPosition();
	void SetGravity(sf::Vector2f grav);
	sf::Vector2f GetGravity();
	void ScaleDownModel();
	void ScaleUpModel();
	void SetVelocity(sf::Vector2f pos);
	bool LevelComplete();
private:
	bool playing;
	bool moving;
	bool grounded;
	bool jump;

	int maxSpeed;

	//float lastGroundTime;

	sf::Clock clock;
	sf::Time lastGroundTime;
	sf::Time timeNow;
	float stillTime;
	float playerHeight;
	float playerWidth;
	float playerFeetBaseSize = 18.0f;

	sf::Sprite playerSpr;
	sf::Texture playerTex;

	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f accel;
	sf::Vector2f velocity;
	sf::Vector2f desiredVel;
	sf::Vector2f gravity;
	

	sf::View* view1;

	LevelEditor* editor;
	MathMethods* math;
	b2Body* groundedBlock = NULL;

	b2World* world;
	b2BodyDef playerBodyDef;
	b2Body* dynBody = NULL;
	b2PolygonShape boxShape;
	b2Fixture* playerPhysicsFixture;
	b2Fixture* playerSensorFixture;
	b2ContactListener* listener;

	void MoveRight();
	void MoveLeft();
	void LimitSpeed();
	void Jump();
	bool isPlayerGrounded();
};