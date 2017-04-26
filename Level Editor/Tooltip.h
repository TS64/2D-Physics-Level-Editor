#pragma once

#include "SFML\Graphics.hpp"

#include <iostream>
#include <vector>

using namespace std;

class Tooltip
{
public:
	Tooltip(sf::Vector2f pos, std::string message, float zoom)
	{
		arialFont.loadFromFile("Textures/Fonts/arial.ttf");
		string.setString(message);
		string.setCharacterSize(20 * zoom);
		string.setFont(arialFont);
		string.setPosition(pos);
		string.setFillColor(sf::Color::Black);

		int currentNoOfCharacter = 0;
		for (size_t i = 0; i < message.length(); i++)
		{
			currentNoOfCharacter++;
			if (message[i] == '\n')
			{
				numberOfLines++;
				if (currentNoOfCharacter > longestLine)
				{
					longestLine = currentNoOfCharacter;
					//cout << "Longest line: " << longestLine << endl;
				}
				currentNoOfCharacter = 0;
			}
		}
		if (currentNoOfCharacter > longestLine)
		{
			longestLine = currentNoOfCharacter;
		}
		//cout << "Longest line: " << longestLine << endl;
		//cout << "Number of lines: " << numberOfLines << endl;

		rect.setPosition(pos);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineThickness(2.0f * zoom);
		rect.setOutlineColor(sf::Color::Black);
		rect.setSize(sf::Vector2f(3 + (9 * longestLine * zoom) + 6, 3 + (22 * numberOfLines * zoom) + 3));
	};

	void Draw(sf::RenderWindow &w)
	{
		w.draw(rect);
		w.draw(string);
	}
private:
	sf::Font arialFont;
	sf::Text string;
	sf::RectangleShape rect;

	int numberOfLines = 1;
	int longestLine = 0;
};
