﻿#pragma once
#pragma comment(lib, "Msimg32.lib")
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include "DataStruct.h"

using namespace std;

class ImageManager
{
private:
	ifstream fin;
	vector<pImageData2> lobbyBitmap;
	vector<pImageData> InGameImageInfo;

public:
	ImageManager();
	~ImageManager();

	void LoadImageData();
	void LoadLobbyData();
	void LoadInGameImageData();
	const vector<pImageData2>& GetLobbyImageData() const;
	const vector<pImageData>& GetInGameImageData();
};

