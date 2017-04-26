#pragma once

#define BOOST_FILESYSTEM_VERSION 3

#include "SFML\Graphics.hpp"

#include "boost\filesystem\operations.hpp"
#include "boost\filesystem\path.hpp"
#include "boost\progress.hpp"

#include <iostream>
#include <vector>
#include "Button.h"
#include "LevelEditor.h"
#include "FileManager.h"
#include "Player.h"
#include "MathMethods.h"
#include "Tooltip.h"

class UI
{
public:
	UI(LevelEditor* e, FileManager* f, Player* p, MathMethods* m, sf::View* v);
	bool HandleEvents(sf::Event e, sf::RenderWindow &w);
	void Draw(sf::RenderWindow &w);
	std::string GetNameToSaveLevelAs();
	void WindowResized();
	bool CanZoom(float zoom);

	enum Mode { BUILDING, SAVING, LOADING, EDITING, PLAYING, OPTIONS };
	enum Attribute { BOUNCE, FRICTION, DENSITY, ROTATION, POSITIONX, POSITIONY };
	Mode mode = BUILDING;
	Attribute attribute = BOUNCE;
private:
	sf::RectangleShape top_UI_line;
	sf::RectangleShape top_UI_box;
	sf::RectangleShape textBoxHighlight;
	sf::RectangleShape backgroundBox;

	sf::Sprite elementSelectSpr;
	sf::Sprite saveLevelUISpr;
	sf::Sprite levelNameBoxSpr;
	sf::Sprite scrollDownSpr;
	sf::Sprite scrollUpSpr;
	sf::Sprite scrollLeftSpr;
	sf::Sprite scrollRightSpr;
	sf::Sprite blockAttributeSpr;
	sf::Sprite bounceTextBoxSpr;
	sf::Sprite positionXTextBoxSpr;
	sf::Sprite positionYTextBoxSpr;
	sf::Sprite frictionTextBoxSpr;
	sf::Sprite densityTextBoxSpr;
	sf::Sprite positionTextBoxSpr;
	sf::Sprite rotationTextBoxSpr;
	sf::Sprite saveChangesSpr;
	sf::Sprite backgroundBtnSpr;
	sf::Sprite saveBlockSpr;
	sf::Sprite cameraSpr;
	vector<sf::Sprite> attributeSprVec; // Bounce, friction, gravity, rotate
	sf::Sprite shapeSelectBoxSpr;
	sf::Sprite shapeSelectIconSpr;
	sf::Sprite gravitySelectBoxSpr;

	sf::Sprite currentBGSpr;
	sf::Sprite grassBtnSpr;
	sf::Sprite caveBtnSpr;
	sf::Sprite cityBtnSpr;
	sf::Sprite desertBtnSpr;
	sf::Sprite mountainBtnSpr;
	sf::Sprite skyBtnSpr;
	sf::Sprite spaceBtnSpr;
	vector<sf::Sprite> BGBtnSprVec;

	sf::Texture elementSelectTex;
	sf::Texture saveLevelUITex;
	sf::Texture levelNameBoxTex;
	sf::Texture scrollDownTex;
	sf::Texture scrollUpTex;
	sf::Texture scrollLeftTex;
	sf::Texture scrollRightTex;
	sf::Texture blockAttributeTex;
	sf::Texture bounceTextBoxTex;
	sf::Texture positionTextBoxTex;
	sf::Texture saveChangesTex;
	sf::Texture backgroundBtnTex;
	sf::Texture currentBGTex;
	sf::Texture saveBlockTex;
	sf::Texture cameraTex;
	sf::Texture shapeSelectBoxTex;
	sf::Texture shapeSelectIconTex;
	sf::Texture gravitySelectBoxTex;

	sf::Texture grassBtnTex;
	sf::Texture caveBtnTex;
	sf::Texture cityBtnTex;
	sf::Texture desertBtnTex;
	sf::Texture mountainBtnTex;
	sf::Texture skyBtnTex;
	sf::Texture spaceBtnTex;
	vector<sf::Texture> crossBtnSpr;

	sf::View* view1;

	vector<Button*> buttons;
	vector<Button*> elementButtons;
	vector<Button*> displayedElementButtons;
	vector<tuple<float, float, float>> elementStats; // Friction, bounce, density
	vector<tuple<float, float, float>> displayedElementStats; // Friction, bounce, density

	LevelEditor* editor;
	FileManager* fileMgr;
	Player* player;
	MathMethods* math;

	bool enteringText = false;
	bool displayingBGs = false;
	bool gravitySelected = false;

	sf::Text nameOfLevel;
	sf::Text bounceValText;
	sf::Text frictionValText;
	sf::Text densityValText;
	sf::Text positionXValText;
	sf::Text positionYValText;
	sf::Text rotationValText;
	sf::Text cameraPosText;
	sf::Font arialFont;

	vector<string> availableLevels;
	vector<string> displayedLevels;

	vector<sf::Text> levelNamesText;
	vector<Tooltip> toolTips;

	int selectedLevelID;
	unsigned int maxLevelsOnScreen = 10;
	unsigned int maxElementsOnScreen = 10;
	int selectedElement = 0;
	int topOfElementsList = 0;
	int topOfLevelList = 0;
	unsigned int levelListFontSize = 40;

	void LoadLevelNames();
	void LoadContent();
	void RemoveNonLevels();
	bool ScrollLevelList(int i);
	bool ScrollElementsList(int i);
	void SwitchMode(int mode);
	void AddNewElement();
	void DeleteElement(sf::RenderWindow &w);
	void DisplayTooltips(sf::RenderWindow &w);
};
