#pragma once
#include <iostream>
#include <tuple>
#include "SFML\Graphics.hpp"
#include "LevelEditor.h"
#include "FileManager.h"

class Button
{
public:
	~Button() {}
	virtual int execute() = 0;
	virtual void Enable(bool b) = 0;
	virtual void Draw(sf::RenderWindow &w) = 0;
	virtual sf::Vector2f GetPosition() = 0;
	virtual void SetPosition(sf::Vector2f p) = 0;
	virtual sf::Vector2f GetSize() = 0;
	virtual int GetType() = 0;
	virtual string GetName() = 0;
	virtual void SetScale(float s) = 0;
protected:
	Button() {}
};

class SaveFileButton : public Button
{
public:
	SaveFileButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e, FileManager* f)
	{
		position = pos;
		size = s;

		saveBtnTex.loadFromFile("Textures/Buttons/buttonSave.png");
		saveBtnSpr.setTexture(saveBtnTex);
		saveBtnSpr.setPosition(position);

		editor = e;
		fileMgr = f;
	}
	virtual int execute()
	{
		if (editor->GetNameOfLevelToSave().size() != 0 &&
			editor->GetNameOfLevelToSave().find_first_not_of(' ') != std::string::npos)
		{
			fileMgr->SaveMap(editor->getLevelRects(), editor->GetNameOfLevelToSave());
			return 0;
		}
		return 1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		saveBtnSpr.setPosition(position);
		saveBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)saveBtnSpr.getPosition()));
		w.draw(saveBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "SaveFile"; }
	virtual void SetScale(float s)
	{
		saveBtnSpr.setScale(s, s);
	}
	
private:
	sf::Sprite saveBtnSpr;
	sf::Texture saveBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class SaveMenuButton : public Button
{
public:
	SaveMenuButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e, FileManager* f)
	{
		position = pos;
		size = s;

		saveBtnTex.loadFromFile("Textures/Buttons/buttonSave.png");
		saveBtnSpr.setTexture(saveBtnTex);
		saveBtnSpr.setPosition(position);

		editor = e;
		fileMgr = f;
	}
	virtual int execute()
	{
		return 1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		saveBtnSpr.setPosition(position);
		saveBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)saveBtnSpr.getPosition()));
		w.draw(saveBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "SaveMenu"; }
	virtual void SetScale(float s)
	{
		saveBtnSpr.setScale(s, s);
	}

private:
	sf::Sprite saveBtnSpr;
	sf::Texture saveBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class LoadMenuButton : public Button
{
public:
	LoadMenuButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e, FileManager* f)
	{
		position = pos;
		size = s;

		loadBtnTex.loadFromFile("Textures/Buttons/buttonLoad.png");
		loadBtnSpr.setTexture(loadBtnTex);
		loadBtnSpr.setPosition(position);

		editor = e;
		fileMgr = f;
	}
	virtual int execute()
	{
		return 2;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		loadBtnSpr.setPosition(position);
		loadBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)loadBtnSpr.getPosition()));
		w.draw(loadBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "LoadMenu"; }
	virtual void SetScale(float s)
	{
		loadBtnSpr.setScale(s, s);
	}

	sf::Sprite loadBtnSpr;
	sf::Texture loadBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class LoadFileButton : public Button
{
public:
	LoadFileButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e, FileManager* f)
	{
		position = pos;
		size = s;

		loadBtnTex.loadFromFile("Textures/Buttons/buttonLoad.png");
		loadBtnSpr.setTexture(loadBtnTex);
		loadBtnSpr.setPosition(position);

		editor = e;
		fileMgr = f;
	}
	virtual int execute()
	{
		if (editor->GetNameOfLevelToLoad().size() != 0 &&
			editor->GetNameOfLevelToLoad().find_first_not_of(' ') != std::string::npos)
		{
			editor->setLevelRects(fileMgr->LoadMap(editor->GetNameOfLevelToLoad()));
			editor->SetNameOfLevelToLoad("");
			return 0;
		}
		return 2;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		loadBtnSpr.setPosition(position);
		loadBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)loadBtnSpr.getPosition()));
		w.draw(loadBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "LoadFile"; }
	virtual void SetScale(float s)
	{
		loadBtnSpr.setScale(s, s);
	}

	sf::Sprite loadBtnSpr;
	sf::Texture loadBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class DeleteButton : public Button
{
public:
	DeleteButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e, FileManager* f)
	{
		position = pos;
		size = s;

		deleteBtnTex.loadFromFile("Textures/Buttons/buttonDelete.png");
		deleteBtnSpr.setTexture(deleteBtnTex);
		deleteBtnSpr.setPosition(position);

		editor = e;
		fileMgr = f;
	}
	virtual int execute()
	{
		int del = editor->GetCurrentBlock();
		if (del >= 0)
		{
			cout << "Deleting " << del << endl;
			editor->DeleteBlock(del);
		}
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		deleteBtnSpr.setPosition(position);
		deleteBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)deleteBtnSpr.getPosition()));
		w.draw(deleteBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Delete"; }
	virtual void SetScale(float s)
	{
		deleteBtnSpr.setScale(s, s);
	}

	sf::Sprite deleteBtnSpr;
	sf::Texture deleteBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class OptionButton : public Button
{
public:
	OptionButton(sf::Vector2f pos, sf::Vector2f s)
	{
		position = pos;
		size = s;

		optionBtnTex.loadFromFile("Textures/Buttons/buttonOptions.png");
		optionBtnSpr.setTexture(optionBtnTex);
		optionBtnSpr.setPosition(position);
	}
	virtual int execute()
	{
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		/*optionBtnSpr.setPosition(position);
		optionBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)optionBtnSpr.getPosition()));
		w.draw(optionBtnSpr);*/
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Option"; }
	virtual void SetScale(float s)
	{
		optionBtnSpr.setScale(s, s);
	}

	sf::Sprite optionBtnSpr;
	sf::Texture optionBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	bool enabled = true;
};

class EditBlockButton : public Button
{
public:
	EditBlockButton(sf::Vector2f pos, sf::Vector2f s)
	{
		position = pos;
		size = s;

		editBtnTex.loadFromFile("Textures/Buttons/buttonEdit.png");
		editBtnSpr.setTexture(editBtnTex);
		editBtnSpr.setPosition(position);
	}
	virtual int execute()
	{
		if (enabled)
		{
			printf("CLICKED EDIT\n");
			return 3;
		}
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		if (enabled)
		{
			editBtnSpr.setPosition(position);
			editBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)editBtnSpr.getPosition()));
			w.draw(editBtnSpr);
		}
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "EditBlock"; }
	virtual void SetScale(float s)
	{
		editBtnSpr.setScale(s, s);
	}

	sf::Sprite editBtnSpr;
	sf::Texture editBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	bool enabled = false;
};

class CancelButton : public Button
{
public:
	CancelButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		cancelBtnTex.loadFromFile("Textures/Buttons/buttonCancel.png");
		cancelBtnSpr.setTexture(cancelBtnTex);
		cancelBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		editor->SetNameOfLevelToLoad("");
		return 0;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		cancelBtnSpr.setPosition(position);
		cancelBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)cancelBtnSpr.getPosition()));
		w.draw(cancelBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Cancel"; }
	virtual void SetScale(float s)
	{
		cancelBtnSpr.setScale(s, s);
	}

	sf::Sprite cancelBtnSpr;
	sf::Texture cancelBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class PlayButton : public Button
{
public:
	PlayButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		playBtnTex.loadFromFile("Textures/Buttons/buttonPlay.png");
		playBtnSpr.setTexture(playBtnTex);
		playBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		cout << "Switched to play mode" << endl;
		return 4;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		playBtnSpr.setPosition(position);
		playBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)playBtnSpr.getPosition()));
		w.draw(playBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Play"; }
	virtual void SetScale(float s)
	{
		playBtnSpr.setScale(s, s);
	}

	sf::Sprite playBtnSpr;
	sf::Texture playBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

class BuildButton : public Button
{
public:
	BuildButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		buildBtnTex.loadFromFile("Textures/Buttons/buttonBuild.png");
		buildBtnSpr.setTexture(buildBtnTex);
		buildBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		cout << "Switched to build mode" << endl;
		return 0;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		buildBtnSpr.setPosition(position);
		buildBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)buildBtnSpr.getPosition()));
		w.draw(buildBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Build"; }
	virtual void SetScale(float s)
	{
		buildBtnSpr.setScale(s, s);
	}

	sf::Sprite buildBtnSpr;
	sf::Texture buildBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	FileManager* fileMgr;
	bool enabled = true;
};

// ID = 1
class GroundButton : public Button
{
public:
	GroundButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		groundBtnTex.loadFromFile("Textures/Buttons/buttonGround.png");
		groundBtnSpr.setTexture(groundBtnTex);
		groundBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		printf("Setting element to grass\n");
		editor->SetCurrentElement(0);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		groundBtnSpr.setPosition(position);
		groundBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)groundBtnSpr.getPosition()));
		w.draw(groundBtnSpr);
	}
	virtual int GetType() { return 1; }
	virtual string GetName() { return "Ground"; }
	virtual void SetScale(float s)
	{
		groundBtnSpr.setScale(s, s);
	}
	sf::Sprite groundBtnSpr;
	sf::Texture groundBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};

// ID = 2
class IceButton : public Button
{
public:
	IceButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		iceBtnTex.loadFromFile("Textures/Buttons/buttonIce.png");
		iceBtnSpr.setTexture(iceBtnTex);
		iceBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		printf("Setting element to ice\n");
		editor->SetCurrentElement(1);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		iceBtnSpr.setPosition(position);
		iceBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)iceBtnSpr.getPosition()));
		w.draw(iceBtnSpr);
	}
	virtual int GetType() { return 2; }
	virtual string GetName() { return "Ice"; }
	virtual void SetScale(float s)
	{
		iceBtnSpr.setScale(s, s);
	}
	sf::Sprite iceBtnSpr;
	sf::Texture iceBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};

// ID = 3
class WaterButton : public Button
{
public:
	WaterButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		waterBtnTex.loadFromFile("Textures/Buttons/buttonWater.png");
		waterBtnSpr.setTexture(waterBtnTex);
		waterBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		printf("Setting element to water\n");
		editor->SetCurrentElement(4);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		waterBtnSpr.setPosition(position);
		waterBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)waterBtnSpr.getPosition()));
		w.draw(waterBtnSpr);
	}
	virtual int GetType() { return 3; }
	virtual string GetName() { return "Water"; }
	virtual void SetScale(float s)
	{
		waterBtnSpr.setScale(s, s);
	}
	sf::Sprite waterBtnSpr;
	sf::Texture waterBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};

// ID = 4
class MetalButton : public Button
{
public:
	MetalButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;

		metalBtnTex.loadFromFile("Textures/Buttons/buttonMetal.png");
		metalBtnSpr.setTexture(metalBtnTex);
		metalBtnSpr.setPosition(position);

		editor = e;
	}
	virtual int execute()
	{
		printf("Setting element to metal\n");
		editor->SetCurrentElement(2);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		metalBtnSpr.setPosition(position);
		metalBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)metalBtnSpr.getPosition()));
		w.draw(metalBtnSpr);
	}
	virtual int GetType() { return 4; }
	virtual string GetName() { return "Metal"; }
	virtual void SetScale(float s)
	{
		metalBtnSpr.setScale(s, s);
	}
	sf::Sprite metalBtnSpr;
	sf::Texture metalBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};

class SpawnButton : public Button
{
public:
	SpawnButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;
		editor = e;

		spawnBtnTex.loadFromFile("Textures/Buttons/buttonSpawnPoint.png");
		spawnBtnSpr.setTexture(spawnBtnTex);
		spawnBtnSpr.setPosition(position);
	}
	virtual int execute()
	{
		printf("Setting element to spawn\n");
		editor->SetCurrentElement(7);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		spawnBtnSpr.setPosition(position);
		spawnBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)spawnBtnSpr.getPosition()));
		w.draw(spawnBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Spawn"; }
	virtual void SetScale(float s)
	{
		spawnBtnSpr.setScale(s, s);
	}
private:
	sf::Sprite spawnBtnSpr;
	sf::Texture spawnBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};

class FinishButton : public Button
{
public:
	FinishButton(sf::Vector2f pos, sf::Vector2f s, LevelEditor* e)
	{
		position = pos;
		size = s;
		editor = e;

		finishBtnTex.loadFromFile("Textures/Buttons/buttonFinishPoint.png");
		finishBtnSpr.setTexture(finishBtnTex);
		finishBtnSpr.setPosition(position);
	}
	virtual int execute()
	{
		printf("Setting element to finish\n");
		editor->SetCurrentElement(8);
		return -1;
	}
	virtual void Enable(bool b)
	{
		enabled = b;
	}
	virtual sf::Vector2f GetPosition()
	{
		return position;
	}
	virtual void SetPosition(sf::Vector2f p)
	{
		position = p;
	}
	virtual sf::Vector2f GetSize()
	{
		return size;
	}
	virtual void Draw(sf::RenderWindow &w)
	{
		finishBtnSpr.setPosition(position);
		finishBtnSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)finishBtnSpr.getPosition()));
		w.draw(finishBtnSpr);
	}
	virtual int GetType() { return -1; }
	virtual string GetName() { return "Finish"; }
	virtual void SetScale(float s)
	{
		finishBtnSpr.setScale(s, s);
	}
private:
	sf::Sprite finishBtnSpr;
	sf::Texture finishBtnTex;
	sf::Vector2f position;
	sf::Vector2f size;
	LevelEditor* editor;
	bool enabled = true;
};
