﻿#include "Character.h"
#include "MessageQueue.h"

Character::Character(const int name, const ObjectData::POSITION pos, const ObjectData::SIZE size, int hNumber, int vNumber, HBITMAP hBitmap, CharacterStatsData characterStats)
	: DynamicObject(name, pos, size, hNumber, vNumber, hBitmap)
{
	this->stats = characterStats;

	Init();
}

Character::~Character()
{

}

void Character::Init()
{
	switch (name)
	{
	case EnumObj::RedBazzi:
		color = CharacterColor::RED;
		characterName = CharacterName::BAZZI;
		break;
	case EnumObj::RedDizni:
		color = CharacterColor::RED;
		characterName = CharacterName::DIZNI;
		break;
	case EnumObj::BlueBazzi:
		color = CharacterColor::BLUE;
		characterName = CharacterName::BAZZI;
		break;
	case EnumObj::BlueDizni:
		color = CharacterColor::BLUE;
		characterName = CharacterName::DIZNI;
		break;
	}

	characterValue.state = State::NORMAL;		//현재상태
	characterValue.isAttackPossible = true;	//공격가능여부
	characterValue.isMoveable = true;			//이동가능여부
	characterValue.waterBallonNumber = 0;		//현재 놓은 물풍선 개수
}

void Character::Input()
{
	Manual();	//조작 입력
}

void Character::Update()
{
	switch (dir)
	{
	case Direction::LEFT:
		pos.x -= stats.speed;

		if (!CheckmDelay(charAnimationTick, 100) && (prevDir == dir))
			break;

		printPos.x += imageWidth;
		printPos.y = 0;
		if (printPos.x >= size.width)
			printPos.x = 0;

		prevDir = Direction::LEFT;
		break;
	case Direction::TOP:
		pos.y -= stats.speed;

		if (!CheckmDelay(charAnimationTick, 100) && (prevDir == dir))
			break;

		printPos.x += imageWidth;
		printPos.y = imageHeight;
		if (printPos.x >= size.width)
			printPos.x = 0;

		prevDir = Direction::TOP;
		break;
	case Direction::RIGHT:
		pos.x += stats.speed;

		if (!CheckmDelay(charAnimationTick, 100) && (prevDir == dir))
			break;

		printPos.x += imageWidth;
		printPos.y = imageHeight * 2;
		if (printPos.x >= size.width)
			printPos.x = 0;

		prevDir = Direction::RIGHT;
		break;
	case Direction::BOTTOM:
		pos.y += stats.speed;

		if (!CheckmDelay(charAnimationTick, 100) && (prevDir == dir))
			break;

		printPos.x += imageWidth;
		printPos.y = imageHeight * 3;
		if (printPos.x >= size.width)
			printPos.x = 0;

		prevDir = Direction::BOTTOM;
		break;
	}
	order = pos.y;

	POINT boomPosition{ -1, -1 };	//터지는물풍선의 위치
	POINT boomPoint{ -1, -1 };		//터지는 물풍선 캐릭터와 조건검사할 좌표

	//물풍선 폭팔범위에 캐릭터가 존재하는지 체크
	if (attackArea.pos.x != -1)
	{
		boomPosition.x = attackArea.pos.x * BLOCK_X + 20;
		boomPosition.y = attackArea.pos.y * BLOCK_Y + 25;

		//위
		for (int i = 0; i <= attackArea.t; i++)
		{
			boomPoint.y = boomPosition.y - (i * BLOCK_Y) + (BLOCK_Y / 2);
			boomPoint.x = boomPosition.x + (BLOCK_X / 2);
			if (pos.y <= boomPoint.y && pos.y + BLOCK_Y >= boomPoint.y
				&& pos.x <= boomPoint.x && pos.x + BLOCK_X >= boomPoint.x)
			{
				characterValue.state = State::TRAPPED;
				stats.speed = 1;
			}
		}
		//우측
		for (int i = 0; i <= attackArea.r; i++)
		{
			boomPoint.y = boomPosition.y + (BLOCK_Y / 2);
			boomPoint.x = boomPosition.x + (i * BLOCK_X) + (BLOCK_X / 2);
			if (pos.y <= boomPoint.y && pos.y + BLOCK_Y >= boomPoint.y
				&& pos.x <= boomPoint.x && pos.x + BLOCK_X >= boomPoint.x)
			{
				characterValue.state = State::TRAPPED;
				stats.speed = 1;
			}
		}
		//아래
		for (int i = 0; i <= attackArea.b; i++)
		{
			boomPoint.y = boomPosition.y + (i * BLOCK_Y) + (BLOCK_Y / 2);
			boomPoint.x = boomPosition.x + (BLOCK_X / 2);
			if (pos.y <= boomPoint.y && pos.y + BLOCK_Y >= boomPoint.y
				&& pos.x <= boomPoint.x && pos.x + BLOCK_X >= boomPoint.x)
			{
				characterValue.state = State::TRAPPED;
				stats.speed = 1;
			}
		}
		//좌측
		for (int i = 0; i <= attackArea.l; i++)
		{
			boomPoint.y = boomPosition.y + (BLOCK_Y / 2);
			boomPoint.x = boomPosition.x - (i * BLOCK_X) + (BLOCK_X / 2);
			if (pos.y <= boomPoint.y && pos.y + BLOCK_Y >= boomPoint.y
				&& pos.x <= boomPoint.x && pos.x + BLOCK_X >= boomPoint.x)
			{
				characterValue.state = State::TRAPPED;
				stats.speed = 1;
			}
		}

		attackArea = { {-1, -1}, -1, -1, -1, -1 };
	}
	



	//갇힌상태라면 실행
	if (characterValue.state == State::TRAPPED)
	{
		//if(바늘사용하면)
		//{
		//	redValue.state = State::NORMAL;
		//	redValue.trappedPrinthNumber = 0;
		//}
		if (CheckmDelay(characterValue.time, 150))
		{
			trappedImage.printHorizontalNumber++;
			if (trappedImage.printHorizontalNumber > trappedImage.hNumber)
			{
				characterValue.state = State::DIE;
				characterValue.isAttackPossible = false;
			}
		}
	}

	//죽을상태라면
	if (characterValue.state == State::DIE)
	{
		characterValue.isMoveable = false;
		if (CheckmDelay(characterValue.time, 150))
		{
			dieImage.printHorizontalNumber++;
			if (dieImage.printHorizontalNumber > dieImage.hNumber)
			{
				//게임종료UI띄우기
				//SceneManager의 sceneState값 변경해야함
			}
		}
	}
}

//매개변수 이름 바꿔줘야할듯
void Character::LateUpdate(const list<Obj*>& inGameObjectVector)
{
	//이동제한
	MapImmovableArea();
	StaticObjectmmovableArea(inGameObjectVector);
	WaterBallonImmovableArea();
	dir = -1;
}

void Character::Render(HDC hDC, HDC memDc)
{
	switch (characterValue.state)
	{
	case State::NORMAL:
		SelectObject(memDc, hBitmap);
		TransparentBlt(hDC,
			pos.x, pos.y,				//출력될 이미지 시작좌표
			imageWidth, imageHeight,	//출력될 이미지크기
			memDc,
			printPos.x, printPos.y,		//이미지에서 출력할 시작위치
			imageWidth, imageHeight,	//이미지에서 출력할 이미지의 크기
			RGB(255, 0, 255));
		break;

	case State::TRAPPED:
		SelectObject(memDc, trappedImage.hBitmap);
		TransparentBlt(hDC,
			pos.x, pos.y,				//출력될 이미지 시작좌표
			trappedImage.imageWidth, trappedImage.imageHeight,	//출력될 이미지크기
			memDc,
			trappedImage.imageWidth * trappedImage.printHorizontalNumber, trappedImage.imageHeight * trappedImage.printVerticalNumber,		//이미지에서 출력할 시작위치
			trappedImage.imageWidth, trappedImage.imageHeight,	//이미지에서 출력할 이미지의 크기
			RGB(255, 0, 255));
		break;

	case State::DIE:
		SelectObject(memDc, dieImage.hBitmap);
		TransparentBlt(hDC,
			pos.x, pos.y,				//출력될 이미지 시작좌표
			dieImage.imageWidth, dieImage.imageHeight,	//출력될 이미지크기
			memDc,
			dieImage.imageWidth * dieImage.printHorizontalNumber, dieImage.imageHeight * dieImage.printVerticalNumber,		//이미지에서 출력할 시작위치
			dieImage.imageWidth, dieImage.imageHeight,	//이미지에서 출력할 이미지의 크기
			RGB(255, 0, 255));
		break;
	}

	//테스트용 현재 플레이어 위치좌표출력
	static char c[255];
	if (CharacterColor::RED == color)
	{
		sprintf_s(c, "RED : %d, %d", pos.x, pos.y);
		TextOut(hDC, 10, 10, c, sizeof(c));
	}
	if (CharacterColor::BLUE == color)
	{
		sprintf_s(c, "BLUE : %d, %d", pos.x, pos.y);
		TextOut(hDC, 10, 50, c, sizeof(c));
	}
}

void Character::GetDefaultImage(const pImageData trappedImage, const pImageData dieImage)
{
	//trapped이미지 저장
	BITMAP bmp;
	GetObject(trappedImage->hBitmap, sizeof(BITMAP), &bmp);
	this->trappedImage.name = trappedImage->name;							//이름
	this->trappedImage.hBitmap = trappedImage->hBitmap;						//비트맵
	this->trappedImage.hNumber = trappedImage->hNumber;						//가로이미지수
	this->trappedImage.vNumber = trappedImage->vNumber;						//세로이미지
	this->trappedImage.imageWidth = bmp.bmWidth / trappedImage->hNumber;	//이미지크기/가로이미지수
	this->trappedImage.imageHeight = bmp.bmHeight / trappedImage->vNumber;	//이미지크기/세로이미지수

	//die이미지 저장
	GetObject(dieImage->hBitmap, sizeof(BITMAP), &bmp);
	this->dieImage.name = dieImage->name;
	this->dieImage.hBitmap = dieImage->hBitmap;
	this->dieImage.hNumber = dieImage->hNumber;
	this->dieImage.vNumber = dieImage->vNumber;
	this->dieImage.imageWidth = bmp.bmWidth / dieImage->hNumber;
	this->dieImage.imageHeight = bmp.bmHeight / dieImage->vNumber;

	//캐릭터마다 출력될 위치값 지정
	this->trappedImage.printHorizontalNumber = 0;			//trapped 가로 출력될 이미지 번호
	this->dieImage.printHorizontalNumber = 0;				//die 가로 출력될 이미지 번호
	switch (characterName)
	{
	case CharacterName::BAZZI:
		this->trappedImage.printVerticalNumber = 0;		//trapped 세로 출력될 이미지 번호
		this->dieImage.printVerticalNumber = 0;			//die 세로 출력될 이미지 번호
		break;
	case CharacterName::DIZNI:
		this->trappedImage.printVerticalNumber = 1;
		this->dieImage.printVerticalNumber = 2;
		break;
	}
}

void Character::SettingAttackPos()
{
	//+20해주는 이유는 맵의 시작위치가 (20,47)이라 +20을 해줘야 값이맞음
	int interval = 0;
	if ((attack.pos.x + 20) % BLOCK_X != 0)
	{
		interval = (attack.pos.x + 20) % BLOCK_X;
		if (interval >= BLOCK_X / 2)
			attack.pos.x = attack.pos.x + (BLOCK_X - interval);
		else
			attack.pos.x = attack.pos.x - interval;
	}
	if (attack.pos.y % BLOCK_Y != 0)
	{
		interval = attack.pos.y % BLOCK_Y;
		if (interval >= BLOCK_Y / 2)
			attack.pos.y = attack.pos.y + (BLOCK_Y - interval) - 2;
		else
			attack.pos.y = attack.pos.y - interval - 2;		//이거 물풍선과 블럭사이의 y좌표에 2만큼오차가 생겨서 빼주는건데 원인찾아서 고치기
	}
}

void Character::Manual()
{
	switch (color)
	{
	case CharacterColor::RED:
		if (characterValue.isMoveable)
		{
			characterValue.prevPos = pos;
			if (GetAsyncKeyState(VK_UP))
			{
				dir = Direction::TOP;
			}
			if (GetAsyncKeyState(VK_DOWN))
			{
				dir = Direction::BOTTOM;
			}
			if (GetAsyncKeyState(VK_LEFT))
			{
				dir = Direction::LEFT;
			}
			if (GetAsyncKeyState(VK_RIGHT))
			{
				dir = Direction::RIGHT;
			}
			if (characterValue.isAttackPossible && (this->stats.bNum > characterValue.waterBallonNumber))
			{
				if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)		//공격
				{
					attack.isColor = CharacterColor::RED;
					attack.pos.x = pos.x;
					attack.pos.y = pos.y + imageHeight / 2;
					SettingAttackPos();		//물풍선 놓는위치 지정
					OverlapCheck();			//물풍선 중복설치금지를 위한 코드
					characterValue.lastWaterBallonPos = attack.pos;	//마지막물풍선위치저장
					characterValue.waterBallonNumber++;				//캐릭터가 놓은 물풍선 수 증가
				}
			}
		}
		break;
	case CharacterColor::BLUE:
		if (characterValue.isMoveable)
		{
			characterValue.prevPos = pos;
			if (GetAsyncKeyState('W'))
			{
				dir = Direction::TOP;
			}
			if (GetAsyncKeyState('S'))
			{
				dir = Direction::BOTTOM;
			}
			if (GetAsyncKeyState('A'))
			{
				dir = Direction::LEFT;
			}
			if (GetAsyncKeyState('D'))
			{
				dir = Direction::RIGHT;
			}
			if (characterValue.isAttackPossible && (this->stats.bNum > characterValue.waterBallonNumber))
			{
				if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)		//공격
				{
					attack.isColor = CharacterColor::BLUE;
					attack.pos.x = pos.x;
					attack.pos.y = pos.y + imageHeight / 2;
					SettingAttackPos();		//물풍선 놓는위치 지정
					OverlapCheck();			//물풍선 중복설치 금지를 위한
					characterValue.lastWaterBallonPos = attack.pos;
					characterValue.waterBallonNumber++;				//캐릭터가 놓은 물풍선 수 증가
				}
			}
		}
		break;
	}
}

bool Character::CheckmDelay(ULONGLONG& animationTick, const int delayTime)
{
	if (GetTickCount64() > animationTick + delayTime)
	{
		animationTick = GetTickCount64();
		return true;
	}
	return false;
}

void Character::SetWaterBallonList(list<ObjectData::Position> waterBallonPos)
{
	this->waterBallonPos = waterBallonPos;
}

Attack& Character::GetAttack()
{
	return this->attack;
}

void Character::SettingBallonNumber(int color)
{
	characterValue.waterBallonNumber--;
}

const int Character::GetWaterBallonBLength()
{
	return stats.bLength;
}

void Character::UPBallonNumber()
{
	(stats.bNumMax != stats.bNum + 1) ? ++stats.bNum : stats.bNum = stats.bNumMax;
}

void Character::UPBallonLength()
{
	(stats.bLengthMax != stats.bLength + 1) ? ++stats.bLength : stats.bLength = stats.bLengthMax;
}

void Character::UPSetSpeed()
{
	(stats.speedMax != stats.speed + 1) ? ++stats.speed : stats.speed = stats.speedMax;
}

void Character::SetAttackArea(const AttackArea& attackArea)
{
	this->attackArea = attackArea;
}

void Character::OverlapCheck()
{
	if (waterBallonPos.empty())
		attack.isAttack = true;

	bool isExist = false;
	for (const auto& wbPos : waterBallonPos)
	{
		if (attack.pos.x == wbPos.x && attack.pos.y == wbPos.y)
			isExist = true;
	}
	if (isExist)
		return;

	attack.isAttack = true;

	if (characterValue.isRevisit == false)
		characterValue.isRevisit = true;
}

void Character::MapImmovableArea()
{
	//맵전체 이동범위제한
	if (pos.x <= MOVE_MIN_X)
		pos.x = MOVE_MIN_X;
	else if (pos.x >= MOVE_MAX_X)
		pos.x = MOVE_MAX_X;

	if (pos.y <= MOVE_MIN_Y)
		pos.y = MOVE_MIN_Y;
	else if (pos.y >= MOVE_MAX_Y)
		pos.y = MOVE_MAX_Y;
}

//벡터를 받는게 아니라, 배열에 저장되어있는 숫자값을 받아가지고 하는게 더 빠를듯
//바꿀려고 했는데 생각해보면 어차피 참조라 속도차이없을거고 const라 변화할일도없기도하고
//map[][]을 받아오면 다시 pos값 구해야하는데 또 map크기만큼 연산해야하니까 이게더 낫다고 생각함
void Character::StaticObjectmmovableArea(const list<Obj*>& inGameObjectVector)
{
	RECT characterRect{ pos.x, pos.y, pos.x + BLOCK_X, pos.y + BLOCK_Y };
	RECT objRect{ 0,0,0,0 };

	//블록과 벽이동범위제한..
	for (const auto& tempObj : inGameObjectVector)
	{
		if (tempObj->GetName() == MessageQueue::StringToEnum("background"))
			continue;
		else if (tempObj->GetName() == MessageQueue::StringToEnum("Block"))
		{
			objRect.left = tempObj->GetPosition().x;
			objRect.top = tempObj->GetPosition().y - SIZE_TUNING;
			objRect.right = tempObj->GetPosition().x + BLOCK_X;
			objRect.bottom = tempObj->GetPosition().y + BLOCK_Y - SIZE_TUNING;
		}
		else if (tempObj->GetName() == MessageQueue::StringToEnum("Wall"))
		{
			objRect.left = tempObj->GetPosition().x;
			objRect.top = tempObj->GetPosition().y;
			objRect.right = tempObj->GetPosition().x + BLOCK_X;
			objRect.bottom = tempObj->GetPosition().y + BLOCK_Y;
		}

		//충돌체크 -> 플레이어 방향체크 -> 플레이어가 벽에 부딪혔을때 옆으로 비껴나가야할지 체크
		if (characterRect.left < objRect.right && characterRect.right > objRect.left
			&& characterRect.top < objRect.bottom && characterRect.bottom > objRect.top)
		{
			switch (dir)
			{
			case Direction::TOP:
				if (objRect.right > characterRect.left&& objRect.right - characterRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x++;
				else if (objRect.left < characterRect.right && characterRect.right - objRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x--;
				characterValue.prevPos.y = objRect.bottom;
				//위에한줄 들어간 이유는 캐릭터 이동속도에따라서 캐릭터는 +5+6+7이런식으로 이동하는데
				//staticObject좌표는 20+47*x로 고정되어있어서 20+5하다보면 y좌표가 안맞아서 이동할 수 없음 그걸 맞춰주기위한코드
				break;

			case Direction::BOTTOM:
				if (objRect.right > characterRect.left&& objRect.right - characterRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x++;
				else if (objRect.left < characterRect.right && characterRect.right - objRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x--;
				characterValue.prevPos.y = objRect.top - BLOCK_Y;
				break;

			case Direction::LEFT:
				if (objRect.bottom > characterRect.top&& objRect.bottom - characterRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y++;
				else if (objRect.top < characterRect.bottom && characterRect.bottom - objRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y--;
				characterValue.prevPos.x = objRect.right;
				break;

			case Direction::RIGHT:
				if (objRect.bottom > characterRect.top&& objRect.bottom - characterRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y++;
				else if (objRect.top < characterRect.bottom && characterRect.bottom - objRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y--;
				characterValue.prevPos.x = objRect.left - BLOCK_X;
				break;
			}
			pos = characterValue.prevPos;
		}
	}
}

void Character::WaterBallonImmovableArea()
{
	RECT characterRect{ pos.x, pos.y, pos.x + BLOCK_X, pos.y + BLOCK_Y };
	RECT objRect{ 0,0,0,0 };

	//물풍선 이동범위 제한
	for (const auto& wbPos : waterBallonPos)
	{
		objRect.left = characterValue.lastWaterBallonPos.x;
		objRect.top = characterValue.lastWaterBallonPos.y - SIZE_TUNING;
		objRect.right = characterValue.lastWaterBallonPos.x + BLOCK_X;
		objRect.bottom = characterValue.lastWaterBallonPos.y + BLOCK_Y - SIZE_TUNING;

		if (characterValue.isRevisit)
		{
			if (!(characterRect.left < objRect.right && characterRect.right > objRect.left
				&& characterRect.top < objRect.bottom && characterRect.bottom > objRect.top))
			{
				characterValue.isRevisit = false;
			}
			else
				continue;
		}

		objRect.left = wbPos.x;
		objRect.top = wbPos.y - SIZE_TUNING;
		objRect.right = wbPos.x + BLOCK_X;
		objRect.bottom = wbPos.y + BLOCK_Y - SIZE_TUNING;

		//캐릭터위에 다른캐릭터가 물풍선을 놓은 경우 처리하는구문
		if (characterValue.prevPos.x < objRect.right && characterValue.prevPos.x + 40 > objRect.left
			&& characterValue.prevPos.y < objRect.bottom && characterValue.prevPos.y + 47 > objRect.top)
			continue;

		//비껴서 앞으로나가기 + 이동제한
		if (characterRect.left < objRect.right && characterRect.right > objRect.left
			&& characterRect.top < objRect.bottom && characterRect.bottom > objRect.top)
		{
			switch (dir)
			{
			case Direction::TOP:
			case Direction::BOTTOM:
				if (objRect.right > characterRect.left&& objRect.right - characterRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x++;
				else if (objRect.left < characterRect.right && characterRect.right - objRect.left <= PERMIT_RANGE)
					characterValue.prevPos.x--;
				break;
			case Direction::LEFT:
			case Direction::RIGHT:
				if (objRect.bottom > characterRect.top&& objRect.bottom - characterRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y++;
				else if (objRect.top < characterRect.bottom && characterRect.bottom - objRect.top <= PERMIT_RANGE)
					characterValue.prevPos.y--;
				break;
			}
			pos = characterValue.prevPos;
		}
	}
}

void Character::Trapped()
{
	//this->characterStats.speed -= 4;		//물풍선 갖히면 속도감소.. 현재는 키 누를때 실행이라 한번에 여려번입력됨

	characterValue.time = GetTickCount64();		//갇히는 시작시간
	characterValue.state = State::TRAPPED;		//상태바꾸기
	characterValue.isAttackPossible = false;		//공격불가능
}