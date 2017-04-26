#pragma once
#include <Box2D\Box2D.h>
#include <iostream>
#include "SFML\Graphics.hpp"

struct ObjectData
{
	int type;
	int mass;
	float density;
};

class Block

{
public:
	virtual ~Block() {
		//std::cout << "Deleted block" << std::endl; 
	}
	virtual void Draw(sf::RenderWindow &w, bool c) = 0;
	virtual void AlphaDraw(sf::RenderWindow &w) = 0;
	virtual void CameraView() = 0;
	virtual void Resize(sf::Vector2f s) = 0;
	virtual sf::Vector2f GetWorldPosition() = 0;
	virtual sf::Vector2f GetScreenPosition() = 0;
	virtual void SetPosition(sf::Vector2f pos) = 0;
	virtual void Move(sf::Vector2f pos) = 0;
	virtual void Rotate(float a) = 0;
	virtual void ResetRotation() = 0;
	virtual float GetRotation() = 0;
	virtual void SetRotation(float r) = 0;
	virtual sf::Vector2f GetSize() = 0;
	virtual int BlockType() = 0;
	virtual bool Valid() = 0;
	virtual b2Body* GetBody() = 0;
	virtual b2BodyDef GetBodyDef() = 0;
	virtual bool GetKine() = 0;
	virtual void SetGravity(sf::Vector2f grav) = 0;
	virtual sf::Vector2f GetGravity() = 0;
protected:
	Block()
	{
		//std::cout << "Created block" << std::endl;
	}
};

// ID = 0
class GrassBlock : public Block
{
public:
	GrassBlock(sf::Vector2f pos, sf::Vector2f s, float r, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		if (s.x != 0 && s.y != 0)
		{
			rotation = r;
			valid = true;
			if (s.y > spriteHeight)
			{
				topSize = sf::Vector2i(s.x, spriteHeight);
				topPosition = (sf::Vector2i)pos;

				bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
				bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
			}
			else if (s.y <= spriteHeight)
			{
				topSize = (sf::Vector2i)s;
				topPosition = (sf::Vector2i)pos;
			}
			topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
			bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);

			grassTopTex.loadFromFile("Textures/Elements/groundTop.png");
			grassTopSpr.setTexture(grassTopTex);
			grassUnderTex.loadFromFile("Textures/Elements/groundUnder.png");
			grassUnderSpr.setTexture(grassUnderTex);

			grassTopTex.setRepeated(true);
			grassTopSpr.setPosition((sf::Vector2f)topPosition + (sf::Vector2f)topCenterPos);
			grassTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
			grassTopSpr.setRotation(r);
			grassTopSpr.setOrigin(topCenterPos);

			grassUnderTex.setRepeated(true);
			grassUnderSpr.setPosition((sf::Vector2f)bottomPosition + (sf::Vector2f)bottomCenterPos);
			grassUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
			grassUnderSpr.setRotation(r);
			grassUnderSpr.setOrigin(bottomCenterPos);

			if (kine)
			{
				grassBodyDef.type = b2_kinematicBody;
			}
			if (!kine)
			{
				grassBodyDef.type = b2_dynamicBody;
			}
			//grassBodyDef.position.Set(pos.x + (s.x / 2), pos.y + (s.y / 2));
			kineBody = world.CreateBody(&grassBodyDef);
			boxShape.SetAsBox((s.x / 2), (s.y / 2), b2Vec2_zero, 0);

			boxFixtureDef.shape = &boxShape;
			//boxFixtureDef.density = data->density;
			boxFixtureDef.friction = friction;
			boxFixtureDef.restitution = bounce;
			boxFixtureDef.density = density;
			boxFixtureDef.filter.categoryBits = categoryBits;
			boxFixtureDef.filter.maskBits = maskBits;
			kineBody->SetFixedRotation(false);
			kineBody->CreateFixture(&boxFixtureDef);
			kineBody->SetTransform(kineBody->GetPosition(), 0);

			//data->type = 1;
			grassTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
			grassUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
			kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
				topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));

			kineBody->SetUserData(data);
		}
	}
	~GrassBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed grass block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		kineBody->SetFixedRotation(false);
		if (playing)
		{
			grassTopSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			grassUnderSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			grassTopSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
			grassUnderSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
		}
		w.draw(grassTopSpr);
		w.draw(grassUnderSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		tempTexTop.loadFromFile("Textures/Elements/groundTop.png");
		tempTexTop.setRepeated(true);
		tempTop.setTexture(tempTexTop);

		tempTop.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos);
		tempTop.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
		tempTop.setColor(sf::Color(255, 255, 255, 127));

		tempTexBottom.loadFromFile("Textures/Elements/groundUnder.png");
		tempTexBottom.setRepeated(true);
		tempBottom.setTexture(tempTexBottom);

		tempBottom.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos);
		tempBottom.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
		tempBottom.setColor(sf::Color(255, 255, 255, 127));

		w.draw(tempTop);
		w.draw(tempBottom);

	}
	virtual void CameraView()
	{
	}
	virtual void Resize(sf::Vector2f s)
	{
		if (s.y > spriteHeight)
		{
			topSize = sf::Vector2i(s.x, spriteHeight);
			bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
		}
		else if (s.y <= spriteHeight)
		{
			topSize = (sf::Vector2i)s;
			bottomSize = sf::Vector2i(0, 0);
		}
		topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
		grassTopSpr.setOrigin(topCenterPos);
		grassTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));

		bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);
		grassUnderSpr.setOrigin(bottomCenterPos);
		grassUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return (sf::Vector2f)topPosition;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return grassTopSpr.getPosition() - topCenterPos;
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		topPosition = (sf::Vector2i)pos;
		bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
		grassTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		grassUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		topPosition += (sf::Vector2i)pos;
		bottomPosition += (sf::Vector2i)pos;
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		grassTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		grassUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);

		kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
			topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(topSize.x, topSize.y + bottomSize.y);
	}
	virtual void Rotate(float a)
	{
		grassTopSpr.rotate(a);
		grassUnderSpr.rotate(a);
		rotation = grassTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		grassTopSpr.setRotation(r);
		grassUnderSpr.setRotation(r);
		rotation = grassTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14 / 180.0f));
	}
	virtual void ResetRotation()
	{
		grassTopSpr.setRotation(0);
		grassUnderSpr.setRotation(0);
	}
	virtual int BlockType()
	{
		return 0;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return grassBodyDef;
	}
	virtual bool GetKine()
	{
		if (grassBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2i topPosition;
	sf::Vector2i bottomPosition;
	sf::Vector2i topSize;
	sf::Vector2i bottomSize;
	sf::Vector2i cameraPos;

	sf::Vector2f topCenterPos;
	sf::Vector2f bottomCenterPos;
	sf::Sprite grassTopSpr;
	sf::Sprite grassUnderSpr;
	sf::Sprite tempTop;
	sf::Sprite tempBottom;
	sf::Texture grassTopTex;
	sf::Texture grassUnderTex;
	sf::Texture tempTexTop;
	sf::Texture tempTexBottom;
	bool valid = false;
	int id = 1;

	float spriteHeight = 50;
	float rotation = 0;

	sf::Vector2f gravity;
	b2BodyDef grassBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape boxShape;
	b2FixtureDef boxFixtureDef;
};

// ID = 1
class IceBlock : public Block
{
public:
	IceBlock(sf::Vector2f pos, sf::Vector2f s, float r, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		if (s.x != 0 && s.y != 0)
		{
			rotation = r;
			valid = true;
			if (s.y > spriteHeight)
			{
				topSize = sf::Vector2i(s.x, spriteHeight);
				topPosition = (sf::Vector2i)pos;

				bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
				bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
			}
			else if (s.y <= spriteHeight)
			{
				topSize = (sf::Vector2i)s;
				topPosition = (sf::Vector2i)pos;
			}
			topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
			bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);

			iceTopTex.loadFromFile("Textures/Elements/iceTop.png");
			iceTopSpr.setTexture(iceTopTex);
			iceUnderTex.loadFromFile("Textures/Elements/iceUnder.png");
			iceUnderSpr.setTexture(iceUnderTex);

			iceTopTex.setRepeated(true);
			iceTopSpr.setPosition((sf::Vector2f)topPosition + (sf::Vector2f)topCenterPos);
			iceTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
			iceTopSpr.setRotation(r);
			iceTopSpr.setOrigin(topCenterPos);

			iceUnderTex.setRepeated(true);
			iceUnderSpr.setPosition((sf::Vector2f)bottomPosition + (sf::Vector2f)bottomCenterPos);
			iceUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
			iceUnderSpr.setRotation(r);
			iceUnderSpr.setOrigin(bottomCenterPos);

			if (kine)
			{
				iceBodyDef.type = b2_kinematicBody;
			}
			if (!kine)
			{
				iceBodyDef.type = b2_dynamicBody;
			}
			//grassBodyDef.position.Set(pos.x + (s.x / 2), pos.y + (s.y / 2));
			kineBody = world.CreateBody(&iceBodyDef);
			boxShape.SetAsBox((s.x / 2), (s.y / 2), b2Vec2_zero, 0);

			boxFixtureDef.shape = &boxShape;
			//boxFixtureDef.density = data->density;
			boxFixtureDef.friction = friction;
			boxFixtureDef.restitution = bounce;
			boxFixtureDef.density = density;
			boxFixtureDef.filter.categoryBits = categoryBits;
			boxFixtureDef.filter.maskBits = maskBits;
			kineBody->SetFixedRotation(false);
			kineBody->CreateFixture(&boxFixtureDef);
			kineBody->SetTransform(kineBody->GetPosition(), 0);

			//data->type = 1;
			iceTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
			iceUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
			kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
				topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));

			kineBody->SetUserData(data);
		}
	}
	~IceBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed ice block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		if (playing)
		{
			iceTopSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			iceUnderSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			iceTopSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
			iceUnderSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
		}
		w.draw(iceTopSpr);
		w.draw(iceUnderSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		tempTexTop.loadFromFile("Textures/Elements/iceTop.png");
		tempTexTop.setRepeated(true);
		tempTop.setTexture(tempTexTop);

		tempTop.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos);
		tempTop.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
		tempTop.setColor(sf::Color(255, 255, 255, 127));

		tempTexBottom.loadFromFile("Textures/Elements/iceUnder.png");
		tempTexBottom.setRepeated(true);
		tempBottom.setTexture(tempTexBottom);

		tempBottom.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos);
		tempBottom.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
		tempBottom.setColor(sf::Color(255, 255, 255, 127));

		w.draw(tempTop);
		w.draw(tempBottom);

	}
	virtual void CameraView()
	{

	}
	virtual void Resize(sf::Vector2f s)
	{
		if (s.y > spriteHeight)
		{
			topSize = sf::Vector2i(s.x, spriteHeight);
			bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
		}
		else if (s.y <= spriteHeight)
		{
			topSize = (sf::Vector2i)s;
			bottomSize = sf::Vector2i(0, 0);
		}
		topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
		iceTopSpr.setOrigin(topCenterPos);
		iceTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));

		bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);
		iceUnderSpr.setOrigin(bottomCenterPos);
		iceUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return (sf::Vector2f)topPosition;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return iceTopSpr.getPosition() - topCenterPos;
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		topPosition = (sf::Vector2i)pos;
		bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
		iceTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		iceUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		topPosition += (sf::Vector2i)pos;
		bottomPosition += (sf::Vector2i)pos;
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		iceTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		iceUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);

		kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
			topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(topSize.x, topSize.y + bottomSize.y);
	}
	virtual void Rotate(float a)
	{
		iceTopSpr.rotate(a);
		iceUnderSpr.rotate(a);
		rotation = iceTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		iceTopSpr.setRotation(r);
		iceUnderSpr.setRotation(r);
		rotation = iceTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14 / 180.0f));
	}
	virtual void ResetRotation()
	{
		iceTopSpr.setRotation(0);
		iceUnderSpr.setRotation(0);
	}
	virtual int BlockType()
	{
		return 1;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return iceBodyDef;
	}
	virtual bool GetKine()
	{
		if (iceBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2i topPosition;
	sf::Vector2i bottomPosition;
	sf::Vector2i topSize;
	sf::Vector2i bottomSize;
	sf::Vector2i cameraPos;

	sf::Vector2f topCenterPos;
	sf::Vector2f bottomCenterPos;
	sf::Sprite iceTopSpr;
	sf::Sprite iceUnderSpr;
	sf::Sprite tempTop;
	sf::Sprite tempBottom;
	sf::Texture iceTopTex;
	sf::Texture iceUnderTex;
	sf::Texture tempTexTop;
	sf::Texture tempTexBottom;
	bool valid = false;
	int id = 1;

	float spriteHeight = 32;
	float rotation = 0;

	sf::Vector2f gravity;
	b2BodyDef iceBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape boxShape;
	b2FixtureDef boxFixtureDef;
};

// ID = 2
class WaterBlock : public Block
{
public:
	WaterBlock(sf::Vector2f pos, sf::Vector2f s, float r, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		if (s.x != 0 && s.y != 0)
		{
			rotation = r;
			valid = true;
			if (s.y > spriteHeight)
			{
				topSize = sf::Vector2i(s.x, spriteHeight);
				topPosition = (sf::Vector2i)pos;

				bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
				bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
			}
			else if (s.y <= spriteHeight)
			{
				topSize = (sf::Vector2i)s;
				topPosition = (sf::Vector2i)pos;
			}
			topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
			bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);

			waterTopTex.loadFromFile("Textures/Elements/waterTop.png");
			waterTopSpr.setTexture(waterTopTex);
			waterUnderTex.loadFromFile("Textures/Elements/waterUnder.png");
			waterUnderSpr.setTexture(waterUnderTex);

			waterTopTex.setRepeated(true);
			waterTopSpr.setPosition((sf::Vector2f)topPosition + topCenterPos);
			waterTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
			waterTopSpr.setRotation(r);
			waterTopSpr.setOrigin(topCenterPos);

			waterUnderTex.setRepeated(true);
			waterUnderSpr.setPosition((sf::Vector2f)bottomPosition + bottomCenterPos);
			waterUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
			waterUnderSpr.setRotation(r);
			waterUnderSpr.setOrigin(bottomCenterPos);

			if (kine)
			{
				waterBodyDef.type = b2_kinematicBody;
			}
			if (!kine)
			{
				waterBodyDef.type = b2_dynamicBody;
			}
			//grassBodyDef.position.Set(pos.x + (s.x / 2), pos.y + (s.y / 2));
			kineBody = world.CreateBody(&waterBodyDef);
			boxShape.SetAsBox((s.x / 2), (s.y / 2), b2Vec2_zero, 0);

			boxFixtureDef.shape = &boxShape;
			//boxFixtureDef.density = data->density;
			boxFixtureDef.friction = friction;
			boxFixtureDef.restitution = bounce;
			boxFixtureDef.density = density;
			boxFixtureDef.filter.categoryBits = categoryBits;
			boxFixtureDef.filter.maskBits = maskBits;
			kineBody->SetFixedRotation(false);
			kineBody->CreateFixture(&boxFixtureDef);
			kineBody->SetTransform(kineBody->GetPosition(), 0);

			//data->type = 2;
			waterTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
			waterUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
			kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
				topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));

			kineBody->SetUserData(data);
		}
	}
	~WaterBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed water block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		if (playing)
		{
			waterTopSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			waterUnderSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			waterTopSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
			waterUnderSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
		}
		w.draw(waterTopSpr);
		w.draw(waterUnderSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		tempTexTop.loadFromFile("Textures/Elements/waterTop.png");
		tempTexTop.setRepeated(true);
		tempTop.setTexture(tempTexTop);

		tempTop.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos);
		tempTop.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
		tempTop.setColor(sf::Color(255, 255, 255, 127));

		tempTexBottom.loadFromFile("Textures/Elements/waterUnder.png");
		tempTexBottom.setRepeated(true);
		tempBottom.setTexture(tempTexBottom);

		tempBottom.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos);
		tempBottom.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
		tempBottom.setColor(sf::Color(255, 255, 255, 127));

		w.draw(tempTop);
		w.draw(tempBottom);

	}
	virtual void CameraView()
	{
	}
	virtual void Resize(sf::Vector2f s)
	{
		if (s.y > spriteHeight)
		{
			topSize = sf::Vector2i(s.x, spriteHeight);
			bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
		}
		else if (s.y <= spriteHeight)
		{
			topSize = (sf::Vector2i)s;
			bottomSize = sf::Vector2i(0, 0);
		}
		topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
		waterTopSpr.setOrigin(topCenterPos);
		waterTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));

		bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);
		waterUnderSpr.setOrigin(bottomCenterPos);
		waterUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return (sf::Vector2f)topPosition;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return waterTopSpr.getPosition() - topCenterPos;
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		topPosition = (sf::Vector2i)pos;
		bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
		waterTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		waterUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		topPosition += (sf::Vector2i)pos;
		bottomPosition += (sf::Vector2i)pos;
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		waterTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		waterUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);

		kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
			topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(topSize.x, topSize.y + bottomSize.y);
	}
	virtual void Rotate(float a)
	{
		waterTopSpr.rotate(a);
		waterUnderSpr.rotate(a);
		rotation = waterTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		waterTopSpr.setRotation(r);
		waterUnderSpr.setRotation(r);
		rotation = waterTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14 / 180.0f));
	}
	virtual void ResetRotation()
	{
		waterTopSpr.setRotation(0);
		waterUnderSpr.setRotation(0);
	}
	virtual int BlockType()
	{
		return 2;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return waterBodyDef;
	}
	virtual bool GetKine()
	{
		if (waterBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2i topPosition;
	sf::Vector2i bottomPosition;
	sf::Vector2i topSize;
	sf::Vector2i bottomSize;
	sf::Vector2i cameraPos;

	sf::Vector2f topCenterPos;
	sf::Vector2f bottomCenterPos;
	sf::Sprite waterTopSpr;
	sf::Sprite waterUnderSpr;
	sf::Sprite tempTop;
	sf::Sprite tempBottom;
	sf::Texture waterTopTex;
	sf::Texture waterUnderTex;
	sf::Texture tempTexTop;
	sf::Texture tempTexBottom;
	bool valid = false;
	int id = 1;

	float spriteHeight = 32;
	float rotation = 0;

	sf::Vector2f gravity;
	b2BodyDef waterBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape boxShape;
	b2FixtureDef boxFixtureDef;
};

// ID = 3
class MetalBlock : public Block
{
public:
	MetalBlock(sf::Vector2f pos, sf::Vector2f s, float r, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		if (s.x != 0 && s.y != 0)
		{
			rotation = r;
			valid = true;
			if (s.y > spriteHeight)
			{
				topSize = sf::Vector2i(s.x, spriteHeight);
				topPosition = (sf::Vector2i)pos;

				bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
				bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
			}
			else if (s.y <= spriteHeight)
			{
				topSize = (sf::Vector2i)s;
				topPosition = (sf::Vector2i)pos;
			}
			topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
			bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);

			metalTopTex.loadFromFile("Textures/Elements/metalTop.png");
			metalTopSpr.setTexture(metalTopTex);
			metalUnderTex.loadFromFile("Textures/Elements/metalUnder.png");
			metalUnderSpr.setTexture(metalUnderTex);

			metalTopTex.setRepeated(true);
			metalTopSpr.setPosition((sf::Vector2f)topPosition + (sf::Vector2f)topCenterPos);
			metalTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
			metalTopSpr.setRotation(r);
			metalTopSpr.setOrigin(topCenterPos);

			metalUnderTex.setRepeated(true);
			metalUnderSpr.setPosition((sf::Vector2f)bottomPosition + (sf::Vector2f)bottomCenterPos);
			metalUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
			metalUnderSpr.setRotation(r);
			metalUnderSpr.setOrigin(bottomCenterPos);

			if (kine)
			{
				metalBodyDef.type = b2_kinematicBody;
			}
			if (!kine)
			{
				metalBodyDef.type = b2_dynamicBody;
			}
			//grassBodyDef.position.Set(pos.x + (s.x / 2), pos.y + (s.y / 2));
			kineBody = world.CreateBody(&metalBodyDef);
			boxShape.SetAsBox((s.x / 2), (s.y / 2), b2Vec2_zero, 0);

			boxFixtureDef.shape = &boxShape;
			//boxFixtureDef.density = data->density;
			boxFixtureDef.friction = friction;
			boxFixtureDef.restitution = bounce;
			boxFixtureDef.density = density;
			boxFixtureDef.filter.categoryBits = categoryBits;
			boxFixtureDef.filter.maskBits = maskBits;
			kineBody->SetFixedRotation(false);
			kineBody->CreateFixture(&boxFixtureDef);
			kineBody->SetTransform(kineBody->GetPosition(), 0);

			//data->type = 1;
			metalTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
			metalUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
			kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
				topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));

			kineBody->SetUserData(data);
		}
	}
	~MetalBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed metal block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		if (playing)
		{
			metalTopSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			metalUnderSpr.setPosition(sf::Vector2f(kineBody->GetPosition().x * 10, kineBody->GetPosition().y * 10));
			metalTopSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
			metalUnderSpr.setRotation(kineBody->GetAngle() * (180 / 3.14f));
		}
		w.draw(metalTopSpr);
		w.draw(metalUnderSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		tempTexTop.loadFromFile("Textures/Elements/metalTop.png");
		tempTexTop.setRepeated(true);
		tempTop.setTexture(tempTexTop);

		tempTop.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos);
		tempTop.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));
		tempTop.setColor(sf::Color(255, 255, 255, 127));

		tempTexBottom.loadFromFile("Textures/Elements/metalUnder.png");
		tempTexBottom.setRepeated(true);
		tempBottom.setTexture(tempTexBottom);

		tempBottom.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos);
		tempBottom.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
		tempBottom.setColor(sf::Color(255, 255, 255, 127));

		w.draw(tempTop);
		w.draw(tempBottom);

	}
	virtual void CameraView()
	{
	}
	virtual void Resize(sf::Vector2f s)
	{
		if (s.y > spriteHeight)
		{
			topSize = sf::Vector2i(s.x, spriteHeight);
			bottomSize = sf::Vector2i(s.x, s.y - spriteHeight);
		}
		else if (s.y <= spriteHeight)
		{
			topSize = (sf::Vector2i)s;
			bottomSize = sf::Vector2i(0, 0);
		}
		topCenterPos = sf::Vector2f(s.x / 2.0f, s.y / 2.0f);
		metalTopSpr.setOrigin(topCenterPos);
		metalTopSpr.setTextureRect(sf::IntRect(0, 0, (int)topSize.x, (int)topSize.y));

		bottomCenterPos = sf::Vector2f(s.x / 2.0f, (s.y / 2.0f) - spriteHeight);
		metalUnderSpr.setOrigin(bottomCenterPos);
		metalUnderSpr.setTextureRect(sf::IntRect(0, 0, (int)bottomSize.x, (int)bottomSize.y));
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return (sf::Vector2f)topPosition;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return metalTopSpr.getPosition() - topCenterPos;
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		topPosition = (sf::Vector2i)pos;
		bottomPosition = sf::Vector2i(pos.x, pos.y + spriteHeight);
		metalTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		metalUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		topPosition += (sf::Vector2i)pos;
		bottomPosition += (sf::Vector2i)pos;
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		metalTopSpr.setPosition((sf::Vector2f)topPosition - (sf::Vector2f)cameraPos + topCenterPos);
		metalUnderSpr.setPosition((sf::Vector2f)bottomPosition - (sf::Vector2f)cameraPos + bottomCenterPos);

		kineBody->SetTransform(b2Vec2(topPosition.x - cameraPos.x + (GetSize().x / 2),
			topPosition.y - cameraPos.y + (GetSize().y / 2)), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(topSize.x, topSize.y + bottomSize.y);
	}
	virtual void Rotate(float a)
	{
		metalTopSpr.rotate(a);
		metalUnderSpr.rotate(a);
		rotation = metalTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		metalTopSpr.setRotation(r);
		metalUnderSpr.setRotation(r);
		rotation = metalTopSpr.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14 / 180.0f));
	}
	virtual void ResetRotation()
	{
		metalTopSpr.setRotation(0);
		metalUnderSpr.setRotation(0);
	}
	virtual int BlockType()
	{
		return 3;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return metalBodyDef;
	}
	virtual bool GetKine()
	{
		if (metalBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2i topPosition;
	sf::Vector2i bottomPosition;
	sf::Vector2i topSize;
	sf::Vector2i bottomSize;
	sf::Vector2i cameraPos;

	sf::Vector2f topCenterPos;
	sf::Vector2f bottomCenterPos;
	sf::Sprite metalTopSpr;
	sf::Sprite metalUnderSpr;
	sf::Sprite tempTop;
	sf::Sprite tempBottom;
	sf::Texture metalTopTex;
	sf::Texture metalUnderTex;
	sf::Texture tempTexTop;
	sf::Texture tempTexBottom;
	bool valid = false;
	int id = 1;

	float spriteHeight = 31;
	float rotation = 0;

	sf::Vector2f gravity;
	b2BodyDef metalBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape boxShape;
	b2FixtureDef boxFixtureDef;
};

// ID = 4
class SpawnBlock : public Block
{
public:
	SpawnBlock(sf::Vector2f pos)
	{
		position = pos;
		spawnTex.loadFromFile("Textures/Elements/spawnPoint.png");
		spawnSpr.setTexture(spawnTex);
		spawnSpr.setOrigin(25, 25);
		spawnSpr.setPosition(position);
	}
	~SpawnBlock()
	{

	}
	virtual void Draw(sf::RenderWindow &w, bool c)
	{
		w.draw(spawnSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w) {}
	virtual void CameraView()
	{
		spawnSpr.setPosition(position - cameraPos);
	}
	virtual void Resize(sf::Vector2f s) {}
	virtual sf::Vector2f GetWorldPosition()
	{
		return position;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return spawnSpr.getPosition();
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		position = pos;
		spawnSpr.setPosition(position - cameraPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		position += pos;
		spawnSpr.setPosition(position - cameraPos);
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(50, 50);
	}
	virtual void Rotate(float a) {}
	virtual float GetRotation()
	{
		return 0;
	}
	virtual void SetRotation(float r)
	{

	}
	virtual void ResetRotation() {}
	virtual int BlockType()
	{
		return 4;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return NULL;
	}
	virtual b2BodyDef GetBodyDef()
	{
		b2BodyDef empty;
		return empty;
	}
	virtual float GetDist()
	{
		return 0;
	}
	virtual bool GetKine() { return true; }
	virtual void SetGravity(sf::Vector2f grav) {}
	virtual sf::Vector2f GetGravity()
	{
		return sf::Vector2f(0, 0);
	}
private:
	sf::Vector2f position;
	sf::Vector2f cameraPos;
	sf::Sprite spawnSpr;
	sf::Texture spawnTex;

	bool valid = false;
	int id = 2;
	float rotation = 0;

	b2BodyDef grassBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape boxShape;
	b2FixtureDef boxFixtureDef;
	b2World * world;
};

// ID = 5
class FinishBlock : public Block
{
public:
	FinishBlock(sf::Vector2f pos)
	{
		position = pos;
		finishTex.loadFromFile("Textures/Elements/finishPoint.png");
		finishSpr.setTexture(finishTex);
		finishSpr.setOrigin(25, 25);
		finishSpr.setPosition(position);
	}
	~FinishBlock()
	{

	}
	virtual void Draw(sf::RenderWindow &w, bool c)
	{
		w.draw(finishSpr);
	}
	virtual void AlphaDraw(sf::RenderWindow &w) {}
	virtual void CameraView()
	{
		finishSpr.setPosition(position - cameraPos);
	}
	virtual void Resize(sf::Vector2f s) {}
	virtual sf::Vector2f GetWorldPosition()
	{
		return position;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return finishSpr.getPosition();
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		position = pos;
		finishSpr.setPosition(position - cameraPos);
	}
	virtual void Move(sf::Vector2f pos)
	{
		position += pos;
		finishSpr.setPosition(position - cameraPos);
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(50, 50);
	}
	virtual void Rotate(float a) {}
	virtual float GetRotation()
	{
		return 0;
	}
	virtual void SetRotation(float r)
	{

	}
	virtual void ResetRotation() {}
	virtual int BlockType()
	{
		return 5;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return NULL;
	}
	virtual b2BodyDef GetBodyDef()
	{
		b2BodyDef empty;
		return empty;
	}
	virtual float GetDist()
	{
		return 0;
	}
	virtual bool GetKine() { return true; }
	virtual void SetGravity(sf::Vector2f grav)
	{

	}
	virtual sf::Vector2f GetGravity()
	{
		return sf::Vector2f(0, 0);
	}
private:
	sf::Vector2f position;
	sf::Vector2f cameraPos;
	sf::Sprite finishSpr;
	sf::Texture finishTex;

	bool valid = false;
	int id = 3;
	float rotation = 0;
};

// ID = 6
class CircleBlock : public Block
{
public:
	CircleBlock(sf::Vector2f pos, float r, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		radius = r;
		//std::cout << "radius: " << radius << std::endl;
		valid = true;
		if (radius < 1)
		{
			printf("Invalid radius\n");
			valid = false;
		}
		centerPos = pos;
		position = centerPos - sf::Vector2f(radius, radius);


		circleTex.loadFromFile("Textures/Elements/rockBall.png");
		circleTex.setSmooth(true);
		circleShape.setTexture(&circleTex);
		circleShape.setRadius(radius);
		//circleShape.setOrigin(radius, radius);

		//circleShape.setTextureRect(sf::IntRect(0, 0, 100, 100));
		circleShape.setPosition(position);

		if (kine)
		{
			circleBodyDef.type = b2_kinematicBody;
		}
		if (!kine)
		{
			circleBodyDef.type = b2_dynamicBody;
		}
		//grassBodyDef.position.Set(pos.x + (s.x / 2), pos.y + (s.y / 2));
		kineBody = world.CreateBody(&circleBodyDef);
		boxShape = new b2CircleShape();
		boxShape->m_radius = radius;

		circleFixtureDef.shape = boxShape;
		circleFixtureDef.friction = friction;
		circleFixtureDef.restitution = bounce;
		circleFixtureDef.density = density;
		circleFixtureDef.filter.categoryBits = categoryBits;
		circleFixtureDef.filter.maskBits = maskBits;
		kineBody->SetFixedRotation(false);
		kineBody->CreateFixture(&circleFixtureDef);
		kineBody->SetTransform(kineBody->GetPosition(), 0);

		//data->type = 1;
		//circleShape.setPosition(position);
		kineBody->SetTransform(b2Vec2(centerPos.x,
			centerPos.y), rotation * (3.14f / 180));

		kineBody->SetUserData(data);
	}
	~CircleBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed circle block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		kineBody->SetFixedRotation(false);
		if (playing)
		{
			circleShape.setPosition(sf::Vector2f(((kineBody->GetPosition().x * 10) - radius), ((kineBody->GetPosition().y * 10) - radius)));
		}
		//std::cout << "angle: " << kineBody->GetAngle() << std::endl;
		std::cout << circleShape.getPosition().x << "-" << circleShape.getPosition().y << std::endl;
		w.draw(circleShape);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		circleTex.loadFromFile("Textures/Elements/rockBall.png");
		circleShape.setTexture(&circleTex);
		circleShape.setFillColor(sf::Color(100, 100, 100, 127));
		circleShape.setPosition(centerPos - sf::Vector2f(radius, radius));

		w.draw(circleShape);
	}
	virtual void CameraView()
	{
	}
	virtual void Resize(sf::Vector2f s)
	{
		radius = s.x;
		circleShape.setRadius(radius);
		position = centerPos - sf::Vector2f(radius, radius);
		//circleShape.setOrigin(s.x / 2.0f, s.x / 2.0f);
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return centerPos;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return centerPos;// -sf::Vector2f(radius, radius);
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		centerPos = pos;
		position = centerPos - sf::Vector2f(radius, radius);
		circleShape.setPosition(position);
		kineBody->SetTransform(b2Vec2(centerPos.x,
			centerPos.y), rotation * (3.14f / 180));
	}
	virtual void Move(sf::Vector2f pos)
	{
		centerPos += pos;
		position = centerPos - sf::Vector2f(radius, radius);
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		circleShape.setPosition(position);

		kineBody->SetTransform(b2Vec2(centerPos.x,
			centerPos.y), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(radius, radius);
		//centerPos = 
	}
	virtual void Rotate(float a)
	{
		circleShape.rotate(a);
		rotation = circleShape.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		circleShape.setRotation(r);
		rotation = circleShape.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14 / 180.0f));
	}
	virtual void ResetRotation()
	{

	}
	virtual int BlockType()
	{
		return 6;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return circleBodyDef;
	}
	virtual bool GetKine()
	{
		if (circleBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2f centerPos;
	sf::Vector2f position;
	sf::Vector2i cameraPos;

	sf::CircleShape circleShape;
	sf::Texture circleTex;

	bool valid = false;
	int id = 1;

	float rotation = 0;
	float radius;

	sf::Vector2f gravity;
	b2BodyDef circleBodyDef;
	b2Body* kineBody = NULL;
	b2CircleShape* boxShape;
	b2FixtureDef circleFixtureDef;
};

// ID = 7
class LineBlock : public Block
{
public:
	LineBlock(sf::Vector2f pos, float l, b2World& world, ObjectData* data,
		uint16 categoryBits, uint16 maskBits, float friction, float bounce, float density, bool kine)
	{
		length = l;
		valid = true;
		if (length < 3)
		{
			valid = false;
		}
		centerPos = pos;
		position = centerPos - sf::Vector2f(length / 2.0f, length / 2.0f);

		lineTex.loadFromFile("Textures/Elements/girderLine.png");
		lineTex.setSmooth(true);
		lineShape.setTexture(&lineTex);
		lineShape.setSize(sf::Vector2f(length, 10));
		lineShape.setOrigin(sf::Vector2f(length / 2.0f, 2.5f));
		//circleShape.setOrigin(radius, radius);

		//circleShape.setTextureRect(sf::IntRect(0, 0, 100, 100));
		lineShape.setPosition(centerPos);

		if (kine)
		{
			lineBodyDef.type = b2_kinematicBody;
		}
		if (!kine)
		{
			lineBodyDef.type = b2_dynamicBody;
		}

		kineBody = world.CreateBody(&lineBodyDef);
		lineBox.SetAsBox(length / 2.0f, 1);

		lineFixtureDef.shape = &lineBox;
		//boxFixtureDef.density = data->density;
		lineFixtureDef.friction = friction;
		lineFixtureDef.restitution = bounce;
		lineFixtureDef.density = density;
		lineFixtureDef.filter.categoryBits = categoryBits;
		lineFixtureDef.filter.maskBits = maskBits;
		kineBody->SetFixedRotation(false);
		kineBody->CreateFixture(&lineFixtureDef);
		kineBody->SetTransform(kineBody->GetPosition(), 0);

		//data->type = 1;
		kineBody->SetTransform(b2Vec2(centerPos.x, centerPos.y), 0);

		kineBody->SetUserData(data);
	}
	~LineBlock()
	{
		if (kineBody)
		{
			std::cout << "Destroyed line block" << std::endl;
			kineBody->GetWorld()->DestroyBody(kineBody);
		}
	}
	virtual void Draw(sf::RenderWindow &w, bool playing)
	{
		kineBody->SetFixedRotation(false);
		if (playing)
		{
			lineShape.setPosition(sf::Vector2f((kineBody->GetPosition().x * 10), (kineBody->GetPosition().y * 10)));
			lineShape.setRotation(kineBody->GetAngle() * (180 / 3.14f));
		}
		//std::cout << "angle: " << kineBody->GetAngle() << std::endl;
		w.draw(lineShape);
	}
	virtual void AlphaDraw(sf::RenderWindow &w)
	{
		lineTex.loadFromFile("Textures/Elements/girderLine.png");
		lineShape.setTexture(&lineTex);
		lineShape.setFillColor(sf::Color(100, 100, 100, 127));

		w.draw(lineShape);
	}
	virtual void CameraView()
	{
	}
	virtual void Resize(sf::Vector2f s)
	{
		length = s.x;
		lineShape.setSize(sf::Vector2f(length, 10));
		position = centerPos - sf::Vector2f(length / 2.0f, length / 2.0f);
		//circleShape.setOrigin(s.x / 2.0f, s.x / 2.0f);
	}
	virtual sf::Vector2f GetWorldPosition()
	{
		return centerPos;
	}
	virtual sf::Vector2f GetScreenPosition()
	{
		return centerPos;// -sf::Vector2f(radius, radius);
	}
	virtual void SetPosition(sf::Vector2f pos)
	{
		centerPos = pos;
		position = centerPos - sf::Vector2f(length / 2.0f, length / 2.0f);
	}
	virtual void Move(sf::Vector2f pos)
	{
		centerPos += pos;
		position = centerPos - sf::Vector2f(length / 2.0f, length / 2.0f);
		b2Vec2 bodyPos = kineBody->GetPosition() + b2Vec2(pos.x, pos.y);
		lineShape.setPosition((sf::Vector2f)centerPos - (sf::Vector2f)cameraPos);

		kineBody->SetTransform(b2Vec2(centerPos.x,
			centerPos.y), rotation * (3.14f / 180));
	}
	virtual sf::Vector2f GetSize()
	{
		return sf::Vector2f(length, length);
		//centerPos = 
	}
	virtual void Rotate(float a)
	{
		lineShape.rotate(a);
		rotation = lineShape.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual float GetRotation()
	{
		return rotation;
	}
	virtual void SetRotation(float r)
	{
		lineShape.setRotation(r);
		rotation = lineShape.getRotation();

		kineBody->SetTransform(kineBody->GetPosition(), rotation * (3.14f / 180.0f));
	}
	virtual void ResetRotation()
	{

	}
	virtual int BlockType()
	{
		return 7;
	}
	virtual bool Valid()
	{
		return valid;
	}
	virtual b2Body* GetBody()
	{
		return kineBody;
	}
	virtual b2BodyDef GetBodyDef()
	{
		return lineBodyDef;
	}
	virtual bool GetKine()
	{
		if (lineBodyDef.type == b2_kinematicBody)
		{
			return true;
		}
		return false;
	}
	virtual void SetGravity(sf::Vector2f grav)
	{
		gravity = grav;
	}
	virtual sf::Vector2f GetGravity()
	{
		return gravity;
	}
private:
	sf::Vector2f centerPos;
	sf::Vector2f position;
	sf::Vector2i cameraPos;

	sf::RectangleShape lineShape;
	sf::Texture lineTex;

	bool valid = false;

	float rotation = 0;
	float length = 0;

	sf::Vector2f gravity;
	b2BodyDef lineBodyDef;
	b2Body* kineBody = NULL;
	b2PolygonShape lineBox;
	b2FixtureDef lineFixtureDef;
};
