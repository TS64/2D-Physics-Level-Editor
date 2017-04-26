#include "Player.h"

Player::Player(sf::Vector2f spawn, sf::View* v, LevelEditor* e, MathMethods* m)
{
	position = spawn;
	view1 = v;
	math = m;
	editor = e;

	grounded = true;
	jump = false;

	lastGroundTime = clock.getElapsedTime();

	playerTex.loadFromFile("Textures/Elements/player.png");
	playerSpr.setTexture(playerTex);
	playerSpr.setPosition(position);
	playerSpr.setScale(1, 1.1f);
	playerSpr.setOrigin(27.0f / 2.0f, 50.0f / 2.0f);
	size = sf::Vector2f(27.0f / 2.0f, 50.0f / 2.0f);

	maxSpeed = 30;

	world = editor->GetWorld();

	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.SetZero();
	dynBody = world->CreateBody(&playerBodyDef);

	b2PolygonShape* poly = new b2PolygonShape();
	poly->SetAsBox(27.0f / 2.0f, 50.0f / 2.0f);
	playerPhysicsFixture = dynBody->CreateFixture(poly, 1);
	playerPhysicsFixture->SetDensity(0.001);
	playerPhysicsFixture->SetFriction(0);
	int isPlayer = 1;
	playerPhysicsFixture->SetUserData((void*)isPlayer);
	
	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = playerFeetBaseSize / 2.0f;
	circle->m_p = b2Vec2(0, 50.0f / 2.0f);
	playerSensorFixture = dynBody->CreateFixture(circle, 1);
	playerSensorFixture->SetDensity(0.001);
	playerSensorFixture->SetFriction(0);
	playerSensorFixture->SetUserData((void*)isPlayer);

	dynBody->SetFixedRotation(true);
	dynBody->ResetMassData();
	dynBody->SetTransform(dynBody->GetPosition(), 0);

	playerHeight = ((50.0f / 2.0f) / 2.0f) + ((playerFeetBaseSize / 2.0f) / 2.0f);
	playerWidth = 27.0f / 2.0f;

	gravity = sf::Vector2f(0, 9.81 * 8);
}

Player::~Player()
{
	if (dynBody)
	{
		dynBody->GetWorld()->DestroyBody(dynBody);
	}
}

void Player::Update(sf::RenderWindow &w, float dt)
{
	velocity = sf::Vector2f(dynBody->GetLinearVelocity().x, dynBody->GetLinearVelocity().y);
	position = sf::Vector2f(dynBody->GetPosition().x, dynBody->GetPosition().y);
	if (playing)
	{
		view1->setCenter(sf::Vector2f(position.x * math->SCALE, position.y * math->SCALE));
		editor->ApplyGravity();
		if (LevelComplete())
		{
			playing = false;
		}
		if (!grounded)
		{
			dynBody->ApplyForceToCenter(dynBody->GetMass() * b2Vec2(gravity.x, gravity.y), true);
		}
	}
	
	grounded = isPlayerGrounded();
	if (grounded)
	{
		lastGroundTime = clock.getElapsedTime();
	}

	if (abs(velocity.x) > maxSpeed)
	{
		dynBody->SetLinearVelocity(b2Vec2(velocity.x, dynBody->GetLinearVelocity().y));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && velocity.x > -maxSpeed)
	{
		//printf("|left");
		dynBody->ApplyLinearImpulseToCenter(b2Vec2(-0.04f, 0), true);
		playerSpr.setScale(-1, 1.1f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && velocity.x < maxSpeed)
	{
		//printf("|right");
		dynBody->ApplyLinearImpulseToCenter(b2Vec2(0.04f, 0), true);
		playerSpr.setScale(1, 1.1f);
	}

	Jump();

	if (jump)
	{
		jump = false;
		if (grounded)
		{
			dynBody->SetLinearVelocity(b2Vec2(velocity.x, 0));
			dynBody->SetTransform(b2Vec2(position.x, position.y - 0.01f), 0);
			dynBody->ApplyLinearImpulseToCenter(b2Vec2(0, -0.75f), true);
		}
	}
}

void Player::Jump()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!jump)
		{
			jump = true;
		}
	}
}

void Player::Draw(sf::RenderWindow &w)
{
	if (playing)
	{
		sf::Vector2f pos = sf::Vector2f(position.x * math->SCALE, position.y * math->SCALE);
		//sf::Vector2f pos = sf::Vector2f(position.x * 1, position.y * 1);
		//pos.x -= playerTex.getSize().x;
		pos.y += playerFeetBaseSize / 2.0f;
		//pos.y -= playerTex.getSize().y;
		playerSpr.setPosition(sf::Vector2f((int)pos.x, (int)pos.y));
		w.draw(playerSpr);
	}
}

void Player::SetPlaying(bool p)
{
	playing = p;
	if (p)
	{
		dynBody->SetLinearVelocity(b2Vec2_zero);
	}
}
bool Player::GetPlaying()
{
	return playing;
}
void Player::SetPosition(sf::Vector2f pos)
{
	position = pos;
	dynBody->SetTransform(b2Vec2(pos.x, pos.y), 0);
	dynBody->SetLinearVelocity(b2Vec2_zero);
	//cout << "Set pos: " << dynBody->GetPosition().x << "-" << dynBody->GetPosition().y << endl;
}
sf::Vector2f Player::GetPosition()
{
	//position = sf::Vector2f(dynBody->GetPosition().x, dynBody->GetPosition().y);
	return position;
}

bool Player::isPlayerGrounded()
{
	groundedBlock = NULL;
	b2Contact* contactList = world->GetContactList();
	for (b2Contact* contact = world->GetContactList(); contact != NULL; contact = contact->GetNext())
	{
		if (contact->IsTouching() && (contact->GetFixtureA() == playerSensorFixture ||
			contact->GetFixtureB() == playerSensorFixture))
		{
			b2Vec2 pos = dynBody->GetPosition();
			b2WorldManifold* manifold = new b2WorldManifold;
			contact->GetWorldManifold(manifold);

			bool below = true;
			float bottomOfPlayer = pos.y + (27.0f / 2.0f) + (50.0f / 2.0f) + 1.5f;
			
			below &= (manifold->points[0].y < bottomOfPlayer);

			if (below)
			{
				//contact->SetFriction(playerSensorFixture->GetFriction());
				if (contact->GetFixtureA()->GetUserData() == NULL &&
					contact->GetFixtureB()->GetUserData() != NULL)
				{
					groundedBlock = contact->GetFixtureA()->GetBody();
					contact->SetFriction(groundedBlock->GetFixtureList()->GetFriction());
				}
				if (contact->GetFixtureB()->GetUserData() == NULL &&
					contact->GetFixtureA()->GetUserData() != NULL)
				{
					groundedBlock = contact->GetFixtureB()->GetBody();
					contact->SetFriction(groundedBlock->GetFixtureList()->GetFriction());
				}
				return true;
			}
			contact->SetFriction(playerSensorFixture->GetFriction());
			
			return false;
		}
	}
	return false;
}

void Player::ScaleDownModel()
{
	//cout << "old pos: " << dynBody->GetPosition().x << "-" << dynBody->GetPosition().y << endl;
	dynBody->DestroyFixture(playerPhysicsFixture);
	dynBody->DestroyFixture(playerSensorFixture);
	

	b2PolygonShape* poly = new b2PolygonShape();
	poly->SetAsBox((27.0f / 2.0f) / math->SCALE, (50.0f / 2.0f) / math->SCALE);
	playerPhysicsFixture = dynBody->CreateFixture(poly, 1);
	playerPhysicsFixture->SetDensity(0.001);
	playerPhysicsFixture->SetFriction(0);
	int isPlayer = 1;
	playerPhysicsFixture->SetUserData((void*)isPlayer);

	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = (playerFeetBaseSize / 2.0f) / math->SCALE;
	circle->m_p = b2Vec2(0, (50.0f / 2.0f) / math->SCALE);
	playerSensorFixture = dynBody->CreateFixture(circle, 1);
	playerSensorFixture->SetDensity(0.001);
	playerSensorFixture->SetFriction(0);
	playerSensorFixture->SetUserData((void*)isPlayer);

	dynBody->SetFixedRotation(true);
	dynBody->ResetMassData();

	playerHeight = ((50.0f / 2.0f) / math->SCALE) + ((playerFeetBaseSize / 2.0f) / math->SCALE);
	playerWidth = (27.0f / 2.0f) / math->SCALE;
	dynBody->SetTransform(b2Vec2(dynBody->GetPosition().x / math->SCALE, dynBody->GetPosition().y / math->SCALE), 0);
	//cout << "new pos: " << dynBody->GetPosition().x << "-" << dynBody->GetPosition().y << endl;
}

void Player::ScaleUpModel()
{
	dynBody->DestroyFixture(playerPhysicsFixture);
	dynBody->DestroyFixture(playerSensorFixture);

	b2PolygonShape* poly = new b2PolygonShape();
	poly->SetAsBox((27.0f / 2.0f) / 1.0f, (50.0f / 2.0f) / 1.0f);
	playerPhysicsFixture = dynBody->CreateFixture(poly, 1);
	playerPhysicsFixture->SetDensity(0.001);
	playerPhysicsFixture->SetFriction(0);
	int isPlayer = 1;
	playerPhysicsFixture->SetUserData((void*)isPlayer);

	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = (playerFeetBaseSize / 2.0f) / 1.0f;
	circle->m_p = b2Vec2(0, (50.0f / 2.0f) / 1.0f);
	playerSensorFixture = dynBody->CreateFixture(circle, 1);
	playerSensorFixture->SetDensity(0.001);
	playerSensorFixture->SetFriction(0);
	playerSensorFixture->SetUserData((void*)isPlayer);

	dynBody->SetFixedRotation(true);
	dynBody->ResetMassData();

	playerHeight = ((50.0f / 2.0f) / 1.0f) + ((playerFeetBaseSize / 2.0f) / 1.0f);
	playerWidth = 27.0f / 2.0f;
	dynBody->SetTransform(b2Vec2(dynBody->GetPosition().x * math->SCALE, dynBody->GetPosition().y * math->SCALE), 0);
	//cout << "height:" << playerHeight << endl;
}

void Player::SetVelocity(sf::Vector2f pos)
{
	dynBody->SetLinearVelocity(b2Vec2(pos.x, pos.y));
}

void Player::SetGravity(sf::Vector2f grav)
{
	gravity = grav;
}
sf::Vector2f Player::GetGravity()
{
	return gravity;
}

bool Player::LevelComplete()
{
	/*cout << "Position: " << (int)position.x * math->SCALE << "-" << (int)position.y * math->SCALE
		<< "|Finish: " << editor->GetLevelFinish().x << "-" << editor->GetLevelFinish().y << endl;*/
	if (position.x * math->SCALE >= editor->GetLevelFinish().x && position.x * math->SCALE <= editor->GetLevelFinish().x + 50 &&
		position.y * math->SCALE >= editor->GetLevelFinish().y && position.y * math->SCALE <= editor->GetLevelFinish().y + 50)
	{
		printf("FINISH\n");
		return true;
	}
	return false;
}