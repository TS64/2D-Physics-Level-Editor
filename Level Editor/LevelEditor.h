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
	void UpdatePreview(sf::RenderWindow &w);	// Update the transparent preview of the block being placed
	void HandleEvents(sf::Event e, sf::RenderWindow &w);	
	void Draw(sf::RenderWindow &w, bool playing);
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> getLevelRects();	// Get a vector of blocks in the level
	void setLevelRects(vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> rects);	// Set the vector of blocks in the level
	void DeleteBlock(int b);	// Delete the block on given position
	b2World* GetWorld();		// Get the world
	int GetCurrentBlock();		// Get the number of the selected block
	int GetCurrentElement();	// Get the element of the selected block
	void SetCurrentElement(int e);	// Set the selected element
	void SetCurrentBlockStats(float friction, float bounce, float density);	// Get the stats of the current block
	string GetNameOfLevelToSave();	// Get the name of the level you are saving
	string GetNameOfLevelToLoad();	// Get the name of the level you are loading
	void SetNameOfLevelToSave(string fileName);
	void SetNameOfLevelToLoad(string fileName);
	bool ShapeSelected();
	
	void SetLevelFinish(sf::Vector2f pos);
	sf::Vector2f GetLevelFinish();
	vector<b2Body*> GetBlockBodies();
	b2Body* GetBlockBody(int i);

	void WindowResized();
	void ResetLevel();
	void SaveDefaultLevel();
	void ApplyGravity();
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
	void SetIsKine(bool kine);	// Set if the next shape is kinematic(true) or dynamic(false)
	void SetShape(int shape);	// Set current shape
	int GetShape();	// Get selected shape (box, circle, line)
	void SetBackGround(int i);
	int GetBackGround();
	void SetPlayerSpawn(sf::Vector2f pos);
	sf::Vector2f GetPlayerSpawn();
	void TurnOffHighlight();

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
	bool isShapeSelected;
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
