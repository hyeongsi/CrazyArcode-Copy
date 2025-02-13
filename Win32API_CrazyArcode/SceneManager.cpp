﻿#include "SceneManager.h"
#include "Character.h"
#include "InGameScene.h"
#include "LobbyScene.h"
#include "ImageManager.h"

SceneManager::SceneManager(HWND hWnd)
{
	this->hWnd = hWnd;
	this->hdc = GetDC(hWnd);
	memDC = CreateCompatibleDC(hdc);
	memDCBack = CreateCompatibleDC(hdc);
	lobbyScene = new LobbyScene();
	inGameScene = new InGameScene();
}

SceneManager::~SceneManager()
{
	DeleteDC(memDC);
	DeleteDC(memDCBack);
	ReleaseDC(hWnd, hdc);

	delete lobbyScene;
	delete inGameScene;
}

void SceneManager::Process()
{
	if (oldHBitMap != NULL)
	{
		DeleteObject(oldHBitMap);
		oldHBitMap = NULL;
	}

	oldHBitMap = (HBITMAP)SelectObject(memDCBack, CreateCompatibleBitmap(hdc, WND_WIDTH, WND_HEIGHT));

	switch (sceneState)
	{
	case GameStage::LOBBY:
		lobbyScene->Process(memDCBack, memDC);
		selectData = lobbyScene->GetSelectData();
		MessageQueue::RunEventQueue(lobbyScene->GetLobbyObjList(), sceneState);
		break;
	case GameStage::INGAME:
		inGameScene->Process(memDCBack, memDC);
		MessageQueue::RunEventQueue(inGameScene->GetInGameObjList(), sceneState);
		break;
	}

	BitBlt(hdc, 0, 0, WND_WIDTH, WND_HEIGHT, memDCBack, 0, 0, SRCCOPY);		
}

void SceneManager::InitInGameData()
{
	inGameScene->Init();
}

void SceneManager::InitInGamePlayTime()
{
	inGameScene->InitInGamePlayTime();
}

const int SceneManager::GetSceneState()
{
	return sceneState;
}

void SceneManager::SetSceneState(int sceneState)
{
	this->sceneState = sceneState;
}

void SceneManager::LoadLobbyData(const vector<pImageData>& lobbyData)
{
	lobbyScene->LoadData(lobbyData);
}

const SelectData& SceneManager::GetSelectData()
{
	return selectData;
}

void SceneManager::LoadInGameImage(const vector<pImageData>& inGameBackGround)
{
	inGameScene->LoadInGameImage(inGameBackGround);
}

void SceneManager::LoadCharacterData(const pImageData* characterImage, const CharacterStatsData* characterStats)
{
	for (int i = 0; i < 2; i++)
		inGameScene->LoadCharacterData(characterImage[i], characterImage[2], characterImage[3], characterStats[i]);
}

void SceneManager::LoadMapData(const MapData& mapData)
{
	inGameScene->GetMapData(mapData);
	inGameScene->LoadStaticObjectData();
}

void SceneManager::LoadItemImageData(const pImageData itemData)
{
	inGameScene->LoadItemImage(itemData);
}