#pragma once

#include "Box2D\Box2D.h"
#include "MathMethods.h"
#include <cmath>
#include <iostream>

using namespace std;

class ScaleWorld
{
public:
	ScaleWorld::ScaleWorld(b2World& w, MathMethods* m) : m_world(w), math(m) {}
	void ScaleDownWorld()
	{
		for (b2Body* BodyIterator = m_world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->IsActive())
			{
				int count = 0;

				for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
				{
					count++;
					b2Shape::Type shapeType = b2Fixture->GetType();
					b2CircleShape* circleShape = (b2CircleShape*)b2Fixture->GetShape();
					int isPlayer = (int)b2Fixture->GetUserData();
					if (shapeType == b2Shape::e_circle && isPlayer != 1) // is not a player
					{
						b2Vec2 newPos = math->scaleDownVector(b2Fixture->GetBody()->GetPosition());
						float angle = b2Fixture->GetBody()->GetAngle();
						b2Fixture->GetBody()->SetTransform(newPos, angle);
						b2Fixture->GetBody()->SetFixedRotation(false);

						math->scaleDownBody(b2Fixture);
						b2Fixture->GetBody()->SetFixedRotation(false);
					}
					else if (shapeType == b2Shape::e_polygon && isPlayer != 1) // is not a player
					{
						b2Vec2 newPos = math->scaleDownVector(b2Fixture->GetBody()->GetPosition());
						float angle = b2Fixture->GetBody()->GetAngle();
						b2Fixture->GetBody()->SetTransform(newPos, angle);

						math->scaleDownBody(b2Fixture);
						b2Fixture->GetBody()->ResetMassData();
						b2Fixture->GetBody()->SetFixedRotation(false);
					}
				}
			}
		}
	}

	void ScaleUpWorld()
	{
		for (b2Body* BodyIterator = m_world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->IsActive())
			{
				int count = 0;

				for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
				{
					count++;
					b2Shape::Type shapeType = b2Fixture->GetType();
					b2CircleShape* circleShape = (b2CircleShape*)b2Fixture->GetShape();
					int isPlayer = (int)b2Fixture->GetUserData();
					if (shapeType == b2Shape::e_circle && isPlayer != 1)
					{
						b2Vec2 newPos = math->scaleUpVector(b2Fixture->GetBody()->GetPosition());
						float angle = b2Fixture->GetBody()->GetAngle();
						b2Fixture->GetBody()->SetTransform(newPos, angle);

						math->scaleUpBody(b2Fixture);
						b2Fixture->GetBody()->SetFixedRotation(false);
					}
					else if (shapeType == b2Shape::e_polygon && isPlayer != 1)
					{
						b2Vec2 newPos = math->scaleUpVector(b2Fixture->GetBody()->GetPosition());
						float angle = b2Fixture->GetBody()->GetAngle();
						b2Fixture->GetBody()->SetTransform(newPos, angle);

						math->scaleUpBody(b2Fixture);
						b2Fixture->GetBody()->SetFixedRotation(false);
					}
				}
			}
		}
	}
	void StepWorld(float dt)
	{
		m_world.Step(dt, 8, 8);
	}

	b2World& m_world;
	MathMethods* math;
};
