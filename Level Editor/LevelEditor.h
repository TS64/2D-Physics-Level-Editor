#pragma once
#include "SFML\Graphics.hpp"
#include "Block.h"
#include "MathMethods.h"
#include <iostream>
#include <Box2D\Box2D.h>

using namespace std;

class LevelEditor
{
public:
	LevelEditor(sf::View* v, MathMethods* m);
	void Update(sf::RenderWindow &w);
	void UpdatePreview(sf::RenderWindow &w);
	void HandleEvents(sf::Event e, sf::RenderWindow &w);
	void Draw(sf::RenderWindow &w, bool playing);
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> getLevelRects();
	void setLevelRects(vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> rects);
	void DeleteBlock(int b);
	b2World* GetWorld();
	int GetCurrentBlock();
	int GetCurrentElement();
	void SetCurrentElement(int e);
	void SetCurrentBlockStats(float friction, float bounce, float density);
	string GetNameOfLevelToSave();
	string GetNameOfLevelToLoad();
	void SetNameOfLevelToSave(string fileName);
	void SetNameOfLevelToLoad(string fileName);
	bool ShapeSelected();
	void SetPlayerSpawn(sf::Vector2f pos);
	sf::Vector2f GetPlayerSpawn();
	void SetLevelFinish(sf::Vector2f pos);
	sf::Vector2f GetLevelFinish();
	vector<b2Body*> GetBlockBodies();
	b2Body* GetBlockBody(int i);
	void SetBackGround(int i);
	int GetBackGround();
	void WindowResized();
	void ResetLevel();
	void SaveDefaultLevel();
	void ApplyGravity();
	void SetShape(int shape);
	int GetShape();
	int GetSelectedBlockType();
	sf::Vector2f GetSelectedBlockSize();

	sf::Vector2f PositionOfSelectedShape();
	void SetPosition(int block, sf::Vector2f pos);
	float GetBounciness(int block);
	void SetBounciness(int block, float bounciness);
	float GetFriction(int block);
	void SetFriction(int block, float friction);
	float GetDensity(int block);
	void SetDensity(int block, float density);
	float GetRotation(int block);
	void SetRotation(int block, float rotation);
	int GetCurrentShape();
	void SetCurrentShape(int shape);
	void SetIsKine(bool kine);

private:
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> levelRects; // Rect, type, angle, bounce, friction, density, kine

	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> defaultLevelRects; // Rect, type, angle, bounce, friction, density, kine

	sf::RectangleShape previewShape;
	sf::RectangleShape shapeHighlight;
	sf::Vector2i mouseDownPos;
	sf::Vector2i mouseUpPos;
	sf::Vector2i mouseCurrentPos;
	sf::Vector2f cameraPos;
	sf::Vector2f panIconPos;
	sf::Vector2f levelStartPos;
	sf::Vector2f levelFinishPos;

	sf::Vector2f defaultGravity;

	bool startDrawing = false;
	bool stopDrawing = false;
	bool cameraMoved = false;
	bool shapeSelected;
	bool spaceKeyPressed = false;
	bool shrunkWorld = false;
	bool isKine = true;

	int selectedShapeID;
	int highlightThickness;
	int currentBackground;

	float zoomLevel;

	sf::Sprite groundTopSpr;
	sf::Sprite groundUnderSpr;
	sf::Sprite panSpr;

	sf::Sprite currentBGSpr;
	sf::Sprite grassBGSpr;
	sf::Sprite caveBGSpr;
	sf::Sprite cityBGSpr;
	sf::Sprite desertBGSpr;
	sf::Sprite mountainBGSpr;
	sf::Sprite skyBGSpr;
	sf::Sprite spaceBGSpr;
	vector<sf::Sprite> BGSprVec;

	sf::Texture groundTopTex;
	sf::Texture groundUnderTex;
	sf::Texture panTex;

	sf::Texture currentBGTex;
	sf::Texture grassBGTex;
	sf::Texture caveBGTex;
	sf::Texture cityBGTex;
	sf::Texture desertBGTex;
	sf::Texture mountainBGTex;
	sf::Texture skyBGTex;
	sf::Texture spaceBGTex;

	bool ClickedBlock(sf::Event e, sf::RenderWindow &w);
	void HandleDrawing(sf::Event e, sf::RenderWindow &w);
	void MoveCamera(sf::Event e);
	void MoveShape(sf::Event e);
	void ChangeHighlightThickness(sf::Event e);
	void DrawBox2dDebug(sf::RenderWindow &w, bool c);
	void HandleFloating();
	void CreateBalls(sf::RenderWindow &w);
	void LoadContent();

	tuple<float, float, float> currentBlockStats; // Current block stats, friction, bounce, density
	enum Tile { GRASS, ICE, METAL, JELLY, WATER, OIL, SPIKE, SPAWN, FINISH };
	enum Shape { RECTANGLE, CIRCLE, LINE };
	enum entityCategory
	{ 
		GRASSBLOCK = 0x0001,
		WATERBLOCK = 0x0002,
		BALL = 0x0004,
	};
	Tile tile = GRASS;
	Shape shape = RECTANGLE;

	vector<Block*> blocks;
	Block* tempBlock;
	MathMethods* math;

	b2World m_world;

	sf::View* view1;
	string nameOfLevelToSave;
	string nameOfLevelToLoad;
};
