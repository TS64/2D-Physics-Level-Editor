#pragma once

#include "SFML\Graphics.hpp"
#include "Box2D\Box2D.h"
#include <cmath>
#include <iostream>

using namespace std;

class MathMethods
{
public:
	const float SCALE = 10;
	float ZOOM = 1;
	MathMethods::MathMethods(sf::RenderWindow *window) : w(window) {}
	sf::Vector2f mapPixelToCoords(sf::Vector2f pos)
	{
		return w->mapPixelToCoords((sf::Vector2i)pos);
	}
	sf::Vector2i mapCoordsToPixel(sf::Vector2f pos)
	{
		return w->mapCoordsToPixel(pos);
	}
	sf::Vector2f rotateVector(sf::Vector2f v, float angleDegrees)
	{
		float ca = cos(angleDegrees);
		float sa = sin(angleDegrees);
		return sf::Vector2f((ca * v.x) - (sa * v.y), (sa * v.x) + (ca * v.y));
	}

	b2Vec2 scaleUpVector(b2Vec2 v)
	{
		return b2Vec2(v.x * SCALE, v.y * SCALE);
	}
	b2Vec2 scaleDownVector(b2Vec2 v)
	{
		return b2Vec2(v.x / SCALE, v.y / SCALE);
	}
	void scaleUpBody(b2Fixture* f)
	{
		b2Shape::Type shapeType = f->GetType();
		b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
		if (shapeType == b2Shape::e_polygon)
		{
			/*for (int i = 0; i < 4; i++)
			{
				cout << "OldUpshapevert" << i << ":" << shape->m_vertices[i].x << "-" << shape->m_vertices[i].y << endl;
			}*/
			float width = abs(shape->m_vertices[0].x - shape->m_vertices[1].x) / 2.0f;
			float height = abs(shape->m_vertices[1].y - shape->m_vertices[2].y) / 2.0f;
			//cout << "w: " << width << "|h: " << height << endl;
			shape->m_vertices[0].Set(-width * SCALE, -height * SCALE);
			shape->m_vertices[1].Set(width * SCALE, -height * SCALE);
			shape->m_vertices[2].Set(width * SCALE, height * SCALE);
			shape->m_vertices[3].Set(-width * SCALE, height * SCALE);
			/*for (int i = 0; i < 4; i++)
			{
				cout << "NewUpshapevert" << i << ":" << shape->m_vertices[i].x << "-" << shape->m_vertices[i].y << endl;
			}*/
		}
		else if (shapeType == b2Shape::e_circle)
		{
			f->GetShape()->m_radius = f->GetShape()->m_radius * SCALE;
		}
	}
	void scaleDownBody(b2Fixture* f)
	{
		b2Shape::Type shapeType = f->GetType();
		b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
		if (shapeType == b2Shape::e_polygon)
		{
			/*for (int i = 0; i < 4; i++)
			{
				cout << "OldDownshapevert" << i << ":" << shape->m_vertices[i].x << "-" << shape->m_vertices[i].y << endl;
			}*/
			float width = abs(shape->m_vertices[0].x - shape->m_vertices[1].x) / 2.0f;
			float height = abs(shape->m_vertices[1].y - shape->m_vertices[2].y) / 2.0f;
			//cout << "w: " << width << "|h: " << height << endl;
			shape->m_vertices[0].Set(-width / SCALE, -height / SCALE);
			shape->m_vertices[1].Set(width / SCALE, -height / SCALE);
			shape->m_vertices[2].Set(width / SCALE, height / SCALE);
			shape->m_vertices[3].Set(-width / SCALE, height / SCALE);
			/*for (int i = 0; i < 4; i++)
			{
				cout << "NewDownshapevert" << i << ":" << shape->m_vertices[i].x << "-" << shape->m_vertices[i].y << endl;
			}*/
		}
		else if (shapeType == b2Shape::e_circle)
		{
			f->GetShape()->m_radius = f->GetShape()->m_radius / SCALE;
		}
	}

	string TrimZerosFromFloat(string s) // Trim zeros and/or decimal point from float/double as a string
	{
		while (s.find(".") != string::npos &&
			s.substr(s.length() - 1, 1) == "0" ||
			s.substr(s.length() - 1, 1) == ".")
		{
			s.pop_back();
		}
		//cout << "Returning trimmed: " << s << endl;
		return s;
	}
	void SetZoom(float zoom)
	{
		ZOOM = zoom;
	}

	float DistanceBetweenPoints(sf::Vector2f a, sf::Vector2f b)
	{
		return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	}
	float DotProduct(sf::Vector2f a, sf::Vector2f b)
	{
		return (a.x * b.x) + (a.y * b.y);
	}
	float Determinant(sf::Vector2f a, sf::Vector2f b)
	{
		return (a.x * b.y) + (a.y * b.x);
	}
private:
	sf::RenderWindow* w;
};
