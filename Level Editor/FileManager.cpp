#include "FileManager.h"

FileManager::FileManager()
{

}

void FileManager::SaveMap(vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> l, string fileName)
{
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> lvl = l;
	XMLNode * pRoot = writeToDoc.NewElement("Root");
	writeToDoc.InsertFirstChild(pRoot);

	XMLElement * pElement = writeToDoc.NewElement("Rec");
	for (int i = 0; i < lvl.size(); i++)
	{
		pElement = writeToDoc.NewElement("Rects");
		pElement->SetAttribute("x", std::get<0>(lvl[i]).getPosition().x);
		pElement->SetAttribute("y", std::get<0>(lvl[i]).getPosition().y);
		pElement->SetAttribute("w", std::get<0>(lvl[i]).getSize().x);
		pElement->SetAttribute("h", std::get<0>(lvl[i]).getSize().y);
		pElement->SetAttribute("type", std::get<1>(lvl[i]));
		pElement->SetAttribute("rotate", std::get<2>(lvl[i]));
		pElement->SetAttribute("bounce", std::get<3>(lvl[i]));
		pElement->SetAttribute("friction", std::get<4>(lvl[i]));
		pElement->SetAttribute("density", std::get<5>(lvl[i]));
		int kine;
		if (std::get<6>(lvl[i]))
		{
			kine = 1;
		}
		if (!std::get<6>(lvl[i]))
		{
			kine = 0;
		}
		pElement->SetAttribute("kine", kine);
		pRoot->InsertEndChild(pElement);
	}
	pRoot->InsertEndChild(pElement);
	
	//const char name[] = "testXML.xml";
	fileName += ".xml";
	const char* fileChar = fileName.c_str();
	XMLError eResult = writeToDoc.SaveFile(fileChar);

	cout << "File Saved" << endl;
}

vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> FileManager::LoadMap(string fileName)
{
	const char* fileChar = fileName.c_str();
	XMLError eResult = readFromDoc.LoadFile(fileChar);

	XMLNode * pRoot = readFromDoc.FirstChild();
	if (pRoot == nullptr)
	{
		//return XML_ERROR_FILE_READ_ERROR;
	}

	XMLElement * pRectElements = pRoot->FirstChildElement("Rects");
	vector<tuple<sf::RectangleShape, int, float, float, float, float, bool>> loadedLvl;
	while (pRectElements != nullptr)
	{
		const char * xVal = nullptr;
		const char * yVal = nullptr;
		const char * wVal = nullptr;
		const char * hVal = nullptr;
		const char * typeVal = nullptr;
		const char * rotateVal = nullptr;
		const char * bounceVal = nullptr;
		const char * frictionVal = nullptr;
		const char * densityVal = nullptr;
		const char * kineVal = nullptr;

		xVal = pRectElements->Attribute("x");
		string strOutX = xVal;
		float x = stof(strOutX);

		yVal = pRectElements->Attribute("y");
		string strOutY = yVal;
		float y = stof(strOutY);

		wVal = pRectElements->Attribute("w");
		string strOutW = wVal;
		float w = stof(strOutW);

		hVal = pRectElements->Attribute("h");
		string strOutH = hVal;
		float h = stof(strOutH);

		typeVal = pRectElements->Attribute("type");
		string strOutT = typeVal;
		int t = stoi(strOutT);

		rotateVal = pRectElements->Attribute("rotate");
		string strOutR = rotateVal;
		float r = stof(strOutR);

		bounceVal = pRectElements->Attribute("bounce");
		string strOutB = bounceVal;
		float b = stof(strOutB);

		frictionVal = pRectElements->Attribute("friction");
		string strOutF = frictionVal;
		float f = stof(strOutF);

		densityVal = pRectElements->Attribute("density");
		string strOutD = densityVal;
		float d = stof(strOutD);

		kineVal = pRectElements->Attribute("kine");
		string strOutK = kineVal;
		bool k;
		if (strOutK == "0")
		{
			k = false;
		}
		if (strOutK == "1")
		{
			k = true;
		}

		sf::RectangleShape rec;
		rec.setPosition(x, y);
		rec.setSize(sf::Vector2f(w, h));
		rec.setOutlineThickness(2);
		rec.setFillColor(sf::Color::Blue);
		tuple<sf::RectangleShape, int, float, float, float, float, bool> block = { rec, t, r, b, f, d, k };
		loadedLvl.push_back(block);

		pRectElements = pRectElements->NextSiblingElement("Rects");
	}

	return loadedLvl;
}