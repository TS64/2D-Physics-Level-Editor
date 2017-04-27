#include "UI.h"

UI::UI(LevelEditor* e, FileManager* f, Player* p, MathMethods* m, sf::View* v)
{
	top_UI_box.setFillColor(sf::Color::White);
	top_UI_box.setOutlineColor(sf::Color::Black);
	top_UI_box.setOutlineThickness(2.0f);
	top_UI_box.setPosition(sf::Vector2f(-2, -2));
	top_UI_box.setSize(sf::Vector2f(v->getSize().x + 4, 57));

	textBoxHighlight.setOutlineColor(sf::Color::Yellow);
	textBoxHighlight.setFillColor(sf::Color::Transparent);
	textBoxHighlight.setOutlineThickness(2.0f);
	textBoxHighlight.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + (saveLevelUITex.getSize().x / 2)
		- (600 / 2),
		saveLevelUISpr.getPosition().y + (saveLevelUITex.getSize().y / 2)
		- (50.0f / 2.0f)));
	textBoxHighlight.setSize(sf::Vector2f(600, 50));

	backgroundBox.setFillColor(sf::Color(198, 198, 198, 255));
	backgroundBox.setOutlineColor(sf::Color::Transparent);
	backgroundBox.setOutlineThickness(0);
	backgroundBox.setPosition(0, 70);
	backgroundBox.setSize(sf::Vector2f(80, 420));

	editor = e;
	fileMgr = f;
	player = p;
	view1 = v;
	math = m;

	buttons.push_back(new SaveMenuButton
	(sf::Vector2f(5, 5), sf::Vector2f(100, 50), editor, fileMgr));

	buttons.push_back(new LoadMenuButton
	(sf::Vector2f(110, 5), sf::Vector2f(100, 50), editor, fileMgr));

	buttons.push_back(new DeleteButton
	(sf::Vector2f(view1->getSize().x - 55, 5), sf::Vector2f(50, 50), editor, fileMgr));

	buttons.push_back(new OptionButton
	(sf::Vector2f(view1->getSize().x - 110, 5), sf::Vector2f(50, 50)));

	buttons.push_back(new EditBlockButton
	(sf::Vector2f(0, 0), sf::Vector2f(60, 25)));

	buttons.push_back(new PlayButton
	(sf::Vector2f(0, v->getSize().y - 50), sf::Vector2f(100, 50), e));

	elementButtons.push_back(new GroundButton
	(sf::Vector2f(240, 5), sf::Vector2f(50, 50), editor));

	elementButtons.push_back(new IceButton
	(sf::Vector2f(290, 5), sf::Vector2f(50, 50), editor));

	elementButtons.push_back(new MetalButton
	(sf::Vector2f(340, 5), sf::Vector2f(50, 50), editor));

	//elementButtons.push_back(new WaterButton
	//(sf::Vector2f(390, 5), sf::Vector2f(50, 50), editor));

	elementButtons.push_back(new SpawnButton
	(sf::Vector2f(390, 5), sf::Vector2f(50, 50), editor));

	elementButtons.push_back(new FinishButton
	(sf::Vector2f(440, 5), sf::Vector2f(50, 50), editor));

	elementStats.push_back({ 2.5, 0.1, 1 });
	elementStats.push_back({ 0.0, 0.2, 1 });
	elementStats.push_back({ 1.5, 0.2, 1 });
	//elementStats.push_back({ 0.0, 0.0, 1 });
	elementStats.push_back({ 0.0, 0.0, 0.0 });
	elementStats.push_back({ 0.0, 0.0, 0.0 });

	displayedElementButtons = elementButtons;
	displayedElementStats = elementStats;

	LoadContent();

	int gap = abs(((scrollLeftSpr.getPosition().x + (scrollLeftTex.getSize().x * math->ZOOM)) + 10)
		- (scrollRightSpr.getPosition().x - 10));
	maxElementsOnScreen = gap / (50 * math->ZOOM);
}

bool UI::HandleEvents(sf::Event e, sf::RenderWindow &w)
{
	bool clicked = false;
	if (e.type == sf::Event::MouseButtonPressed &&
		e.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePos = math->mapPixelToCoords(sf::Vector2f(e.mouseButton.x, e.mouseButton.y));
		
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			if (e.mouseButton.x >= buttons[i]->GetPosition().x && e.mouseButton.x <= buttons[i]->GetPosition().x + (buttons[i]->GetSize().x * math->ZOOM) &&
				e.mouseButton.y >= buttons[i]->GetPosition().y && e.mouseButton.y <= buttons[i]->GetPosition().y + (buttons[i]->GetSize().y * math->ZOOM) &&
				!clicked)
			{
				int m = buttons[i]->execute();
				if (m != -1 && m != (int)mode)
				{
					mode = (Mode)m;
					SwitchMode(mode);
				}
				return true;
			}
		}
		for (int i = 0; i < displayedElementButtons.size(); i++)
		{
			if (e.mouseButton.x >= displayedElementButtons[i]->GetPosition().x && 
				e.mouseButton.x <= displayedElementButtons[i]->GetPosition().x + (displayedElementButtons[i]->GetSize().x * math->ZOOM) &&
				e.mouseButton.y >= displayedElementButtons[i]->GetPosition().y && 
				e.mouseButton.y <= displayedElementButtons[i]->GetPosition().y + (displayedElementButtons[i]->GetSize().y * math->ZOOM))
			{
				displayedElementButtons[i]->execute();
				float friction = std::get<0>(displayedElementStats[i]);
				float bounce = std::get<1>(displayedElementStats[i]);
				float density = std::get<2>(displayedElementStats[i]);
				editor->SetCurrentBlockStats(friction, bounce, density);
				selectedElement = i;
				return true;
			}
		}
		if (mode == BUILDING)
		{
			if (mousePos.x >= backgroundBtnSpr.getPosition().x && mousePos.x <= backgroundBtnSpr.getPosition().x + ((backgroundBtnTex.getSize().x / 3.0f) * math->ZOOM) &&
				mousePos.y >= backgroundBtnSpr.getPosition().y && mousePos.y <= backgroundBtnSpr.getPosition().y + ((backgroundBtnTex.getSize().y / 3.0f) * math->ZOOM))
			{
				//printf("clicked bgbtn\n");
				if (displayingBGs)
				{
					displayingBGs = false;
				}
				else if (!displayingBGs)
				{
					displayingBGs = true;
				}
				return true;
			}

			// Scroll element list left
			if (mousePos.x >= scrollLeftSpr.getPosition().x && mousePos.x <= scrollLeftSpr.getPosition().x + (scrollLeftTex.getSize().x * math->ZOOM) &&
				mousePos.y >= scrollLeftSpr.getPosition().y && mousePos.y <= scrollLeftSpr.getPosition().y + (scrollLeftTex.getSize().y * math->ZOOM))
			{
				printf("clicked scroll Left\n");
				ScrollElementsList(-1);
				return true;
			}

			// Scroll element list right
			if (mousePos.x >= scrollRightSpr.getPosition().x && mousePos.x <= scrollRightSpr.getPosition().x + (scrollRightTex.getSize().x * math->ZOOM) &&
				mousePos.y >= scrollRightSpr.getPosition().y && mousePos.y <= scrollRightSpr.getPosition().y + (scrollRightTex.getSize().y * math->ZOOM))
			{
				printf("clicked scroll Right\n");
				ScrollElementsList(1);
				return true;
			}

			// Save a custom block to your list
			if (mousePos.x >= saveBlockSpr.getPosition().x && mousePos.x <= saveBlockSpr.getPosition().x + (50 * math->ZOOM) &&
				mousePos.y >= saveBlockSpr.getPosition().y && mousePos.y <= saveBlockSpr.getPosition().y + (50 * math->ZOOM))
			{
				printf("clicked save block button\n");
				AddNewElement();
				return true;
			}

			sf::Vector2f yesPos = sf::Vector2f(shapeSelectBoxSpr.getPosition().x + (((456 + 97) / 3.0f) * math->ZOOM), shapeSelectBoxSpr.getPosition().y + ((85 / 3.0f) * math->ZOOM));
			sf::Vector2f noPos = sf::Vector2f(shapeSelectBoxSpr.getPosition().x + ((456 / 3.0f) * math->ZOOM), shapeSelectBoxSpr.getPosition().y + ((85 / 3.0f) * math->ZOOM));
			float height = (59 / 3.0f) * math->ZOOM;
			float width = (89 / 3.0f) * math->ZOOM;

			if (mousePos.x >= noPos.x && mousePos.x <= noPos.x + width &&
				mousePos.y >= noPos.y && mousePos.y <= noPos.y + height)
			{
				printf("GRAVITY OFF\n");
				gravitySelected = false;
				editor->SetIsKine(true);
				return true;
			}
			if (mousePos.x >= yesPos.x && mousePos.x <= yesPos.x + width &&
				mousePos.y >= yesPos.y && mousePos.y <= yesPos.y + height)
			{
				printf("GRAVITY ON\n");
				gravitySelected = true;
				editor->SetIsKine(false);
				return true;
			}
			for (int i = 0; i < 3; i++)
			{
				sf::Vector2f shapePos;
				float boxSize = ((450 / 3.0f) / 3.0f) * math->ZOOM;
				
				shapePos.x = shapeSelectBoxSpr.getPosition().x;
				shapePos.y = shapeSelectBoxSpr.getPosition().y;

				if (mousePos.x >= shapePos.x + (boxSize * i) && mousePos.x <= shapePos.x + (boxSize * (1 + i)) &&
					mousePos.y >= shapePos.y && mousePos.y <= shapePos.y + boxSize)
				{
					cout << "Set shape: " << i << endl;
					editor->SetShape(i);
					return true;
				}
			}
			shapeSelectBoxSpr.setPosition((view1->getSize().x / 2.0f) - ((450 / 3.0f) / 2.0f) * math->ZOOM, 57 * math->ZOOM);
			// Display background image options
			if (displayingBGs)
			{
				for (int i = 0; i < BGBtnSprVec.size(); i++)
				{
					if (e.mouseButton.x >= BGBtnSprVec[i].getPosition().x && e.mouseButton.x <= BGBtnSprVec[i].getPosition().x + ((BGBtnSprVec[i].getTexture()->getSize().x / 3.0f) * math->ZOOM) &&
						e.mouseButton.y >= BGBtnSprVec[i].getPosition().y && e.mouseButton.y <= BGBtnSprVec[i].getPosition().y + ((BGBtnSprVec[i].getTexture()->getSize().y / 3.0f) * math->ZOOM))
					{
						editor->SetBackGround(i);
						currentBGSpr = BGBtnSprVec[i];
						currentBGSpr.setPosition(5 * math->ZOOM, 62 * math->ZOOM);
						currentBGSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
						return true;
					}
				}
			}
		}
		else if (mode == SAVING)
		{
			sf::Vector2f mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
			mousePos = w.mapPixelToCoords((sf::Vector2i)mousePos);
			if (mousePos.x >= levelNameBoxSpr.getPosition().x && mousePos.x <= levelNameBoxSpr.getPosition().x + (600 * math->ZOOM) &&
				mousePos.y >= levelNameBoxSpr.getPosition().y && mousePos.y <= levelNameBoxSpr.getPosition().y + (50 * math->ZOOM))
			{
				enteringText = true;
			}
		}
		else if (mode == LOADING)
		{
			sf::Vector2f mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
			mousePos = w.mapPixelToCoords((sf::Vector2i)mousePos);
			cout << "mousePos" << mousePos.x << "-" << mousePos.y << endl;
			cout << "pos: " << levelNamesText[0].getPosition().x << "-" << levelNamesText[0].getPosition().y << endl;
			cout << "size: " << levelNamesText[0].getLocalBounds().width << "-" << levelNamesText[0].getLocalBounds().height << endl;
			for (int i = 0; i < levelNamesText.size(); i++)
			{
				if (mousePos.x >= levelNamesText[i].getPosition().x && mousePos.x <= levelNamesText[i].getPosition().x + levelNamesText[i].getLocalBounds().width &&
					mousePos.y >= levelNamesText[i].getPosition().y + 10 && mousePos.y <= levelNamesText[i].getPosition().y + levelNamesText[i].getLocalBounds().height + 10)
				{
					selectedLevelID = i;
					cout << "Selected to load: " << (string)levelNamesText[i].getString() << endl;
					textBoxHighlight.setPosition(levelNamesText[i].getGlobalBounds().left, levelNamesText[i].getGlobalBounds().top);
					textBoxHighlight.setSize(sf::Vector2f(levelNamesText[i].getLocalBounds().width, levelNamesText[i].getLocalBounds().height));
					//cout << "highsize: " << textBoxHighlight.getSize().x << "-" << textBoxHighlight.getSize().y << endl;
					editor->SetNameOfLevelToLoad(levelNamesText[i].getString());
				}
			}
			if (mousePos.x >= scrollDownSpr.getPosition().x && mousePos.x <= scrollDownSpr.getPosition().x + (scrollDownTex.getSize().x * math->ZOOM) &&
				mousePos.y >= scrollDownSpr.getPosition().y && mousePos.y <= scrollDownSpr.getPosition().y + (scrollDownTex.getSize().x * math->ZOOM))
			{
				printf("Scroll down\n");
				ScrollLevelList(1);
				return true;
			}
			else if (mousePos.x >= scrollUpSpr.getPosition().x && mousePos.x <= scrollUpSpr.getPosition().x + (scrollUpTex.getSize().x * math->ZOOM) &&
				mousePos.y >= scrollUpSpr.getPosition().y && mousePos.y <= scrollUpSpr.getPosition().y + (scrollUpTex.getSize().y * math->ZOOM))
			{
				printf("Scroll Up\n");
				ScrollLevelList(-1);
				return true;
			}
		}
		else if (mode == EDITING)
		{
			sf::Vector2f mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
			mousePos = w.mapPixelToCoords((sf::Vector2i)mousePos);
			cout << "mousePos: " << mousePos.x << "-" << mousePos.y << endl;
			if (mousePos.x >= saveChangesSpr.getPosition().x && mousePos.x <= saveChangesSpr.getPosition().x + (saveChangesTex.getSize().x * math->ZOOM) &&
				mousePos.y >= saveChangesSpr.getPosition().y && mousePos.y <= saveChangesSpr.getPosition().y + (saveChangesTex.getSize().y * math->ZOOM))
			{
				if (bounceValText.getString() == "")
				{
					bounceValText.setString("0");
				}
				if (frictionValText.getString() == "")
				{
					frictionValText.setString("0");
				}
				if (densityValText.getString() == "")
				{
					densityValText.setString("0");
				}
				if (positionXValText.getString() == "")
				{
					positionXValText.setString("0");
				}
				if (positionYValText.getString() == "")
				{
					positionYValText.setString("0");
				}
				if (rotationValText.getString() == "")
				{
					rotationValText.setString("0");
				}

				float friction = stof((string)frictionValText.getString());
				float bounce = stof((string)bounceValText.getString());
				float density = stof((string)densityValText.getString());
				float rotation = stof((string)rotationValText.getString());
				float positionX = stof((string)positionXValText.getString());
				float positionY = stof((string)positionYValText.getString());

				editor->SetBounciness(editor->GetCurrentBlock(), bounce);
				editor->SetFriction(editor->GetCurrentBlock(), friction);
				editor->SetDensity(editor->GetCurrentBlock(), density);
				editor->SetRotation(editor->GetCurrentBlock(), rotation);
				editor->SetPosition(editor->GetCurrentBlock(), sf::Vector2f(positionX, positionY));
				editor->SetCurrentBlockStats(friction, bounce, density);
				SwitchMode(BUILDING);
				editor->TurnOffHighlight();
				return true;
			}
			for (int i = 0; i < attributeSprVec.size(); i++)
			{
				if (mousePos.x >= attributeSprVec[i].getPosition().x && mousePos.x <= attributeSprVec[i].getPosition().x + ((attributeSprVec[i].getTexture()->getSize().x / 3.0f) * math->ZOOM) &&
					mousePos.y >= attributeSprVec[i].getPosition().y && mousePos.y <= attributeSprVec[i].getPosition().y + ((attributeSprVec[i].getTexture()->getSize().y / 3.0f) * math->ZOOM))
				{
					enteringText = true;
					textBoxHighlight.setPosition(attributeSprVec[i].getPosition());
					textBoxHighlight.setSize(((sf::Vector2f)attributeSprVec[i].getTexture()->getSize() / 3.0f) * math->ZOOM);
					attribute = (Attribute)i;
				}
			}
			if (mousePos.x >= positionXTextBoxSpr.getPosition().x && mousePos.x <= positionXTextBoxSpr.getPosition().x + ((positionXTextBoxSpr.getTexture()->getSize().x / 3.0f) * math->ZOOM) &&
				mousePos.y >= positionXTextBoxSpr.getPosition().y && mousePos.y <= positionXTextBoxSpr.getPosition().y + ((positionXTextBoxSpr.getTexture()->getSize().y / 3.0f) * math->ZOOM))
			{
				enteringText = true;
				textBoxHighlight.setPosition(positionXTextBoxSpr.getPosition());
				textBoxHighlight.setSize(((sf::Vector2f)positionXTextBoxSpr.getTexture()->getSize() / 3.0f) * math->ZOOM);
				attribute = POSITIONX;
			}
			if (mousePos.x >= positionYTextBoxSpr.getPosition().x && mousePos.x <= positionYTextBoxSpr.getPosition().x + ((positionYTextBoxSpr.getTexture()->getSize().x / 3.0f) * math->ZOOM) &&
				mousePos.y >= positionYTextBoxSpr.getPosition().y && mousePos.y <= positionYTextBoxSpr.getPosition().y + ((positionYTextBoxSpr.getTexture()->getSize().y / 3.0f) * math->ZOOM))
			{
				enteringText = true;
				textBoxHighlight.setPosition(positionYTextBoxSpr.getPosition());
				textBoxHighlight.setSize(((sf::Vector2f)positionYTextBoxSpr.getTexture()->getSize() / 3.0f) * math->ZOOM);
				attribute = POSITIONY;
			}
		}
	}
	if (mode == BUILDING)
	{
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::X)
			{
				DeleteElement(w);
			}
		}
	}
	if (mode == SAVING)
	{
		if (enteringText)
		{
			if (e.type == sf::Event::TextEntered)
			{
				if (e.text.unicode < 128 && nameOfLevel.getString().getSize() <= 20)
				{
					if (static_cast<char>(e.text.unicode) != '<' &&
						static_cast<char>(e.text.unicode) != '>' &&
						static_cast<char>(e.text.unicode) != ':' &&
						static_cast<char>(e.text.unicode) != '"' &&
						static_cast<char>(e.text.unicode) != '/' &&
						static_cast<char>(e.text.unicode) != '\\' &&
						static_cast<char>(e.text.unicode) != '|' &&
						static_cast<char>(e.text.unicode) != '?' &&
						static_cast<char>(e.text.unicode) != '*' &&
						static_cast<char>(e.text.unicode) != '.')
					{
						string str = nameOfLevel.getString();
						str += static_cast<char>(e.text.unicode);
						nameOfLevel.setString(str);
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && nameOfLevel.getString().getSize() != 0)
			{
				string str = nameOfLevel.getString();
				str.pop_back();
				nameOfLevel.setString(str);
			}
		}
	}
	if (mode == EDITING)
	{
		if (enteringText)
		{
			switch (attribute)
			{
				case BOUNCE:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((bounceValText.getString().find(".") != sf::String::InvalidPos && bounceValText.getString().getSize() <= 9) ||
							(bounceValText.getString().find(".") == sf::String::InvalidPos && bounceValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = bounceValText.getString();
								str += static_cast<char>(e.text.unicode);
								bounceValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = bounceValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									bounceValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && bounceValText.getString().getSize() != 0)
					{
						string str = bounceValText.getString();
						str.pop_back();
						bounceValText.setString(str);
					}
					break;
				}
				case FRICTION:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((frictionValText.getString().find(".") != sf::String::InvalidPos && frictionValText.getString().getSize() <= 9) ||
							(frictionValText.getString().find(".") == sf::String::InvalidPos && frictionValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = frictionValText.getString();
								str += static_cast<char>(e.text.unicode);
								frictionValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = frictionValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									frictionValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && frictionValText.getString().getSize() != 0)
					{
						string str = frictionValText.getString();
						str.pop_back();
						frictionValText.setString(str);
					}
					break;
				}
				case DENSITY:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((densityValText.getString().find(".") != sf::String::InvalidPos && densityValText.getString().getSize() <= 9) ||
							(densityValText.getString().find(".") == sf::String::InvalidPos && densityValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = densityValText.getString();
								str += static_cast<char>(e.text.unicode);
								densityValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = densityValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									densityValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && densityValText.getString().getSize() != 0)
					{
						string str = densityValText.getString();
						str.pop_back();
						densityValText.setString(str);
					}
					break;
				}
				case ROTATION:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((rotationValText.getString().find(".") != sf::String::InvalidPos && rotationValText.getString().getSize() <= 9) ||
							(rotationValText.getString().find(".") == sf::String::InvalidPos && rotationValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = rotationValText.getString();
								str += static_cast<char>(e.text.unicode);
								rotationValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = rotationValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									rotationValText.setString(str);
								}
							}
							else if (static_cast<char>(e.text.unicode) == '-')
							{
								string str = rotationValText.getString();
								if (str == "")
								{
									str += "-";
									rotationValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && rotationValText.getString().getSize() != 0)
					{
						string str = rotationValText.getString();
						str.pop_back();
						rotationValText.setString(str);
					}
					break;
				}
				case POSITIONX:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((positionXValText.getString().find(".") != sf::String::InvalidPos && positionXValText.getString().getSize() <= 9) ||
							(positionXValText.getString().find(".") == sf::String::InvalidPos && positionXValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = positionXValText.getString();
								str += static_cast<char>(e.text.unicode);
								positionXValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = positionXValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									positionXValText.setString(str);
								}
							}
							else if (static_cast<char>(e.text.unicode) == '-')
							{
								string str = positionXValText.getString();
								if (str == "")
								{
									str += "-";
									positionXValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && positionXValText.getString().getSize() != 0)
					{
						string str = positionXValText.getString();
						str.pop_back();
						positionXValText.setString(str);
					}
					break;
				}
				case POSITIONY:
				{
					if (e.type == sf::Event::TextEntered)
					{
						if (e.text.unicode < 128 &&
							((positionYValText.getString().find(".") != sf::String::InvalidPos && positionYValText.getString().getSize() <= 9) ||
							(positionYValText.getString().find(".") == sf::String::InvalidPos && positionYValText.getString().getSize() <= 8)))
						{
							if (static_cast<char>(e.text.unicode) == '0' ||
								static_cast<char>(e.text.unicode) == '1' ||
								static_cast<char>(e.text.unicode) == '2' ||
								static_cast<char>(e.text.unicode) == '3' ||
								static_cast<char>(e.text.unicode) == '4' ||
								static_cast<char>(e.text.unicode) == '5' ||
								static_cast<char>(e.text.unicode) == '6' ||
								static_cast<char>(e.text.unicode) == '7' ||
								static_cast<char>(e.text.unicode) == '8' ||
								static_cast<char>(e.text.unicode) == '9')
							{
								string str = positionYValText.getString();
								str += static_cast<char>(e.text.unicode);
								positionYValText.setString(str);
							}
							else if (static_cast<char>(e.text.unicode) == '.')
							{
								string str = positionYValText.getString();
								if (str.find('.') == std::string::npos)
								{
									str += ".";
									positionYValText.setString(str);
								}
							}
							else if (static_cast<char>(e.text.unicode) == '-')
							{
								string str = positionYValText.getString();
								if (str == "")
								{
									str += "-";
									positionYValText.setString(str);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && positionYValText.getString().getSize() != 0)
					{
						string str = positionYValText.getString();
						str.pop_back();
						positionYValText.setString(str);
					}
					break;
				}
			}
		}
	}
	return clicked;
}

void UI::Draw(sf::RenderWindow &w)
{
	switch (mode)
	{
		case BUILDING:
		{
			top_UI_box.setPosition(-2, -2);
			top_UI_box.setPosition(w.mapPixelToCoords((sf::Vector2i)top_UI_box.getPosition()));
			w.draw(top_UI_box);

			//buttons[2]->SetPosition(sf::Vector2f(view1->getSize().x - (buttons[2]->GetSize().x * math->ZOOM), 5)); // Delete Button
			//buttons[3]->SetPosition(sf::Vector2f(view1->getSize().x - 155, 5)); // Option Button
			if (editor->ShapeSelected())
			{
				// Edit Block button
				buttons[4]->Enable(true);
				if (editor->GetSelectedBlockType() != 6)
				{
					sf::Vector2f posShape = sf::Vector2f(editor->PositionOfSelectedShape().x, editor->PositionOfSelectedShape().y - (25 * math->ZOOM));
					posShape = (sf::Vector2f)w.mapCoordsToPixel(posShape);
					buttons[4]->SetPosition(sf::Vector2f(posShape));
				}
				if (editor->GetSelectedBlockType() == 6)
				{
					sf::Vector2f posShape = sf::Vector2f(editor->PositionOfSelectedShape().x - editor->GetSelectedBlockSize().x, 
						editor->PositionOfSelectedShape().y - editor->GetSelectedBlockSize().x - (25 * math->ZOOM));
					posShape = (sf::Vector2f)w.mapCoordsToPixel(posShape);
					buttons[4]->SetPosition(sf::Vector2f(posShape));
				}
			}
			else if (!editor->ShapeSelected())
			{
				// Edit Block button
				buttons[4]->Enable(false);
			}
			for (int i = 0; i < buttons.size(); i++)
			{
				buttons[i]->Draw(w);
			}
			for (int i = 0; i < displayedElementButtons.size(); i++)
			{
				displayedElementButtons[i]->Draw(w);
			}
			elementSelectSpr.setPosition(displayedElementButtons[0]->GetPosition().x + (selectedElement * 50.0f * math->ZOOM), 5.0f);
			elementSelectSpr.setPosition(w.mapPixelToCoords((sf::Vector2i)elementSelectSpr.getPosition()));
			w.draw(elementSelectSpr);
			if (displayingBGs)
			{
				backgroundBox.setPosition(math->mapPixelToCoords(sf::Vector2f(0, 70 * math->ZOOM)));
				w.draw(backgroundBox);
			}

			backgroundBtnSpr.setPosition(math->mapPixelToCoords(sf::Vector2f(0, 57 * math->ZOOM)));
			w.draw(backgroundBtnSpr);

			currentBGSpr.setPosition(math->mapPixelToCoords(sf::Vector2f(5 * math->ZOOM, 62 * math->ZOOM)));
			w.draw(currentBGSpr);

			saveBlockSpr.setPosition(math->mapPixelToCoords(sf::Vector2f(view1->getSize().x - 5 - 5 - 5 - (50 * math->ZOOM * 3), 5)));
			w.draw(saveBlockSpr);
			if (displayingBGs)
			{
				for (int i = 0; i < BGBtnSprVec.size(); i++)
				{
					BGBtnSprVec[i].setPosition(sf::Vector2f(5 * math->ZOOM, (57 * math->ZOOM) + (60 * math->ZOOM) + 5 + (i * 51 * math->ZOOM)));
					BGBtnSprVec[i].setPosition(math->mapPixelToCoords(BGBtnSprVec[i].getPosition()));
					BGBtnSprVec[i].setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
					w.draw(BGBtnSprVec[i]);
				}
			}
			float rightOfLoad = 5 + (100 * math->ZOOM) + 5 + (100 * math->ZOOM) + 5;
			scrollLeftSpr.setPosition(math->mapPixelToCoords(sf::Vector2f(rightOfLoad + 5, 5)));
			w.draw(scrollLeftSpr);

			float leftOfSaveBlock = view1->getSize().x - 5 - (50 * math->ZOOM) - 5 - (50 * math->ZOOM)
				- 5 - (50 * math->ZOOM) - 5 - (15 * math->ZOOM);
			scrollRightSpr.setPosition(math->mapPixelToCoords(sf::Vector2f(leftOfSaveBlock - 5, 5)));
			w.draw(scrollRightSpr);

			if (editor->GetBackGround() == 1 || editor->GetBackGround() == 2 || editor->GetBackGround() == 6)
			{
				cameraPosText.setFillColor(sf::Color::White);
			}
			if (editor->GetBackGround() == 0 || editor->GetBackGround() == 3 || editor->GetBackGround() == 4 || editor->GetBackGround() == 5)
			{
				cameraPosText.setFillColor(sf::Color::Black);
			}
			cameraPosText.setString("(" + to_string((int)view1->getCenter().x - ((int)view1->getSize().x / 2)) 
				+ ", " + to_string((int)view1->getCenter().y - ((int)view1->getSize().y / 2)) + ")");
			cameraPosText.setCharacterSize(20 * math->ZOOM);
			cameraPosText.setPosition(view1->getSize().x - (cameraPosText.getLocalBounds().width), 60 * math->ZOOM);
			cameraPosText.setPosition(math->mapPixelToCoords(cameraPosText.getPosition()));
			w.draw(cameraPosText);
			cameraSpr.setPosition(cameraPosText.getPosition().x - (cameraTex.getSize().x * cameraSpr.getScale().x) + 10, cameraPosText.getPosition().y - (3 * math->ZOOM));
			cameraSpr.setScale((1.0f / 5.0f) * math->ZOOM, (1.0f / 5.0f) * math->ZOOM);
			w.draw(cameraSpr);

			shapeSelectBoxSpr.setPosition((view1->getSize().x / 2.0f) - ((450 / 3.0f) / 2.0f) * math->ZOOM, 57 * math->ZOOM);
			shapeSelectBoxSpr.setPosition(math->mapPixelToCoords(shapeSelectBoxSpr.getPosition()));
			shapeSelectBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

			sf::Vector2f shapePos;
			float boxSize = ((450 / 3.0f) / 3.0f) * math->ZOOM;

			shapePos.x = shapeSelectBoxSpr.getPosition().x;
			shapePos.y = shapeSelectBoxSpr.getPosition().y;

			shapeSelectIconSpr.setPosition(shapePos.x + (1 * math->ZOOM) + (boxSize * editor->GetShape()),
				shapePos.y + (1 * math->ZOOM));

			sf::Vector2f fix = shapeSelectBoxSpr.getPosition();
			fix = math->mapPixelToCoords(fix);
			//shapeSelectIconSpr.setPosition(sf::Vector2f(fix.x, shapeSelectIconSpr.getPosition().y));

			//shapeSelectIconSpr.setPosition(math->mapPixelToCoords(shapeSelectIconSpr.getPosition()));
			shapeSelectIconSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
			w.draw(shapeSelectBoxSpr);
			w.draw(shapeSelectIconSpr);

			gravitySelectBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
			if (gravitySelected)
			{
				gravitySelectBoxSpr.setPosition(shapeSelectBoxSpr.getPosition().x + (((456 + 97) / 3.0f) * math->ZOOM), shapeSelectBoxSpr.getPosition().y + ((85 / 3.0f) * math->ZOOM));
			}
			if (!gravitySelected)
			{
				gravitySelectBoxSpr.setPosition(shapeSelectBoxSpr.getPosition().x + ((456 / 3.0f) * math->ZOOM), shapeSelectBoxSpr.getPosition().y + ((85 / 3.0f) * math->ZOOM));
			}
			w.draw(gravitySelectBoxSpr);

			DisplayTooltips(w);
			
			break;
		}
		case SAVING:
		{
			saveLevelUISpr.setPosition((view1->getSize().x / 2) - ((saveLevelUITex.getSize().x / 2) * math->ZOOM), 
				(view1->getSize().y / 2) - ((saveLevelUITex.getSize().y / 2) * math->ZOOM));
			saveLevelUISpr.setPosition(w.mapPixelToCoords((sf::Vector2i)saveLevelUISpr.getPosition()));
			w.draw(saveLevelUISpr);

			// Save Button
			buttons[0]->SetPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((100 / 2) * math->ZOOM) - (60 * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM) + (60 * math->ZOOM)));
			buttons[0]->SetPosition((sf::Vector2f)w.mapCoordsToPixel(buttons[0]->GetPosition()));

			// Cancel Button
			buttons[1]->SetPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((120 / 2) * math->ZOOM) + (60 * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM) + (60 * math->ZOOM)));
			buttons[1]->SetPosition((sf::Vector2f)w.mapCoordsToPixel(buttons[1]->GetPosition()));

			levelNameBoxSpr.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((600 / 2) * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM)));
			w.draw(levelNameBoxSpr);

			if (enteringText)
			{
				textBoxHighlight.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
					- ((600 / 2) * math->ZOOM),
					saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
					- ((50.0f / 2.0f) * math->ZOOM)));
				//textBoxHighlight.setPosition(math->mapPixelToCoords(textBoxHighlight.getPosition()));
				//textBoxHighlight.setSize(sf::Vector2f(600, 50));
				w.draw(textBoxHighlight);
			}

			sf::FloatRect textRect = nameOfLevel.getLocalBounds();
			nameOfLevel.setOrigin(textRect.left + (textRect.width / 2.0f),
				textRect.top + (textRect.height / 2.0f));
			nameOfLevel.setPosition(sf::Vector2f(levelNameBoxSpr.getPosition().x + (300 * math->ZOOM),
				levelNameBoxSpr.getPosition().y + (25 * math->ZOOM)));
			//nameOfLevel.setPosition(math->mapPixelToCoords(nameOfLevel.getPosition()));
			w.draw(nameOfLevel);

			for (int i = 0; i < buttons.size(); i++)
			{
				buttons[i]->Draw(w);
			}
			break;
		}
		case LOADING:
		{
			// Load File Button
			buttons[0]->SetPosition(sf::Vector2f((view1->getSize().x / 2) - (60 * math->ZOOM),
				view1->getSize().y - (60 * math->ZOOM)));
			//buttons[0]->SetPosition(w.mapPixelToCoords((sf::Vector2i)buttons[0]->GetPosition()));

			// Cancel Button
			buttons[1]->SetPosition(sf::Vector2f((view1->getSize().x / 2) + (60 * math->ZOOM),
				view1->getSize().y - (60 * math->ZOOM)));
			//buttons[1]->SetPosition(w.mapPixelToCoords((sf::Vector2i)buttons[1]->GetPosition()));

			for (int i = 0; i < levelNamesText.size(); i++)
			{
				w.draw(levelNamesText[i]);
			}
			for (int i = 0; i < buttons.size(); i++)
			{
				buttons[i]->Draw(w);
			}
			if (editor->GetNameOfLevelToLoad() != "")
			{
				w.draw(textBoxHighlight);
			}
			scrollUpSpr.setPosition(sf::Vector2f(200, 50));
			scrollUpSpr.setPosition(math->mapPixelToCoords(scrollUpSpr.getPosition()));
			scrollUpSpr.setScale(math->ZOOM, math->ZOOM);
			w.draw(scrollUpSpr);

			scrollDownSpr.setPosition(sf::Vector2f(200, view1->getSize().y - (100 * math->ZOOM)));
			scrollDownSpr.setPosition(math->mapPixelToCoords(scrollDownSpr.getPosition()));
			scrollDownSpr.setScale(math->ZOOM, math->ZOOM);
			w.draw(scrollDownSpr);
			break;
		}
		case EDITING:
		{
			blockAttributeSpr.setPosition(sf::Vector2f((view1->getSize().x / 2) - (350 * math->ZOOM), (view1->getSize().y / 2) - (100 * math->ZOOM)));
			blockAttributeSpr.setPosition(math->mapPixelToCoords(blockAttributeSpr.getPosition()));
			blockAttributeSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

			bounceValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (0 * 26)) * math->ZOOM));
			//bounceValText.setPosition(math->mapPixelToCoords(bounceValText.getPosition()));

			frictionValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (1 * 26)) * math->ZOOM));
			//frictionValText.setPosition(math->mapPixelToCoords(frictionValText.getPosition()));

			densityValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (2 * 26)) * math->ZOOM));
			//densityValText.setPosition(math->mapPixelToCoords(densityValText.getPosition()));

			rotationValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (3 * 26)) * math->ZOOM));
			//rotationValText.setPosition(math->mapPixelToCoords(rotationValText.getPosition()));

			positionXValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (4 * 26)) * math->ZOOM));
			//positionXValText.setPosition(math->mapPixelToCoords(positionXValText.getPosition()));

			positionYValText.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 5 + 89 + 50) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (4 * 26)) * math->ZOOM));
			//positionYValText.setPosition(math->mapPixelToCoords(positionYValText.getPosition()));

			saveChangesSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (450 * math->ZOOM),
				blockAttributeSpr.getPosition().y + (100 * math->ZOOM)));
			//saveChangesSpr.setPosition(math->mapPixelToCoords(saveChangesSpr.getPosition()));

			w.draw(blockAttributeSpr);
			for (int i = 0; i < 4; i++)
			{
				attributeSprVec[i].setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (155 * math->ZOOM),
					blockAttributeSpr.getPosition().y + (12 + (i * 26)) * math->ZOOM));
				//attributeSprVec[i].setPosition(math->mapPixelToCoords(attributeSprVec[i].getPosition()));
				attributeSprVec[i].setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
				w.draw(attributeSprVec[i]);
			}
			positionXTextBoxSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (155 * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (4 * 26)) * math->ZOOM));
			//positionXTextBoxSpr.setPosition(math->mapPixelToCoords(positionXTextBoxSpr.getPosition()));
			positionXTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

			positionYTextBoxSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 89 + 50) * math->ZOOM),
				blockAttributeSpr.getPosition().y + (12 + (4 * 26)) * math->ZOOM));
			//positionYTextBoxSpr.setPosition(math->mapPixelToCoords(positionYTextBoxSpr.getPosition()));
			positionYTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
			w.draw(positionXTextBoxSpr);
			w.draw(positionYTextBoxSpr);
			w.draw(bounceValText);
			w.draw(frictionValText);
			w.draw(densityValText);
			w.draw(rotationValText);
			w.draw(positionXValText);
			w.draw(positionYValText);
			w.draw(saveChangesSpr);
			// Cancel button
			buttons[0]->SetPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (450 * math->ZOOM),
				blockAttributeSpr.getPosition().y + (20 * math->ZOOM)));
			buttons[0]->SetPosition((sf::Vector2f)math->mapCoordsToPixel(buttons[0]->GetPosition()));
			for (int i = 0; i < buttons.size(); i++)
			{
				buttons[i]->Draw(w);
			}
			if (enteringText)
			{
				w.draw(textBoxHighlight);
			}
			break;
		}
		case PLAYING:
		{
			// Build button
			buttons[0]->SetPosition(sf::Vector2f(0, view1->getSize().y - (50 * math->ZOOM)));

			for (int i = 0; i < buttons.size(); i++)
			{
				buttons[i]->Draw(w);
			}
			if (player->LevelComplete())
			{
				player->SetPlaying(false);
				SwitchMode((int)BUILDING);
			}
		}
	}
}

std::string UI::GetNameToSaveLevelAs()
{
	return nameOfLevel.getString();
}

void UI::LoadLevelNames()
{
	boost::filesystem::path p("./");

	availableLevels.clear();
	try
	{
		if (boost::filesystem::exists(p))
		{
			if (boost::filesystem::is_directory(p))
			{
				//cout << p << " is a directory containing:\n";

				for (auto&& x : boost::filesystem::directory_iterator(p))
				{
					availableLevels.push_back(x.path().filename().string());
				}

				std::sort(availableLevels.begin(), availableLevels.end());

				for (auto&& x : availableLevels)
				{
					//cout << "   " << x << "\n";
				}

				RemoveNonLevels();

				levelNamesText.clear();
				for (int i = 0; i < availableLevels.size(); i++)
				{
					levelNamesText.push_back(sf::Text());
					levelNamesText.back().setCharacterSize((unsigned int)levelListFontSize * math->ZOOM);
					levelNamesText.back().setString(availableLevels[i]);
					levelNamesText.back().setFont(arialFont);
					levelNamesText.back().setFillColor(sf::Color::Black);
					levelNamesText.back().setPosition(sf::Vector2f(50.0f, 50 + (25.0f * math->ZOOM) + 5 + (i * levelNamesText.back().getCharacterSize())));
					levelNamesText.back().setPosition(math->mapPixelToCoords(levelNamesText.back().getPosition()));
				}
			}
			else
			{
				cout << p << " exists, but is not a directory" << endl;
			}
		}
		else
		{
			cout << p << " does not exist\n";
		}
	}
	catch (const boost::filesystem::filesystem_error& ex)
	{
		cout << ex.what() << "\n";
	}
}

void UI::RemoveNonLevels()
{
	for (int i = 0; i < availableLevels.size(); i++)
	{
		boost::filesystem::path p("./" + availableLevels[i]);
		if (boost::filesystem::is_regular_file(p))
		{
			int len = availableLevels[i].size();
			if (len >= 4)
			{
				string temp = availableLevels[i];
				string ending = temp.erase(0, len - 4);
				if (ending != ".xml")
				{
					//cout << availableLevels[i] << " is not a level\n";
					availableLevels.erase(availableLevels.begin() + i);
					i--;
				}
				else
				{
					cout << ending << " " << availableLevels[i] << " is a level\n";
				}
			}
		}
		else
		{
			//cout << availableLevels[i] << " is not a file\n";
			availableLevels.erase(availableLevels.begin() + i);
			i--;
		}
	}
}

bool UI::ScrollLevelList(int i)
{
	int gap = (scrollDownSpr.getPosition().y - 10) - (scrollUpSpr.getPosition().y + (scrollUpTex.getSize().y * math->ZOOM) + 5);
	maxLevelsOnScreen = gap / (levelListFontSize * math->ZOOM);
	if (maxLevelsOnScreen <= 0)
	{
		return false;
	}
	if (availableLevels.size() > maxLevelsOnScreen &&
		topOfLevelList + i >= 0 &&
		topOfLevelList + i + maxLevelsOnScreen <= availableLevels.size())
	{
		topOfLevelList += i;
		printf("Scroll success\n");
	}
	if (topOfLevelList + i < 0)
	{
		topOfLevelList = 0;
		printf("Hit top of list\n");
	}
	if (maxLevelsOnScreen < availableLevels.size() && topOfLevelList + i + maxLevelsOnScreen > availableLevels.size())
	{
		topOfLevelList = availableLevels.size() - maxLevelsOnScreen;
		printf("hit bottom of list\n");
	}
	displayedLevels.clear();
	for (int i = topOfLevelList; i < maxLevelsOnScreen + topOfLevelList; i++)
	{
		if (i < availableLevels.size())
		{
			displayedLevels.push_back(availableLevels[i]);
		}
	}

	levelNamesText.clear();
	for (int i = 0; i < displayedLevels.size(); i++)
	{
		levelNamesText.push_back(sf::Text());
		levelNamesText.back().setCharacterSize((unsigned int)levelListFontSize * math->ZOOM);
		levelNamesText.back().setString(displayedLevels[i]);
		levelNamesText.back().setFont(arialFont);
		levelNamesText.back().setFillColor(sf::Color::Black);
		levelNamesText.back().setPosition(sf::Vector2f(50.0f, 50 + (25.0f * math->ZOOM) + 5 + (i * levelNamesText.back().getCharacterSize())));
		levelNamesText.back().setPosition(math->mapPixelToCoords(levelNamesText.back().getPosition()));
	}

	textBoxHighlight.setPosition(levelNamesText[selectedLevelID].getGlobalBounds().left,
		levelNamesText[selectedLevelID].getGlobalBounds().top);
	textBoxHighlight.setSize(sf::Vector2f(levelNamesText[selectedLevelID].getLocalBounds().width,
		levelNamesText[selectedLevelID].getLocalBounds().height));
	textBoxHighlight.setOutlineThickness(2.0f * math->ZOOM);

	return true;
}

bool UI::ScrollElementsList(int i)
{
	float rightOfLoad = 5 + (100 * math->ZOOM) + 5 + (100 * math->ZOOM) + 5;
	float leftOfSaveBlock = view1->getSize().x - 5 - (50 * math->ZOOM) - 5 - (50 * math->ZOOM) - 5 - (50 * math->ZOOM) - 5 - (15 * math->ZOOM);

	scrollLeftSpr.setPosition(sf::Vector2f(rightOfLoad + 5, 5));
	//scrollLeftSpr.setPosition(math->mapPixelToCoords(scrollLeftSpr.getPosition()));
	scrollLeftSpr.setScale(math->ZOOM, math->ZOOM);

	scrollRightSpr.setPosition(sf::Vector2f(leftOfSaveBlock - 5, 5));
	//scrollRightSpr.setPosition(math->mapPixelToCoords(scrollRightSpr.getPosition()));
	scrollRightSpr.setScale(math->ZOOM, math->ZOOM);

	int gap = (scrollRightSpr.getPosition().x - 5) - ((scrollLeftSpr.getPosition().x + (scrollLeftTex.getSize().x * math->ZOOM)) + 5);
	maxElementsOnScreen = gap / (50 * math->ZOOM);
	if ((elementButtons.size() > maxElementsOnScreen || topOfElementsList > 0) &&
		topOfElementsList + i >= 0 &&
		(topOfElementsList + i + maxElementsOnScreen <= elementButtons.size() || topOfElementsList > 0))
	{
		topOfElementsList += i;
		printf("Scroll success\n");
	}
	if (topOfElementsList + i < 0)
	{
		topOfElementsList = 0;
		printf("Hit left of list\n");
	}
	//if (topOfElementsList + i + maxElementsOnScreen > elementButtons.size())
	//{
	//	topOfElementsList = elementButtons.size() - maxLevelsOnScreen;
	//	printf("hit right of list\n");
	//}
	if (maxElementsOnScreen < elementButtons.size() && topOfElementsList + i + maxElementsOnScreen > elementButtons.size())
	{
		topOfElementsList = elementButtons.size() - maxElementsOnScreen;
		printf("hit right of list\n");
	}

	displayedElementButtons.clear();
	for (int i = topOfElementsList, p = 0; i < maxElementsOnScreen + topOfElementsList; i++, p++)
	{
		if (i < elementButtons.size())
		{
			displayedElementButtons.push_back(elementButtons[i]);
			displayedElementButtons.back()->SetPosition(sf::Vector2f(scrollLeftSpr.getPosition().x + (scrollLeftTex.getSize().x * math->ZOOM) +
				(5 * math->ZOOM) + (p * 50 * math->ZOOM), 5));
			displayedElementButtons.back()->SetScale(math->ZOOM);
		}
	}

	scrollLeftSpr.setPosition(math->mapPixelToCoords(scrollLeftSpr.getPosition()));
	scrollRightSpr.setPosition(math->mapPixelToCoords(scrollRightSpr.getPosition()));

	if (selectedElement > maxElementsOnScreen - 1)
	{
		selectedElement = maxElementsOnScreen - 1;
	}
	if (selectedElement < 0)
	{
		selectedElement = 0;
	}
	displayedElementButtons[selectedElement]->execute();

	displayedElementStats.clear();
	for (int i = topOfElementsList; i < maxElementsOnScreen + topOfElementsList; i++)
	{
		if (i < elementButtons.size())
		{
			displayedElementStats.push_back(elementStats[i]);
		}
	}
	float friction = std::get<0>(displayedElementStats[selectedElement]);
	float bounce = std::get<1>(displayedElementStats[selectedElement]);
	float density = std::get<2>(displayedElementStats[selectedElement]);
	editor->SetCurrentBlockStats(friction, bounce, density);

	elementSelectSpr.setPosition(displayedElementButtons[0]->GetPosition().x + (selectedElement * 50.0f * math->ZOOM), 5.0f);
	elementSelectSpr.setScale(sf::Vector2f(math->ZOOM, math->ZOOM));

	return true;
}

void UI::AddNewElement()
{
	switch (editor->GetCurrentElement())
	{
		case 0: // GRASS
		{
			elementButtons.push_back(new GroundButton
			(sf::Vector2f(250, 5), sf::Vector2f(50, 50), editor));
			float friction = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetFriction();
			float bounce = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetRestitution();
			float density = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetDensity();
			elementStats.push_back({ friction, bounce, density });
			ScrollElementsList(1);
			break;
		}
		case 1: // ICE
		{
			elementButtons.push_back(new IceButton
			(sf::Vector2f(250, 5), sf::Vector2f(50, 50), editor));
			float friction = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetFriction();
			float bounce = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetRestitution();
			float density = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetDensity();
			elementStats.push_back({ friction, bounce, density });
			ScrollElementsList(1);
			break;
		}
		case 2: // WATER
		{
			elementButtons.push_back(new WaterButton
			(sf::Vector2f(250, 5), sf::Vector2f(50, 50), editor));
			float friction = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetFriction();
			float bounce = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetRestitution();
			float density = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetDensity();
			elementStats.push_back({ friction, bounce, density });
			ScrollElementsList(1);
			break;
		}
		case 3: // METAL
		{
			elementButtons.push_back(new MetalButton
			(sf::Vector2f(250, 5), sf::Vector2f(50, 50), editor));
			float friction = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetFriction();
			float bounce = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetRestitution();
			float density = editor->GetBlockBody(editor->GetCurrentBlock())->GetFixtureList()->GetDensity();
			elementStats.push_back({ friction, bounce, density });
			ScrollElementsList(1);
			break;
		}
		case 4: // SPAWN
		{
			break;
		}
		case 5: // FINISH
		{
			break;
		}
	}
}
void UI::DeleteElement(sf::RenderWindow &w)
{
	for (int i = 0; i < displayedElementButtons.size(); i++)
	{
		if (sf::Mouse::getPosition(w).x >= displayedElementButtons[i]->GetPosition().x &&
			sf::Mouse::getPosition(w).x <= displayedElementButtons[i]->GetPosition().x + (displayedElementButtons[i]->GetSize().x * math->ZOOM) &&
			sf::Mouse::getPosition(w).y >= displayedElementButtons[i]->GetPosition().y &&
			sf::Mouse::getPosition(w).y <= displayedElementButtons[i]->GetPosition().y + (displayedElementButtons[i]->GetSize().y * math->ZOOM))
		{
			int elementToDelete = topOfElementsList + i;
			if (elementToDelete > 5)
			{
				elementButtons.erase(elementButtons.begin() + elementToDelete);
				elementStats.erase(elementStats.begin() + elementToDelete);
				displayedElementButtons.clear();
				float rightOfLoad = 5 + (100 * math->ZOOM) + 5 + (100 * math->ZOOM) + 5 + 5;
				for (int i = topOfElementsList, p = 0; i < maxElementsOnScreen + topOfElementsList; i++, p++)
				{
					if (i < elementButtons.size())
					{
						displayedElementButtons.push_back(elementButtons[i]);
						displayedElementButtons.back()->SetPosition(sf::Vector2f(rightOfLoad + (scrollLeftTex.getSize().x * math->ZOOM) +
							(5 * math->ZOOM) + (p * 50 * math->ZOOM), 5));
						displayedElementButtons.back()->SetPosition(displayedElementButtons.back()->GetPosition());
						displayedElementButtons.back()->SetScale(math->ZOOM);
					}
				}

				selectedElement = 0;
				elementSelectSpr.setPosition(displayedElementButtons[0]->GetPosition().x + (selectedElement * 50.0f * math->ZOOM), 5.0f);
				displayedElementButtons[selectedElement]->execute();

				displayedElementStats.clear();
				for (int i = topOfElementsList; i < maxElementsOnScreen + topOfElementsList; i++)
				{
					if (i < elementButtons.size())
					{
						displayedElementStats.push_back(elementStats[i]);
					}
				}
			}
		}
	}
}

void UI::DisplayTooltips(sf::RenderWindow &w)
{
	for (int i = 0; i < displayedElementButtons.size(); i++)
	{
		if (sf::Mouse::getPosition(w).x >= displayedElementButtons[i]->GetPosition().x &&
			sf::Mouse::getPosition(w).x <= displayedElementButtons[i]->GetPosition().x + (displayedElementButtons[i]->GetSize().x * math->ZOOM) &&
			sf::Mouse::getPosition(w).y >= displayedElementButtons[i]->GetPosition().y &&
			sf::Mouse::getPosition(w).y <= displayedElementButtons[i]->GetPosition().y + (displayedElementButtons[i]->GetSize().y * math->ZOOM))
		{
			string type = "";
			if (displayedElementButtons[i]->GetType() == 1)
			{
				type = "Grass Block\n";
			}
			if (displayedElementButtons[i]->GetType() == 2)
			{
				type = "Ice Block\n";
			}
			if (displayedElementButtons[i]->GetType() == 3)
			{
				type = "Water Block\n";
			}
			if (displayedElementButtons[i]->GetType() == 4)
			{
				type = "Metal Block\n";
			}
			if (type != "")
			{
				string tip;
				tip = type + "Friction: " + math->TrimZerosFromFloat(std::to_string(std::get<0>(displayedElementStats[i]))) +
					"\nBounce: " + math->TrimZerosFromFloat(std::to_string(std::get<1>(displayedElementStats[i]))) +
					"\nDensity: " + math->TrimZerosFromFloat(std::to_string(std::get<2>(displayedElementStats[i])));
				Tooltip* elementTip = new Tooltip(sf::Vector2f(math->mapPixelToCoords((sf::Vector2f)sf::Mouse::getPosition(w)) + sf::Vector2f(10, 10)), tip, math->ZOOM);
				elementTip->Draw(w);
				delete elementTip;
			}

		}
	}
}

void UI::WindowResized()
{
	top_UI_box.setOutlineThickness(2.0f * math->ZOOM);
	top_UI_box.setPosition(sf::Vector2f(-2, -2));
	top_UI_box.setSize(sf::Vector2f(view1->getSize().x + 4, 57 * math->ZOOM));

	saveLevelUISpr.setPosition((view1->getSize().x / 2) - ((saveLevelUITex.getSize().x / 2) * math->ZOOM),
		(view1->getSize().y / 2) - ((saveLevelUITex.getSize().y / 2) * math->ZOOM));
	saveLevelUISpr.setScale(math->ZOOM, math->ZOOM);

	switch (mode)
	{
		case SAVING:
		{
			textBoxHighlight.setOutlineThickness(2.0f * math->ZOOM);
			textBoxHighlight.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((600 / 2) * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM)));
			textBoxHighlight.setSize(sf::Vector2f(600 * math->ZOOM, 50 * math->ZOOM));
			break;
		}
		case LOADING:
		{
			ScrollLevelList(0);
			selectedLevelID = 0;
			if (levelNamesText.size() > 0)
			{
				textBoxHighlight.setPosition(levelNamesText[selectedLevelID].getGlobalBounds().left,
					levelNamesText[selectedLevelID].getGlobalBounds().top);
				textBoxHighlight.setSize(sf::Vector2f(levelNamesText[selectedLevelID].getLocalBounds().width,
					levelNamesText[selectedLevelID].getLocalBounds().height));
				textBoxHighlight.setOutlineThickness(2.0f * math->ZOOM);
				editor->SetNameOfLevelToLoad(levelNamesText[selectedLevelID].getString());
			}

			break;
		}
		case EDITING:
		{
			blockAttributeSpr.setPosition(sf::Vector2f((view1->getSize().x / 2) - (350 * math->ZOOM), (view1->getSize().y / 2) - (100 * math->ZOOM)));
			saveChangesSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (300 + math->ZOOM),
				blockAttributeSpr.getPosition().y + (145 * math->ZOOM)));

			blockAttributeSpr.setScale(math->ZOOM, math->ZOOM);
			saveChangesSpr.setScale(math->ZOOM, math->ZOOM);
			bounceValText.setCharacterSize(20 * math->ZOOM);
			frictionValText.setCharacterSize(20 * math->ZOOM);
			densityValText.setCharacterSize(20 * math->ZOOM);
			positionXValText.setCharacterSize(20 * math->ZOOM);
			positionYValText.setCharacterSize(20 * math->ZOOM);
			rotationValText.setCharacterSize(20 * math->ZOOM);

			for (int i = 0; i < 4; i++)
			{
				attributeSprVec[i].setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (155 * math->ZOOM),
					blockAttributeSpr.getPosition().y + ((12 + (i * 26)) * math->ZOOM)));
				attributeSprVec[i].setPosition(math->mapPixelToCoords(attributeSprVec[i].getPosition()));
			}

			positionXTextBoxSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (155 * math->ZOOM),
				blockAttributeSpr.getPosition().y + ((12 + (4 * 26)) * math->ZOOM)));
			positionXTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
			positionYTextBoxSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + ((155 + 89 + 50) * math->ZOOM),
				blockAttributeSpr.getPosition().y + ((12 + (4 * 26)) * math->ZOOM)));
			positionYTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

			if ((int)attribute < 4)
			{
				textBoxHighlight.setPosition(attributeSprVec[(int)attribute].getPosition());
				textBoxHighlight.setSize(((sf::Vector2f)attributeSprVec[(int)attribute].getTexture()->getSize() / 3.0f) * math->ZOOM);
			}
			if ((int)attribute == 4)
			{
				textBoxHighlight.setPosition(positionXTextBoxSpr.getPosition());
				textBoxHighlight.setSize(((sf::Vector2f)positionXTextBoxSpr.getTexture()->getSize() / 3.0f) * math->ZOOM);
			}
			if ((int)attribute == 5)
			{
				textBoxHighlight.setPosition(positionYTextBoxSpr.getPosition());
				textBoxHighlight.setSize(((sf::Vector2f)positionYTextBoxSpr.getTexture()->getSize() / 3.0f) * math->ZOOM);
			}
		}
	}

	levelNameBoxSpr.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
		- ((600 / 2) * math->ZOOM),
		saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
		- ((50.0f / 2.0f) * math->ZOOM)));
	levelNameBoxSpr.setScale(math->ZOOM, math->ZOOM);

	nameOfLevel.setCharacterSize(40 * math->ZOOM);
	nameOfLevel.setPosition(sf::Vector2f(levelNameBoxSpr.getPosition().x + (300 * math->ZOOM),
		levelNameBoxSpr.getPosition().y + (25 * math->ZOOM)));

	///// REPOSITIONING BUTTONS /////
	for (int i = 0; i < buttons.size(); i++)
	{
		string buttonName = buttons[i]->GetName();
		if (buttonName == "SaveFile")
		{
			//buttons[1]
			buttons[i]->SetPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((100 / 2) * math->ZOOM) - (60 * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM) + (60 * math->ZOOM)));
		}
		else if (buttonName == "SaveMenu")
		{
			buttons[i]->SetPosition(sf::Vector2f(5, 5));
		}
		else if (buttonName == "LoadMenu")
		{
			buttons[i]->SetPosition(sf::Vector2f(5 + (100 * math->ZOOM) + 5, 5));
		}
		else if (buttonName == "LoadFile")
		{
			//buttons[i]->SetPosition(sf::Vector2f((100 * math->ZOOM) + (15 * math->ZOOM), 5));
		}
		else if (buttonName == "Delete")
		{
			buttons[i]->SetPosition(sf::Vector2f(view1->getSize().x - 5 - (buttons[i]->GetSize().x * math->ZOOM), 5));
		}
		else if (buttonName == "Option")
		{
			buttons[i]->SetPosition(sf::Vector2f(view1->getSize().x - 5 - 5 - (buttons[i]->GetSize().x * math->ZOOM * 2), 5));
		}
		else if (buttonName == "Play")
		{
			buttons[i]->SetPosition(sf::Vector2f(0, view1->getSize().y - (buttons[i]->GetSize().y * math->ZOOM)));
		}
		else if (buttonName == "Cancel")
		{
			buttons[i]->SetPosition(sf::Vector2f
			(saveLevelUISpr.getPosition().x + ((saveLevelUITex.getSize().x / 2) * math->ZOOM)
				- ((120 / 2) * math->ZOOM) + (60 * math->ZOOM),
				saveLevelUISpr.getPosition().y + ((saveLevelUITex.getSize().y / 2) * math->ZOOM)
				- ((50.0f / 2.0f) * math->ZOOM) + (60 * math->ZOOM)));
		}
		buttons[i]->SetScale(math->ZOOM);
	}

	saveBlockSpr.setPosition(sf::Vector2f(view1->getSize().x - 5 - 5 - 5 - (50 * math->ZOOM * 3), 5));
	saveBlockSpr.setScale(sf::Vector2f(math->ZOOM, math->ZOOM));

	backgroundBox.setPosition(0, 70 * math->ZOOM);
	backgroundBox.setSize(sf::Vector2f(80 * math->ZOOM, 420 * math->ZOOM));

	backgroundBtnSpr.setPosition(0, 57 * math->ZOOM);
	backgroundBtnSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

	currentBGSpr.setPosition(5 * math->ZOOM, 62 * math->ZOOM);
	currentBGSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

	scrollUpSpr.setPosition(sf::Vector2f(200, 50));
	scrollDownSpr.setPosition(sf::Vector2f(200, view1->getSize().y - (100 * math->ZOOM)));

	shapeSelectBoxSpr.setPosition((view1->getSize().x / 2.0f) - ((shapeSelectBoxTex.getSize().x / 3.0f) / 2.0f) * math->ZOOM, 57 * math->ZOOM);
	shapeSelectBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
	shapeSelectIconSpr.setPosition(((view1->getSize().x / 2.0f) - ((shapeSelectBoxTex.getSize().x / 3.0f) / 2.0f) + 3) * math->ZOOM, 60 * math->ZOOM);
	shapeSelectIconSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

	gravitySelectBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
	gravitySelectBoxSpr.setPosition((shapeSelectBoxSpr.getPosition().x + 456) * math->ZOOM, (shapeSelectBoxSpr.getPosition().y + 85) * math->ZOOM);

	///// CHANGING ELEMENTS LIST /////
	ScrollElementsList(0);
}

bool UI::CanZoom(float zoom)
{
	if ((70 * zoom) + (420 * zoom) >= view1->getSize().y - (50 * zoom))
	{
		printf("Can't zoom, Background list won't fit\n");
		return false;
	}

	float rightOfLoad = 5 + (100 * zoom) + 5 + (100 * zoom) + 5 + (15 * zoom) + 5;
	sf::Vector2f scrollLeft = sf::Vector2f(rightOfLoad, 5);
	scrollLeft = math->mapPixelToCoords(scrollLeft);

	float leftOfSaveBlock = view1->getSize().x - 5 - (50 * zoom) - 5 - (50 * zoom) - 5 - (50 * zoom) - 5 - (15 * zoom) - 5;
	sf::Vector2f scrollRight = math->mapPixelToCoords(sf::Vector2f(leftOfSaveBlock, 5));

	int gap = (leftOfSaveBlock - 5) - (rightOfLoad + 5);
	int temp = gap / (50 * zoom);
	if (temp <= 0)
	{
		printf("Can't zoom, Elements won't fit\n");
		return false;
	}

	gap = (scrollDownSpr.getPosition().y - 10) - (scrollUpSpr.getPosition().y + (scrollUpTex.getSize().y * math->ZOOM) + 5);
	temp = gap / (levelListFontSize * zoom);
	if (temp <= 0)
	{
		printf("Can't zoom, levels won't fit\n");
		return false;
	}

	return true;
}
void UI::SwitchMode(int m)
{
	mode = (Mode)m;
	for (int i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
	}
	buttons.clear();
	enteringText = false;
	player->SetPlaying(false);
	if (mode == BUILDING)
	{
		buttons.push_back(new SaveMenuButton
		(sf::Vector2f(5, 5), sf::Vector2f(100, 50), editor, fileMgr));

		buttons.push_back(new LoadMenuButton
		(sf::Vector2f(120, 5), sf::Vector2f(100, 50), editor, fileMgr));

		buttons.push_back(new DeleteButton
		(sf::Vector2f(700, 5), sf::Vector2f(50, 50), editor, fileMgr));

		buttons.push_back(new OptionButton
		(sf::Vector2f(645, 5), sf::Vector2f(50, 50)));

		buttons.push_back(new EditBlockButton
		(sf::Vector2f(0, 0), sf::Vector2f(60, 25)));

		buttons.push_back(new PlayButton
		(sf::Vector2f(0, view1->getSize().y - 50), sf::Vector2f(100, 50), editor));

		view1->setCenter(view1->getSize() / 2.0f);

		//editor->ResetLevel();
	}
	else if (mode == SAVING)
	{
		buttons.push_back(new SaveFileButton
		(sf::Vector2f(saveLevelUISpr.getPosition().x + (saveLevelUITex.getSize().x / 2)
			- (100 / 2) - 60,
			saveLevelUISpr.getPosition().y + (saveLevelUITex.getSize().y / 2)
			- (50.0f / 2.0f) + 60),
			sf::Vector2f(100, 50), editor, fileMgr));

		buttons.push_back(new CancelButton
		(sf::Vector2f(saveLevelUISpr.getPosition().x + (saveLevelUITex.getSize().x / 2)
			- (120 / 2) + 60,
			saveLevelUISpr.getPosition().y + (saveLevelUITex.getSize().y / 2)
			- (50.0f / 2.0f) + 60),
			sf::Vector2f(120, 50), editor));
	}
	else if (mode == LOADING)
	{
		buttons.push_back(new LoadFileButton
		(sf::Vector2f((view1->getSize().x / 2) - 60,
			view1->getSize().y - 50),
			sf::Vector2f(100, 50), editor, fileMgr));

		buttons.push_back(new CancelButton
		(sf::Vector2f((view1->getSize().x / 2) + 60,
			view1->getSize().y - 50),
			sf::Vector2f(120, 50), editor));

		LoadLevelNames();
		topOfLevelList = 0;
		ScrollLevelList(0);
	}
	else if (mode == EDITING)
	{
		buttons.push_back(new CancelButton
		(sf::Vector2f((view1->getSize().x / 2) + 200,
			(view1->getSize().y / 2) + 50),
			sf::Vector2f(120, 50), editor));

		saveChangesSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (350 + math->ZOOM) - ((saveChangesTex.getSize().x / 2) * math->ZOOM),
			blockAttributeSpr.getPosition().y + (145 * math->ZOOM)));

		string bVal = std::to_string(editor->GetBounciness(editor->GetCurrentBlock()));
		string fVal = std::to_string(editor->GetFriction(editor->GetCurrentBlock()));
		string dVal = std::to_string(editor->GetDensity(editor->GetCurrentBlock()));
		string xVal = std::to_string(editor->PositionOfSelectedShape().x);
		string yVal = std::to_string(editor->PositionOfSelectedShape().y);
		string rVal = std::to_string(editor->GetRotation(editor->GetCurrentBlock()));
		bounceValText.setString(bVal);
		frictionValText.setString(fVal);
		densityValText.setString(dVal);
		positionXValText.setString(xVal);
		positionYValText.setString(yVal);
		rotationValText.setString(rVal);
		//enteringText = true;
	}
	else if (mode == PLAYING)
	{
		buttons.push_back(new BuildButton
		(sf::Vector2f(0, view1->getSize().y - 50),
			sf::Vector2f(100, 50), editor));

		editor->SaveDefaultLevel();
		player->SetPlaying(true);
		player->SetPosition(sf::Vector2f(editor->GetPlayerSpawn().x / 1, editor->GetPlayerSpawn().y / 1));
		player->SetVelocity(sf::Vector2f(0, 0));
	}
	else if (mode == OPTIONS)
	{

	}

	WindowResized();
}
void UI::LoadContent()
{
	shapeSelectBoxTex.loadFromFile("Textures/UI/shapeSelect.png");
	shapeSelectBoxSpr.setTexture(shapeSelectBoxTex);
	shapeSelectBoxSpr.setPosition((view1->getSize().x / 2.0f) - (shapeSelectBoxTex.getSize().x / 2.0f) * math->ZOOM, 57 * math->ZOOM);

	shapeSelectIconTex.loadFromFile("Textures/UI/shapeSelectBox.png");
	shapeSelectIconSpr.setTexture(shapeSelectIconTex);
	shapeSelectIconSpr.setPosition(((view1->getSize().x / 2.0f) - (shapeSelectBoxTex.getSize().x / 2.0f) + 3) * math->ZOOM, 57 * math->ZOOM);

	gravitySelectBoxTex.loadFromFile("Textures/UI/gravitySelect.png");
	gravitySelectBoxSpr.setTexture(gravitySelectBoxTex);
	gravitySelectBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
	gravitySelectBoxSpr.setPosition((shapeSelectBoxSpr.getPosition().x + 456) * math->ZOOM, (shapeSelectBoxSpr.getPosition().y + 85) * math->ZOOM);

	elementSelectTex.loadFromFile("Textures/Buttons/elementSelect.png");
	elementSelectSpr.setTexture(elementSelectTex);

	saveBlockTex.loadFromFile("Textures/Buttons/buttonSaveBlockSmall.png");
	saveBlockSpr.setTexture(saveBlockTex);
	saveBlockSpr.setPosition(sf::Vector2f(view1->getSize().x - 5 - 5 - 5 - (50 * math->ZOOM * 3), 5));

	saveLevelUITex.loadFromFile("Textures/UI/saveLevelUIBox.png");
	saveLevelUISpr.setTexture(saveLevelUITex);
	saveLevelUISpr.setPosition((view1->getSize().x / 2) - (saveLevelUITex.getSize().x / 2),
		(view1->getSize().y / 2) - (saveLevelUITex.getSize().y / 2));

	levelNameBoxTex.loadFromFile("Textures/UI/textBox.png");
	levelNameBoxSpr.setTexture(levelNameBoxTex);
	levelNameBoxSpr.setPosition(sf::Vector2f(saveLevelUISpr.getPosition().x + (saveLevelUITex.getSize().x / 2)
		- (600 / 2),
		saveLevelUISpr.getPosition().y + (saveLevelUITex.getSize().y / 2)
		- (50.0f / 2.0f)));

	scrollDownTex.loadFromFile("Textures/UI/scrollDownButton.png");
	scrollDownSpr.setTexture(scrollDownTex);
	scrollDownSpr.setPosition(sf::Vector2f(200, view1->getSize().y - 100));

	scrollUpTex.loadFromFile("Textures/UI/scrollUpButton.png");
	scrollUpSpr.setTexture(scrollUpTex);
	scrollUpSpr.setPosition(sf::Vector2f(200, 50));

	scrollLeftTex.loadFromFile("Textures/UI/scrollLeftButton.png");
	scrollLeftSpr.setTexture(scrollLeftTex);
	float rightOfLoad = 5 + (100 * math->ZOOM) + 5 + (100 * math->ZOOM) + 5;
	scrollLeftSpr.setPosition(sf::Vector2f(rightOfLoad + 5, 5));

	scrollRightTex.loadFromFile("Textures/UI/scrollRightButton.png");
	scrollRightSpr.setTexture(scrollRightTex);
	scrollRightSpr.setPosition(sf::Vector2f(saveBlockSpr.getPosition().x - 20, 5));

	blockAttributeTex.loadFromFile("Textures/UI/blockAttributeUI.png");
	blockAttributeSpr.setTexture(blockAttributeTex);
	blockAttributeSpr.setPosition(sf::Vector2f((view1->getSize().x / 2) - 350, 
		(view1->getSize().y / 2) - 100));
	blockAttributeSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

	bounceTextBoxTex.loadFromFile("Textures/UI/bounceAttributeUITextBox.png");
	bounceTextBoxSpr.setTexture(bounceTextBoxTex);

	for (int i = 0; i < 4; i++)
	{
		attributeSprVec.push_back(bounceTextBoxSpr);
		attributeSprVec.back().setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + (100 * math->ZOOM),
			blockAttributeSpr.getPosition().y + ((10 + (i + 20)) * math->ZOOM)));
		attributeSprVec.back().setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
	}

	positionTextBoxTex.loadFromFile("Textures/UI/positionAttributeUITextBox.png");
	positionXTextBoxSpr.setTexture(positionTextBoxTex);
	positionXTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);
	positionYTextBoxSpr.setTexture(positionTextBoxTex);
	positionYTextBoxSpr.setScale((1.0f / 3.0f) * math->ZOOM, (1.0f / 3.0f) * math->ZOOM);

	saveChangesTex.loadFromFile("Textures/Buttons/buttonSaveChanges.png");
	saveChangesSpr.setTexture(saveChangesTex);
	saveChangesSpr.setPosition(sf::Vector2f(blockAttributeSpr.getPosition().x + 300, 
		blockAttributeSpr.getPosition().y + 145));

	arialFont.loadFromFile("Textures/Fonts/arial.ttf");

	nameOfLevel.setString("");
	nameOfLevel.setCharacterSize(40);
	nameOfLevel.setFont(arialFont);
	nameOfLevel.setPosition(sf::Vector2f(levelNameBoxSpr.getPosition().x + 300, 
		levelNameBoxSpr.getPosition().y + 25));
	nameOfLevel.setFillColor(sf::Color::Black);

	bounceValText.setString("");
	bounceValText.setCharacterSize(20);
	bounceValText.setFont(arialFont);
	bounceValText.setFillColor(sf::Color::Black);

	frictionValText.setString("");
	frictionValText.setCharacterSize(20);
	frictionValText.setFont(arialFont);
	frictionValText.setFillColor(sf::Color::Black);

	densityValText.setString("");
	densityValText.setCharacterSize(20);
	densityValText.setFont(arialFont);
	densityValText.setFillColor(sf::Color::Black);

	positionXValText.setString("");
	positionXValText.setCharacterSize(20);
	positionXValText.setFont(arialFont);
	positionXValText.setFillColor(sf::Color::Black);

	positionYValText.setString("");
	positionYValText.setCharacterSize(20);
	positionYValText.setFont(arialFont);
	positionYValText.setFillColor(sf::Color::Black);

	rotationValText.setString("");
	rotationValText.setCharacterSize(20);
	rotationValText.setFont(arialFont);
	rotationValText.setFillColor(sf::Color::Black);

	cameraPosText.setString("");
	cameraPosText.setCharacterSize(20);
	cameraPosText.setFont(arialFont);
	cameraPosText.setPosition(sf::Vector2f(view1->getSize().x - cameraPosText.getLocalBounds().width, 57));
	cameraPosText.setFillColor(sf::Color::Black);

	cameraTex.loadFromFile("Textures/UI/cameraIcon.png");
	cameraSpr.setTexture(cameraTex);
	cameraSpr.setScale((1.0f / 4.0f) * math->ZOOM, (1.0f / 4.0f) * math->ZOOM);
	cameraSpr.setPosition(cameraPosText.getPosition().x - (cameraTex.getSize().x * cameraSpr.getScale().x), 57);

	backgroundBtnTex.loadFromFile("Textures/Buttons/buttonBG.png");
	backgroundBtnSpr.setTexture(backgroundBtnTex);
	backgroundBtnSpr.setPosition(0, 57);
	backgroundBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	grassBtnTex.loadFromFile("Textures/UI/buttonGrass.png");
	grassBtnSpr.setTexture(grassBtnTex);
	grassBtnSpr.setPosition(5, 62);
	grassBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	caveBtnTex.loadFromFile("Textures/UI/buttonCave.png");
	caveBtnSpr.setTexture(caveBtnTex);
	caveBtnSpr.setPosition(0, 100);
	caveBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	cityBtnTex.loadFromFile("Textures/UI/buttonCity.png");
	cityBtnSpr.setTexture(cityBtnTex);
	cityBtnSpr.setPosition(0, 100);
	cityBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	desertBtnTex.loadFromFile("Textures/UI/buttonDesert.png");
	desertBtnSpr.setTexture(desertBtnTex);
	desertBtnSpr.setPosition(0, 100);
	desertBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	mountainBtnTex.loadFromFile("Textures/UI/buttonMountain.png");
	mountainBtnSpr.setTexture(mountainBtnTex);
	mountainBtnSpr.setPosition(0, 100);
	mountainBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	skyBtnTex.loadFromFile("Textures/UI/buttonSky.png");
	skyBtnSpr.setTexture(skyBtnTex);
	skyBtnSpr.setPosition(0, 100);
	skyBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	spaceBtnTex.loadFromFile("Textures/UI/buttonSpace.png");
	spaceBtnSpr.setTexture(spaceBtnTex);
	spaceBtnSpr.setPosition(0, 100);
	spaceBtnSpr.setScale(1.0f / 3.0f, 1.0f / 3.0f);

	BGBtnSprVec.push_back(grassBtnSpr);
	BGBtnSprVec.push_back(caveBtnSpr);
	BGBtnSprVec.push_back(cityBtnSpr);
	BGBtnSprVec.push_back(desertBtnSpr);
	BGBtnSprVec.push_back(mountainBtnSpr);
	BGBtnSprVec.push_back(skyBtnSpr);
	BGBtnSprVec.push_back(spaceBtnSpr);

	currentBGSpr = grassBtnSpr;
}