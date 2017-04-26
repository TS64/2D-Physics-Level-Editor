#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "SFML\Graphics.hpp"

using namespace std;
using namespace tinyxml2;

class FileManager
{
public:
	FileManager();
	void SaveMap(vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> l, string fileName);
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> LoadMap(string fileName);
private:
	XMLDocument writeToDoc;
	XMLDocument readFromDoc;
};
