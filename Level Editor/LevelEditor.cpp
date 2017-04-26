#include "LevelEditor.h"

LevelEditor::LevelEditor(sf::View* v, MathMethods* m) : m_world(b2Vec2(0, 9.81 * 8 * 0))
{
	cameraPos = sf::Vector2f(0, 0);
	defaultGravity = sf::Vector2f(0, 9.81 * 8);

	shapeHighlight.setFillColor(sf::Color::Blue);

	shapeSelected = false;
	selectedShapeID = -1;
	highlightThickness = 2;
	zoomLevel = 1.0f;

	v->setViewport(sf::FloatRect(0, 0, 1, 1));
	v->zoom(1);
	view1 = v;
	math = m;

	LoadContent();

	currentBlockStats = { 2.5f, 0.1, 1 };
}

void LevelEditor::Update(sf::RenderWindow &w)
{
	if (stopDrawing)
	{
		delete tempBlock;
		switch (shape)
		{
			case RECTANGLE:
			{
				switch (tile)
				{
				case GRASS:
				{
					mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);
					if (mouseUpPos.x < mouseDownPos.x)
					{
						std::swap(mouseUpPos.x, mouseDownPos.x);
					}
					if (mouseUpPos.y < mouseDownPos.y)
					{
						std::swap(mouseUpPos.y, mouseDownPos.y);
					}
					sf::Vector2f size;
					size.x = abs(mouseDownPos.x - mouseUpPos.x);
					size.y = abs(mouseDownPos.y - mouseUpPos.y);
					ObjectData* myData = new ObjectData;
					myData->type = 1;
					myData->density = 1;
					myData->mass = -1;
					sf::Vector2f pos = (sf::Vector2f)mouseDownPos + cameraPos;

					cout << "Creating block with stats: Friction: " << std::get<0>(currentBlockStats)
						<< "|Bounce: " << std::get<1>(currentBlockStats) << "|Density: " << std::get<2>(currentBlockStats) << endl;
					blocks.push_back(new GrassBlock((sf::Vector2f)mouseDownPos + cameraPos, size, 0, m_world, myData,
						GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), isKine));
					blocks.back()->SetGravity(defaultGravity);
					if (!blocks.back()->Valid())
					{
						delete blocks.back();
						blocks.erase(blocks.end() - 1);
					}
					break;
				}
				case ICE:
				{
					mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);
					if (mouseUpPos.x < mouseDownPos.x)
					{
						std::swap(mouseUpPos.x, mouseDownPos.x);
					}
					if (mouseUpPos.y < mouseDownPos.y)
					{
						std::swap(mouseUpPos.y, mouseDownPos.y);
					}
					sf::Vector2f size;
					size.x = abs(mouseDownPos.x - mouseUpPos.x);
					size.y = abs(mouseDownPos.y - mouseUpPos.y);
					ObjectData* myData = new ObjectData;
					myData->type = 1;
					myData->density = 1;
					myData->mass = -1;
					sf::Vector2f pos = (sf::Vector2f)mouseDownPos + cameraPos;

					cout << "Creating block with stats: Friction: " << std::get<0>(currentBlockStats)
						<< "|Bounce: " << std::get<1>(currentBlockStats) << "|Density: " << std::get<2>(currentBlockStats) << endl;
					blocks.push_back(new IceBlock((sf::Vector2f)mouseDownPos + cameraPos, size, 0, m_world, myData,
						GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), isKine));
					blocks.back()->SetGravity(defaultGravity);
					if (!blocks.back()->Valid())
					{
						delete blocks.back();
						blocks.erase(blocks.end() - 1);
					}
					break;
				}
				case METAL:
				{
					mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);
					if (mouseUpPos.x < mouseDownPos.x)
					{
						std::swap(mouseUpPos.x, mouseDownPos.x);
					}
					if (mouseUpPos.y < mouseDownPos.y)
					{
						std::swap(mouseUpPos.y, mouseDownPos.y);
					}
					sf::Vector2f size;
					size.x = abs(mouseDownPos.x - mouseUpPos.x);
					size.y = abs(mouseDownPos.y - mouseUpPos.y);
					ObjectData* myData = new ObjectData;
					myData->type = 1;
					myData->density = 1;
					myData->mass = -1;
					sf::Vector2f pos = (sf::Vector2f)mouseDownPos + cameraPos;

					cout << "Creating block with stats: Friction: " << std::get<0>(currentBlockStats)
						<< "|Bounce: " << std::get<1>(currentBlockStats) << "|Density: " << std::get<2>(currentBlockStats) << endl;
					blocks.push_back(new MetalBlock((sf::Vector2f)mouseDownPos + cameraPos, size, 0, m_world, myData,
						GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), isKine));
					blocks.back()->SetGravity(defaultGravity);
					if (!blocks.back()->Valid())
					{
						delete blocks.back();
						blocks.erase(blocks.end() - 1);
					}
					break;
				}
				case WATER:
				{
					mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);
					if (mouseUpPos.x < mouseDownPos.x)
					{
						std::swap(mouseUpPos.x, mouseDownPos.x);
					}
					if (mouseUpPos.y < mouseDownPos.y)
					{
						std::swap(mouseUpPos.y, mouseDownPos.y);
					}
					sf::Vector2f size;
					size.x = abs(mouseDownPos.x - mouseUpPos.x);
					size.y = abs(mouseDownPos.y - mouseUpPos.y);
					ObjectData* myData = new ObjectData;
					myData->type = 1;
					myData->density = 0.5f;
					myData->mass = -1;

					cout << "Creating block with stats: Friction: " << std::get<0>(currentBlockStats)
						<< "|Bounce: " << std::get<1>(currentBlockStats) << "|Density: " << std::get<2>(currentBlockStats) << endl;
					blocks.push_back(new WaterBlock((sf::Vector2f)mouseDownPos + cameraPos, size, 0, m_world, myData,
						WATERBLOCK, GRASSBLOCK | WATERBLOCK, 0, 0, std::get<2>(currentBlockStats), isKine));
					blocks.back()->SetGravity(defaultGravity);
					if (!blocks.back()->Valid())
					{
						delete blocks.back();
						blocks.erase(blocks.end() - 1);
					}
					break;
				}
				break;
				}
			}
			break;
			case CIRCLE:
			{
				mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);
				float size = math->DistanceBetweenPoints((sf::Vector2f)mouseDownPos, (sf::Vector2f)mouseUpPos);

				cout << "Creating block with stats: Friction: " << std::get<0>(currentBlockStats)
					<< "|Bounce: " << std::get<1>(currentBlockStats) << "|Density: " << std::get<2>(currentBlockStats) << endl;
				blocks.push_back(new CircleBlock((sf::Vector2f)mouseDownPos, size, m_world, NULL, GRASSBLOCK, GRASSBLOCK | BALL, 
					std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), isKine));
				blocks.back()->SetGravity(defaultGravity);
				if (!blocks.back()->Valid())
				{
					delete blocks.back();
					blocks.erase(blocks.end() - 1);
				}
			}
			break;
			case LINE:
			{
				mouseUpPos = (sf::Vector2i)w.mapPixelToCoords(mouseUpPos);

				sf::Vector2f direction = (sf::Vector2f)mouseUpPos - (sf::Vector2f)mouseDownPos;
				float l = math->DistanceBetweenPoints(direction, sf::Vector2f(0, 0));
				sf::Vector2f normalized = sf::Vector2f(direction.x / l, direction.y / l);

				float dot = math->DotProduct(normalized, sf::Vector2f(1, 0));
				float det = math->Determinant(normalized, sf::Vector2f(1, 0));
				float angle = atan2(det, dot) * (180 / 3.14f);
				float length = math->DistanceBetweenPoints((sf::Vector2f)mouseUpPos, (sf::Vector2f)mouseDownPos);

				blocks.push_back(new LineBlock(((sf::Vector2f)mouseDownPos + (sf::Vector2f)mouseUpPos) / 2.0f, length, m_world, NULL,
					GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), isKine));
				blocks.back()->SetGravity(defaultGravity);
				blocks.back()->SetRotation(angle);
				if (!blocks.back()->Valid())
				{
					delete blocks.back();
					blocks.erase(blocks.end() - 1);
				}
				break;
			}
			break;
		}
	}
	stopDrawing = false;
	UpdatePreview(w);
}

void LevelEditor::UpdatePreview(sf::RenderWindow &w) // Draw preview for block about to be placed
{
	switch (shape)
	{
		case RECTANGLE:
		{
			w.setView(*view1);
			mouseCurrentPos = sf::Mouse::getPosition(w);
			if (startDrawing)
			{
				sf::Vector2i originalPos = mouseDownPos;
				if (mouseCurrentPos.y > 55.0f) // Cursor must be below top UI bar
				{
					mouseCurrentPos = (sf::Vector2i)w.mapPixelToCoords(mouseCurrentPos);
					sf::Vector2i tempPos = originalPos;
					if (mouseCurrentPos.x < mouseDownPos.x)
					{
						tempPos.x = mouseCurrentPos.x;
					}
					if (mouseCurrentPos.y < mouseDownPos.y)
					{
						tempPos.y = mouseCurrentPos.y;
					}
					tempBlock->SetPosition((sf::Vector2f)tempPos);
					sf::Vector2f tempSize = sf::Vector2f(abs(mouseDownPos.x - mouseCurrentPos.x),
						abs(mouseDownPos.y - mouseCurrentPos.y));
					tempBlock->Resize(tempSize);
				}
			}
		}
		break;
		case CIRCLE:
		{
			w.setView(*view1);
			mouseCurrentPos = sf::Mouse::getPosition(w);
			if (startDrawing)
			{
				sf::Vector2i originalPos = mouseDownPos;
				if (mouseCurrentPos.y > 55.0f) // Cursor must be below top UI bar
				{
					mouseCurrentPos = (sf::Vector2i)w.mapPixelToCoords(mouseCurrentPos);
					sf::Vector2i tempPos = originalPos;
					float r = math->DistanceBetweenPoints((sf::Vector2f)mouseDownPos, (sf::Vector2f)mouseCurrentPos);
					tempBlock->SetPosition((sf::Vector2f)mouseDownPos);
					cout << "dist: " << r << endl;
					tempBlock->Resize(sf::Vector2f(r, r));
				}
			}
		}
		break;
		case LINE:
		{
			w.setView(*view1);
			w.setView(*view1);
			mouseCurrentPos = sf::Mouse::getPosition(w);
			if (startDrawing)
			{
				sf::Vector2i originalPos = mouseDownPos;
				if (mouseCurrentPos.y > 55.0f) // Cursor must be below top UI bar
				{
					mouseCurrentPos = (sf::Vector2i)w.mapPixelToCoords(mouseCurrentPos);
					sf::Vector2i tempPos = originalPos;
					float r = math->DistanceBetweenPoints((sf::Vector2f)mouseDownPos, (sf::Vector2f)mouseCurrentPos);

					sf::Vector2f direction = (sf::Vector2f)mouseCurrentPos - (sf::Vector2f)mouseDownPos;
					float l = math->DistanceBetweenPoints(direction, sf::Vector2f(0, 0));
					sf::Vector2f normalized = sf::Vector2f(direction.x / l, direction.y / l);

					float dot = math->DotProduct(normalized, sf::Vector2f(1, 0));
					float det = math->Determinant(normalized, sf::Vector2f(1, 0));
					float angle = atan2(det, dot) * (180 / 3.14f);
					//cout << "Normalized: " << normalized.x << "-" << normalized.y << "|Angle: " << angle << endl;
					tempBlock->SetRotation(angle);
					//cout << "dist: " << r << endl;
					tempBlock->Resize(sf::Vector2f(r, r));
				}
			}
		}
	}
}

void LevelEditor::HandleEvents(sf::Event e, sf::RenderWindow &w)
{
	ChangeHighlightThickness(e);
	if (!ClickedBlock(e, w))
	{
		HandleDrawing(e, w);
	}
	MoveCamera(e);
	MoveShape(e);
	CreateBalls(w);
}

void LevelEditor::HandleDrawing(sf::Event e, sf::RenderWindow &w)
{
	if (!startDrawing)
	{
		if (e.type == sf::Event::MouseButtonPressed &&
			e.mouseButton.button == sf::Mouse::Left) // Click while not drawing a block
		{
			if (e.mouseButton.y > 55)
			{
				mouseDownPos.x = e.mouseButton.x;
				mouseDownPos.y = e.mouseButton.y;
				startDrawing = true;
				switch (shape)
				{
					case RECTANGLE:
					{
						switch (tile)
						{
						case GRASS:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							tempBlock = new GrassBlock((sf::Vector2f)mouseDownPos, sf::Vector2f(0, 0), 0, m_world,
								NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
							break;
						case ICE:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							tempBlock = new IceBlock((sf::Vector2f)mouseDownPos, sf::Vector2f(0, 0), 0, m_world,
								NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
							break;
						case METAL:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							tempBlock = new MetalBlock((sf::Vector2f)mouseDownPos, sf::Vector2f(0, 0), 0, m_world,
								NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
							break;
						case WATER:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							tempBlock = new WaterBlock((sf::Vector2f)mouseDownPos, sf::Vector2f(0, 0), 0, m_world,
								NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
							break;
						case SPAWN:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							levelStartPos = (sf::Vector2f)mouseDownPos;
							for (int i = 0; i < blocks.size(); i++)
							{
								if (blocks[i]->BlockType() == 4) // Spawn Block
								{
									delete blocks[i];
									blocks.erase(blocks.begin() + i);
								}
							}
							blocks.push_back(new SpawnBlock((sf::Vector2f)mouseDownPos + cameraPos));
							startDrawing = false;
							break;
						case FINISH:
							mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
							levelFinishPos = (sf::Vector2f)mouseDownPos;
							for (int i = 0; i < blocks.size(); i++)
							{
								if (blocks[i]->BlockType() == 5) // Finish block
								{
									delete blocks[i];
									blocks.erase(blocks.begin() + i);
								}
							}
							blocks.push_back(new FinishBlock((sf::Vector2f)mouseDownPos + cameraPos));
							startDrawing = false;
							break;
						}
						break;
					}
					break;
					case CIRCLE:
					{
						mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
						tempBlock = new CircleBlock((sf::Vector2f)mouseDownPos, 0, m_world,
							NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
						break;
					}
					break;
					case LINE:
					{
						mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
						tempBlock = new LineBlock((sf::Vector2f)mouseDownPos, 0, m_world,
							NULL, NULL, NULL, std::get<0>(currentBlockStats), std::get<1>(currentBlockStats), std::get<2>(currentBlockStats), true);
						break;
					}
					break;
				}
			}
		}
		if (e.type == sf::Event::MouseButtonPressed &&
			e.mouseButton.button == sf::Mouse::Right)
		{
			mouseDownPos.x = e.mouseButton.x;
			mouseDownPos.y = e.mouseButton.y;
			mouseDownPos = mouseDownPos = (sf::Vector2i)w.mapPixelToCoords(mouseDownPos);
			blocks.push_back(new CircleBlock((sf::Vector2f)mouseDownPos, 20, m_world, NULL,
				GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, 0.5, 0.5, 1, false));
			blocks.back()->SetGravity(defaultGravity);
		}

	}
	if (startDrawing) // while drawing
	{
		if (e.type == sf::Event::MouseButtonReleased &&
			e.mouseButton.button == sf::Mouse::Left) // Releasing click places the block
		{
			mouseUpPos.x = e.mouseButton.x;
			mouseUpPos.y = e.mouseButton.y;
			if (mouseUpPos.y < 55)
			{
				mouseUpPos.y = 55;
			}
			stopDrawing = true;
			startDrawing = false;
		}
	}
}

void LevelEditor::MoveCamera(sf::Event e)
{
	if (!shapeSelected)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			view1->move(sf::Vector2f(-5, 0));
			cameraMoved = true;
			//cameraPos.x--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			view1->move(sf::Vector2f(5, 0));
			cameraMoved = true;
			//cameraPos.x++;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			view1->move(sf::Vector2f(0, -5));
			cameraMoved = true;
			//cameraPos.y--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			view1->move(sf::Vector2f(0, 5));
			cameraMoved = true;
			//cameraPos.y++;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			view1->setCenter(view1->getCenter().x / 2.0f, view1->getCenter().y / 2.0f);
			cameraMoved = true;
			//cameraPos = sf::Vector2f(0, 0);
		}
		for (unsigned int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->CameraView();
		}
		//cout << view1.getCenter().x << "-" << view1.getCenter().y << endl;
	}
}

void LevelEditor::MoveShape(sf::Event e)
{
	if (shapeSelected)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			sf::Vector2f pos = blocks[selectedShapeID]->GetWorldPosition();
			blocks[selectedShapeID]->Move
			(sf::Vector2f(-1.0f, 0));

			pos = shapeHighlight.getPosition();
			shapeHighlight.setPosition
			(sf::Vector2f(pos.x - 1.0f, pos.y));

			pos = panSpr.getPosition();
			panSpr.setPosition
			(sf::Vector2f(pos.x - 1.0f, pos.y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			sf::Vector2f pos = blocks[selectedShapeID]->GetWorldPosition();
			blocks[selectedShapeID]->Move
			(sf::Vector2f(1.0f, 0));

			pos = shapeHighlight.getPosition();
			shapeHighlight.setPosition
			(sf::Vector2f(pos.x + 1.0f, pos.y));

			pos = panSpr.getPosition();
			panSpr.setPosition
			(sf::Vector2f(pos.x + 1.0f, pos.y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			sf::Vector2f pos = blocks[selectedShapeID]->GetWorldPosition();
			blocks[selectedShapeID]->Move
			(sf::Vector2f(0, -1.0f));

			pos = shapeHighlight.getPosition();
			shapeHighlight.setPosition
			(sf::Vector2f(pos.x, pos.y - 1.0f));

			pos = panSpr.getPosition();
			panSpr.setPosition
			(sf::Vector2f(pos.x, pos.y - 1.0f));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			sf::Vector2f pos = blocks[selectedShapeID]->GetWorldPosition();
			blocks[selectedShapeID]->Move
			(sf::Vector2f(0, 1.0f));

			pos = shapeHighlight.getPosition();
			shapeHighlight.setPosition
			(sf::Vector2f(pos.x, pos.y + 1.0f));

			pos = panSpr.getPosition();
			panSpr.setPosition
			(sf::Vector2f(pos.x, pos.y + 1.0f));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) // Rotate right
		{
			blocks[selectedShapeID]->Rotate(1);
			shapeHighlight.rotate(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) // Rotate left
		{
			blocks[selectedShapeID]->Rotate(-1);
			shapeHighlight.rotate(-1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) // Reset rotation
		{
			blocks[selectedShapeID]->ResetRotation();
			shapeHighlight.setRotation(0);
		}
	}
}

bool LevelEditor::ClickedBlock(sf::Event e, sf::RenderWindow &w)
{	
	if (e.type == sf::Event::MouseButtonPressed &&
		e.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Vector2i(e.mouseButton.x, e.mouseButton.y);
		mousePos = (sf::Vector2i)w.mapPixelToCoords(mousePos);

		for (int i = 0; i < blocks.size(); i++)
		{
			if (selectedShapeID != i && 
				mousePos.x > blocks[i]->GetScreenPosition().x - cameraPos.x &&
				mousePos.x < blocks[i]->GetScreenPosition().x + blocks[i]->GetSize().x - cameraPos.x &&
				mousePos.y > blocks[i]->GetScreenPosition().y - cameraPos.y &&
				mousePos.y < blocks[i]->GetScreenPosition().y + blocks[i]->GetSize().y - cameraPos.y &&
				(blocks[i]->BlockType() == 0 || blocks[i]->BlockType() == 1 || blocks[i]->BlockType() == 2 || blocks[i]->BlockType() == 3))
			{
				shapeHighlight.setOrigin(sf::Vector2f((blocks[i]->GetSize().x / 2.0f) + highlightThickness, (blocks[i]->GetSize().y / 2.0f) + highlightThickness));
				shapeHighlight.setPosition(blocks[i]->GetScreenPosition() + sf::Vector2f(-0, -0) - 
					cameraPos + sf::Vector2f(blocks[i]->GetSize().x / 2.0f, blocks[i]->GetSize().y / 2.0f));
				shapeHighlight.setSize(blocks[i]->GetSize() + sf::Vector2f(highlightThickness * 2, highlightThickness * 2));
				shapeHighlight.setRotation(blocks[i]->GetRotation());
				panSpr.setPosition(sf::Vector2f(blocks[i]->GetScreenPosition().x + (blocks[i]->GetSize().x / 2.0f) - 
					(panSpr.getTextureRect().width / 4.0f) - cameraPos.x,
					blocks[i]->GetScreenPosition().y + (blocks[i]->GetSize().y / 2.0f) - 
					(panSpr.getTextureRect().height / 4.0f) - cameraPos.y));
				shapeSelected = true;
				cout << "SELECTED" << endl;
				selectedShapeID = i;
				return true;
			}
			if (selectedShapeID != i &&
				mousePos.x > blocks[i]->GetScreenPosition().x - blocks[i]->GetSize().x - cameraPos.x &&
				mousePos.x < blocks[i]->GetScreenPosition().x + blocks[i]->GetSize().x - cameraPos.x &&
				mousePos.y > blocks[i]->GetScreenPosition().y - blocks[i]->GetSize().y - cameraPos.y &&
				mousePos.y < blocks[i]->GetScreenPosition().y + blocks[i]->GetSize().y - cameraPos.y &&
				blocks[i]->BlockType() == 6)
			{
				shapeHighlight.setOrigin(sf::Vector2f((blocks[i]->GetSize().x / 2.0f) + highlightThickness, (blocks[i]->GetSize().y / 2.0f) + highlightThickness));
				shapeHighlight.setPosition(blocks[i]->GetScreenPosition() -
					cameraPos - sf::Vector2f(blocks[i]->GetSize().x / 2.0f, blocks[i]->GetSize().y / 2.0f));
				shapeHighlight.setSize((blocks[i]->GetSize() * 2.0f) + sf::Vector2f(highlightThickness * 2, highlightThickness * 2));
				shapeHighlight.setRotation(blocks[i]->GetRotation());
				panSpr.setPosition(sf::Vector2f(blocks[i]->GetScreenPosition().x -
					(panSpr.getTextureRect().width / 4.0f) - cameraPos.x,
					blocks[i]->GetScreenPosition().y -
					(panSpr.getTextureRect().height / 4.0f) - cameraPos.y));
				shapeSelected = true;
				cout << "SELECTED" << endl;
				selectedShapeID = i;
				return true;
			}
			float dist = math->DistanceBetweenPoints((sf::Vector2f)mousePos, blocks[i]->GetScreenPosition());
			cout << "Dist: " << dist << endl;
			if (selectedShapeID != i &&
				dist < 20 &&
				blocks[i]->BlockType() == 7)
			{
				shapeHighlight.setOrigin(sf::Vector2f(highlightThickness, highlightThickness));
				shapeHighlight.setPosition(blocks[i]->GetScreenPosition() -
					cameraPos - sf::Vector2f(blocks[i]->GetSize().x / 2.0f, blocks[i]->GetSize().y / 2.0f));
				shapeHighlight.setSize(sf::Vector2f(0, 0));
				shapeHighlight.setRotation(blocks[i]->GetRotation());
				panSpr.setPosition(sf::Vector2f(blocks[i]->GetScreenPosition().x -
					(panSpr.getTextureRect().width / 4.0f) - cameraPos.x,
					blocks[i]->GetScreenPosition().y -
					(panSpr.getTextureRect().height / 4.0f) - cameraPos.y));
				shapeSelected = true;
				cout << "SELECTED" << endl;
				selectedShapeID = i;
				return true;
			}
		}
		if (e.mouseButton.y > 55)
		{
			shapeSelected = false;
			selectedShapeID = -1;
		}
	}
	return false;
}

void LevelEditor::ApplyGravity()
{
	for (b2Body* BodyIterator = m_world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			int count = 0;
			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				//cout << "worldgrav: " << m_world.GetGravity().x << "-" << m_world.GetGravity().y << endl;
				//b2Fixture->GetBody()->ApplyForceToCenter(b2Fixture->GetBody()->GetMass() * b2Vec2(0, 9.81f * 8.0f), true);
			}
		}
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->BlockType() == 0 || blocks[i]->BlockType() == 1 || blocks[i]->BlockType() == 2 ||
			blocks[i]->BlockType() == 3 || blocks[i]->BlockType() == 6 || blocks[i]->BlockType() == 7)
		{
			blocks[i]->GetBody()->SetFixedRotation(false);
			blocks[i]->GetBody()->ApplyForceToCenter(blocks[i]->GetBody()->GetMass() *
				b2Vec2(blocks[i]->GetGravity().x, blocks[i]->GetGravity().y), true);
		}
	}
}

void LevelEditor::ChangeHighlightThickness(sf::Event e)
{
	bool thicknessChanged = false;
	sf::Vector2f highlight;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) // Increase Thickness
	{
		highlightThickness++;
		highlight = sf::Vector2f(1, 1);
		thicknessChanged = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) // Decrease Thickness
	{
		if (highlightThickness > 0)
		{
			highlightThickness--;
			highlight = sf::Vector2f(-1, -1);
		}
		thicknessChanged = true;
	}
	if (thicknessChanged)
	{
		cout << highlightThickness << endl;
		shapeHighlight.setOrigin(sf::Vector2f(shapeHighlight.getOrigin().x + (highlight.x / 2.0f), shapeHighlight.getOrigin().y + (highlight.y / 2.0f)));
		shapeHighlight.setSize(shapeHighlight.getSize() + highlight);
	}
}

void LevelEditor::Draw(sf::RenderWindow &w, bool playing)
{
	w.setView(*view1);
	currentBGSpr.setPosition(0, 0);
	currentBGSpr.setPosition(math->mapPixelToCoords(currentBGSpr.getPosition()));
	w.draw(currentBGSpr);
	if (startDrawing && tempBlock)
	{
		tempBlock->AlphaDraw(w);
	}
	
	for (int i = 0; i < blocks.size(); i++)
	{
		if (shapeSelected && selectedShapeID == i)
		{
			w.draw(shapeHighlight);
		}
		blocks[i]->Draw(w, playing);
	}
	if (shapeSelected)
	{
		w.draw(panSpr);
	}
	//DrawBox2dDebug(w, cameraMoved);
}

vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> LevelEditor::getLevelRects()
{
	levelRects.clear();
	for (int i = 0; i < blocks.size(); i++)
	{
		sf::RectangleShape r;
		r.setPosition(blocks[i]->GetWorldPosition());
		r.setSize(blocks[i]->GetSize());
		int type = blocks[i]->BlockType();
		float rotation = blocks[i]->GetRotation();
		float bounce = 0;
		float friction = 0;
		float density = 0;
		if (type <= 3 || type >= 6)
		{
			bounce = blocks[i]->GetBody()->GetFixtureList()->GetRestitution();
			friction = blocks[i]->GetBody()->GetFixtureList()->GetFriction();
			density = blocks[i]->GetBody()->GetFixtureList()->GetDensity();
		}
		tuple<sf::RectangleShape, int, float, float, float, float, bool> lvl = { r, type, rotation, bounce, friction, density, blocks[i]->GetKine() };
		levelRects.push_back(lvl);
	}
	return levelRects;
}

void LevelEditor::setLevelRects(vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> rects)
{
	levelRects = rects;

	for (int i = 0; i < blocks.size(); i++)
	{
		delete blocks[i];
	}
	blocks.clear();
	for (int i = 0; i < levelRects.size(); i++)
	{
		switch (std::get<1>(levelRects[i]))
		{
		case 0:
			blocks.push_back(new GrassBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize(),
				std::get<2>(levelRects[i]), m_world, NULL, GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK, 
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			break;
		case 1:
			blocks.push_back(new IceBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize(),
				std::get<2>(levelRects[i]), m_world, NULL, GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK,
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			break;
		case 2:
			blocks.push_back(new WaterBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize(),
				std::get<2>(levelRects[i]), m_world, NULL, WATERBLOCK, GRASSBLOCK | BALL | WATERBLOCK, 
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			break;
		case 3:
			blocks.push_back(new MetalBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize(),
				std::get<2>(levelRects[i]), m_world, NULL, WATERBLOCK, GRASSBLOCK | BALL | WATERBLOCK,
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			break;
		case 4:
			blocks.push_back(new SpawnBlock
			(std::get<0>(levelRects[i]).getPosition()));
			levelStartPos = std::get<0>(levelRects[i]).getPosition();
			break;
		case 5:
			blocks.push_back(new FinishBlock
			(std::get<0>(levelRects[i]).getPosition()));
			levelFinishPos = std::get<0>(levelRects[i]).getPosition();
			break;
		case 6:
			blocks.push_back(new CircleBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize().x,
				m_world, NULL, WATERBLOCK, GRASSBLOCK | BALL | WATERBLOCK,
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			break;
		case 7:
			blocks.push_back(new LineBlock
			(std::get<0>(levelRects[i]).getPosition(),
				std::get<0>(levelRects[i]).getSize().x,
				m_world, NULL, GRASSBLOCK, GRASSBLOCK | BALL | WATERBLOCK,
				std::get<4>(levelRects[i]), std::get<3>(levelRects[i]), std::get<5>(levelRects[i]), std::get<6>(levelRects[i])));
			blocks.back()->SetGravity(defaultGravity);
			blocks.back()->SetRotation(std::get<2>(levelRects[i]));
		}
	}
}

int LevelEditor::GetCurrentBlock()
{
	if (shapeSelected)
	{
		return selectedShapeID;
	}
	return -1;
}

void LevelEditor::DeleteBlock(int b)
{
	if (shapeSelected)
	{
		shapeSelected = false;

		//m_world.DestroyBody(blocks[b]->GetBody());
		delete blocks[b];
		blocks.erase(blocks.begin() + b);

		selectedShapeID = -1;
	}
}

int LevelEditor::GetCurrentElement()
{
	if (shapeSelected)
	{
		return blocks[selectedShapeID]->BlockType();
	}
	printf("No block selected\n");
	return -1;
}
void LevelEditor::SetCurrentElement(int e)
{
	tile = (Tile)e;
}
void LevelEditor::SetCurrentBlockStats(float friction, float bounce, float density)
{
	currentBlockStats = { friction, bounce, density };
	cout << "Friction: " << friction << "|Bounce: " << bounce << "|Density: " << density << endl;
}
int LevelEditor::GetSelectedBlockType()
{
	if (shapeSelected)
	{
		return blocks[selectedShapeID]->BlockType();
	}
	return -1;
}
sf::Vector2f LevelEditor::GetSelectedBlockSize()
{
	if (shapeSelected)
	{
		return blocks[selectedShapeID]->GetSize();
	}
	return sf::Vector2f(0, 0);
}
void LevelEditor::SetIsKine(bool kine)
{
	isKine = kine;
}

void LevelEditor::HandleFloating()
{
	vector<b2Fixture> circles;
	for (b2Body* BodyIterator = m_world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			int count = 0;
			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_circle)
				{
					circles.push_back(*b2Fixture);
				}
			}
		}
	}
	for (b2Body* BodyIterator = m_world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			int count = 0;
			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_polygon)
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();
					for (int i = 0; i < circles.size(); i++)
					{
						if (circles[i].GetBody()->GetPosition().y > polygonShape->GetVertex(0).y + b2Fixture->GetBody()->GetPosition().y)
						{
							//cout << "Pushing up" << endl;
							//circles[i].GetBody()->ApplyForceToCenter(b2Vec2(0, -50000), true);
						}
					}
				}
			}
		}
	}
}

string LevelEditor::GetNameOfLevelToSave()
{
	return nameOfLevelToSave;
}
string LevelEditor::GetNameOfLevelToLoad()
{
	return nameOfLevelToLoad;
}
void LevelEditor::SetNameOfLevelToSave(string fileName)
{
	nameOfLevelToSave = fileName;
}
void LevelEditor::SetNameOfLevelToLoad(string fileName)
{
	nameOfLevelToLoad = fileName;
}

void LevelEditor::CreateBalls(sf::RenderWindow &w)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spaceKeyPressed)
	{
		spaceKeyPressed = true;
		b2BodyDef circleBodyDef;
		circleBodyDef.type = b2_dynamicBody;
		sf::Vector2i pos = sf::Mouse::getPosition(w);
		pos = (sf::Vector2i)w.mapPixelToCoords(pos);
		circleBodyDef.position.Set(pos.x, pos.y);
		b2Body* circleBody = m_world.CreateBody(&circleBodyDef);

		b2CircleShape circleShape;
		circleShape.m_radius = 20;
		circleShape.m_p.Set(10, 10);

		b2FixtureDef circleFixtureDef;
		circleFixtureDef.shape = &circleShape;
		circleFixtureDef.density = 1.0f;
		circleFixtureDef.friction = 0.0f;
		circleFixtureDef.filter.categoryBits = BALL;
		circleFixtureDef.filter.maskBits = GRASSBLOCK | BALL;

		circleBody->SetFixedRotation(true);
		circleBody->CreateFixture(&circleFixtureDef);
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		spaceKeyPressed = false;
	}
}

bool LevelEditor::ShapeSelected()
{
	return shapeSelected;
}
sf::Vector2f LevelEditor::PositionOfSelectedShape()
{
	if (blocks[selectedShapeID])
	{
		return blocks[selectedShapeID]->GetScreenPosition();
	}
	return sf::Vector2f(-100, -100);
}
void LevelEditor::SetPosition(int block, sf::Vector2f pos)
{
	blocks[block]->SetPosition(pos);
	blocks[block]->Move(sf::Vector2f(0, 0));
}
float LevelEditor::GetBounciness(int block)
{
	return blocks[block]->GetBody()->GetFixtureList()->GetRestitution();
}
void LevelEditor::SetBounciness(int block, float bounciness)
{
	blocks[block]->GetBody()->GetFixtureList()->SetRestitution(bounciness);
}
float LevelEditor::GetFriction(int block)
{
	return blocks[block]->GetBody()->GetFixtureList()->GetFriction();
}
void LevelEditor::SetFriction(int block, float friction)
{
	blocks[block]->GetBody()->GetFixtureList()->SetFriction(friction);
}
float LevelEditor::GetDensity(int block)
{
	return blocks[block]->GetBody()->GetFixtureList()->GetDensity();
}
void LevelEditor::SetDensity(int block, float density)
{
	blocks[block]->GetBody()->GetFixtureList()->SetDensity(density);
}
float LevelEditor::GetRotation(int block)
{
	if (blocks[selectedShapeID])
	{
		return blocks[selectedShapeID]->GetRotation();
	}
	return 0;
}
void LevelEditor::SetRotation(int block, float rotation)
{
	blocks[block]->SetRotation(rotation);
}
int LevelEditor::GetCurrentShape()
{
	return shape;
}
void LevelEditor::SetCurrentShape(int s)
{
	shape = (Shape)s;
}

void LevelEditor::SetPlayerSpawn(sf::Vector2f pos)
{
	levelStartPos = pos;
}
sf::Vector2f LevelEditor::GetPlayerSpawn()
{
	return levelStartPos;
}
void LevelEditor::SetLevelFinish(sf::Vector2f pos)
{
	levelFinishPos = pos;
}
sf::Vector2f LevelEditor::GetLevelFinish()
{
	return levelFinishPos;
}
b2World* LevelEditor::GetWorld()
{
	return &m_world;
}
vector<b2Body*> LevelEditor::GetBlockBodies()
{
	vector<b2Body*> bodies;

	for (int i = 0; i < blocks.size(); i++)
	{
		bodies.push_back(blocks[i]->GetBody());
	}

	return bodies;
}
b2Body* LevelEditor::GetBlockBody(int i)
{
	return blocks[i]->GetBody();
}
void LevelEditor::WindowResized()
{
	sf::IntRect rect = currentBGSpr.getTextureRect();
	float xsize = view1->getSize().x / rect.width;
	float ysize = view1->getSize().y / rect.height;
	float scale;
	if (xsize >= ysize)
	{
		currentBGSpr.setScale(xsize, xsize);
	}
	else
	{
		currentBGSpr.setScale(ysize, ysize);
	}
}
void LevelEditor::LoadContent()
{
	panTex.loadFromFile("Textures/UI/iconPan.png");
	panSpr.setTexture(panTex);
	panSpr.setScale(0.5f, 0.5f);

	grassBGTex.loadFromFile("Textures/UI/grassBG.png");
	grassBGSpr.setTexture(grassBGTex);
	grassBGSpr.setPosition(sf::Vector2f(0, 0));
	grassBGTex.setRepeated(true);
	float xsize = 800.0f / grassBGTex.getSize().x;
	float ysize = 600.0f / grassBGTex.getSize().y;
	if (xsize >= ysize)
	{
		grassBGSpr.setScale(xsize, xsize);
	}
	else
	{
		grassBGSpr.setScale(ysize, ysize);
	}
	currentBGSpr = grassBGSpr;

	caveBGTex.loadFromFile("Textures/UI/caveBG.png");
	caveBGSpr.setTexture(caveBGTex);
	caveBGSpr.setPosition(sf::Vector2f(0, 0));

	cityBGTex.loadFromFile("Textures/UI/cityBG.png");
	cityBGSpr.setTexture(cityBGTex);
	cityBGSpr.setPosition(sf::Vector2f(0, 0));

	desertBGTex.loadFromFile("Textures/UI/desertBG.png");
	desertBGSpr.setTexture(desertBGTex);
	desertBGSpr.setPosition(sf::Vector2f(0, 0));

	mountainBGTex.loadFromFile("Textures/UI/mountainBG.png");
	mountainBGSpr.setTexture(mountainBGTex);
	mountainBGSpr.setPosition(sf::Vector2f(0, 0));

	skyBGTex.loadFromFile("Textures/UI/skyBG.png");
	skyBGSpr.setTexture(skyBGTex);
	skyBGSpr.setPosition(sf::Vector2f(0, 0));

	spaceBGTex.loadFromFile("Textures/UI/spaceBG.png");
	spaceBGSpr.setTexture(spaceBGTex);
	spaceBGSpr.setPosition(sf::Vector2f(0, 0));

	BGSprVec.push_back(grassBGSpr);
	BGSprVec.push_back(caveBGSpr);
	BGSprVec.push_back(cityBGSpr);
	BGSprVec.push_back(desertBGSpr);
	BGSprVec.push_back(mountainBGSpr);
	BGSprVec.push_back(skyBGSpr);
	BGSprVec.push_back(spaceBGSpr);
}
int LevelEditor::GetShape()
{
	return shape;
}
void LevelEditor::SetShape(int s)
{
	shape = (Shape)s;
}

void LevelEditor::SetBackGround(int i)
{
	currentBGSpr = BGSprVec[i];
	currentBackground = i;

	sf::IntRect rect = currentBGSpr.getTextureRect();
	float xsize = view1->getSize().x / rect.width;
	float ysize = view1->getSize().y / rect.height;
	float scale;
	if (xsize >= ysize)
	{
		currentBGSpr.setScale(xsize, xsize);
	}
	else
	{
		currentBGSpr.setScale(ysize, ysize);
	}
}
int LevelEditor::GetBackGround()
{
	return currentBackground;
}

void LevelEditor::ResetLevel()
{
	printf("RESETING LEVEL\n");
	setLevelRects(defaultLevelRects);
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->BlockType() == 0 || blocks[i]->BlockType() == 1 || blocks[i]->BlockType() == 2 ||
			blocks[i]->BlockType() == 3 || blocks[i]->BlockType() == 6)
		{
			blocks[i]->GetBody()->SetLinearVelocity(b2Vec2_zero);
		}
	}
}
void LevelEditor::SaveDefaultLevel()
{
	printf("SAVING DEFAULT\n");
	defaultLevelRects = getLevelRects();
}

void LevelEditor::DrawBox2dDebug(sf::RenderWindow &w, bool c)
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
				if (shapeType == b2Shape::e_circle)
				{
					sf::CircleShape circle;
					circle.setOrigin(sf::Vector2f(circleShape->m_radius, circleShape->m_radius));
					circle.setOutlineThickness(1.0f);
					circle.setOutlineColor(sf::Color::Red);
					circle.setFillColor(sf::Color::Transparent);
					circle.setRadius(circleShape->m_radius);
					circle.setPosition(b2Fixture->GetBody()->GetPosition().x + circleShape->m_p.x,
						b2Fixture->GetBody()->GetPosition().y + circleShape->m_p.y);

					w.draw(circle);
				}
				else if (shapeType == b2Shape::e_polygon)
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();

					int length = (int)polygonShape->GetVertexCount();

					sf::VertexArray line(sf::LinesStrip, length + 1);

					b2Vec2 center = b2Vec2_zero;
					center.x = b2Fixture->GetBody()->GetPosition().x;
					center.y = b2Fixture->GetBody()->GetPosition().y;

					for (int i = 0; i < length; i++)
					{
						line[i].position.x = cos(b2Fixture->GetBody()->GetAngle())
							* (((polygonShape->GetVertex(i).x + b2Fixture->GetBody()->GetPosition().x) * zoomLevel) - center.x)
							- sin(b2Fixture->GetBody()->GetAngle())
							* (((polygonShape->GetVertex(i).y + b2Fixture->GetBody()->GetPosition().y) * zoomLevel) - center.y) + center.x;

						line[i].position.y = sin(b2Fixture->GetBody()->GetAngle())
							* (((polygonShape->GetVertex(i).x + b2Fixture->GetBody()->GetPosition().x) * zoomLevel) - center.x)
							+ cos(b2Fixture->GetBody()->GetAngle())
							* (((polygonShape->GetVertex(i).y + b2Fixture->GetBody()->GetPosition().y) * zoomLevel) - center.y) + center.y;
						line[i].color = sf::Color::Red;
					}

					line[length].position.y = line[0].position.y;
					line[length].position.x = line[0].position.x;
					line[length].color = sf::Color::Red;

					w.draw(line);
				}
			}
		}
	}
}