#ifdef  _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "SFML\Graphics.hpp"
#include "LevelEditor.h"
#include "FileManager.h"
#include "UI.h"
#include "Player.h"
#include "MathMethods.h"
#include "ScaleWorld.h"

using namespace std;
using namespace tinyxml2;

XMLDocument writeToDoc;
XMLDocument readFromDoc;

bool levelShrunk = false;

sf::View view1(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
sf::RenderWindow window(sf::VideoMode(800, 600), "Level Editor");


MathMethods* math = new MathMethods(&window);
LevelEditor* editor = new LevelEditor(&view1, math);
FileManager* fileMgr = new FileManager();
Player* player = new Player(sf::Vector2f(0, 0), &view1, editor, math);
UI* ui = new UI(editor, fileMgr, player, math, &view1);
ScaleWorld* scaleWorld = new ScaleWorld(*editor->GetWorld(), math);


int main(int argc, char** argv) 
{
	//sf::RenderWindow window(sf::VideoMode(800, 600), "Level Editor");
	window.setView(view1);
	window.setFramerateLimit(60);
	//player->ScaleDownModel();

	while (window.isOpen())
	{
		if (player->GetPlaying() && !levelShrunk)
		{
			printf("SHRINK\n");
			scaleWorld->ScaleDownWorld();
			player->ScaleDownModel();
			levelShrunk = true;
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			{
				if (ui->CanZoom(math->ZOOM + 0.1f))
				{
					math->ZOOM += 0.1f;
					//view1.setSize(window.getSize().x, window.getSize().y);
					ui->WindowResized();
					editor->WindowResized();
					printf("Set zoom to x1\n");
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && math->ZOOM > 0.8f)
			{
				if (ui->CanZoom(math->ZOOM - 0.1f))
				{
					math->ZOOM -= 0.1f;
					//view1.setSize(window.getSize().x * 2, window.getSize().y * 2);
					ui->WindowResized();
					editor->WindowResized();
					printf("Set zoom to x1.25\n");
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (ui->CanZoom(1))
				{
					math->ZOOM = 1;
					//view1.setSize(window.getSize().x * 3, window.getSize().y * 3);
					ui->WindowResized();
					editor->WindowResized();
					printf("Set zoom to x1\n");
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::Vector2f size = (sf::Vector2f)window.getSize();

				if (size.x < 800)
				{
					size.x = 800;
				}
				if (size.y < 600)
				{
					size.y = 600;
				}

				window.setSize((sf::Vector2u)size);
				sf::Vector2f dif = view1.getSize() - size;
				view1.move(-dif / 2.0f);
				sf::Vector2f oldSize = view1.getSize();
				view1.setSize(size);
				window.setView(view1);
				if (size.x < oldSize.x || size.y < oldSize.y)
				{
					if (!ui->CanZoom(math->ZOOM))
					{
						float maxZoom = 1.0f;
						while (ui->CanZoom(maxZoom))
						{
							maxZoom += 0.1f;
						}
						maxZoom -= 0.1f;
						math->ZOOM = maxZoom;
						//view1.setSize(window.getSize().x * 3, window.getSize().y * 3);
						ui->WindowResized();
						editor->WindowResized();
						cout << "Set zoom to x" << maxZoom << endl;
					}
				}
				ui->WindowResized();
				editor->WindowResized();
			}
			
			bool clicked = ui->HandleEvents(event, window);
			editor->SetNameOfLevelToSave(ui->GetNameToSaveLevelAs());
			if (ui->mode == ui->BUILDING && !clicked)
			{
				editor->HandleEvents(event, window);
			}
		}
		if (!player->GetPlaying() && levelShrunk)
		{
			printf("GROW\n");
			scaleWorld->ScaleUpWorld();
			player->ScaleUpModel();
			editor->ResetLevel();
			levelShrunk = false;
		}
		editor->Update(window);
		player->Update(window, 16.6667);
		if (player->GetPlaying())
		{
			scaleWorld->StepWorld(1.0f / 60.0f);
		}
		
		window.clear(sf::Color::White);

		editor->Draw(window, player->GetPlaying());
		ui->Draw(window);
		player->Draw(window);

		window.display();
		
		
	}
	//return XML_SUCCESS;
	return 0;
}

