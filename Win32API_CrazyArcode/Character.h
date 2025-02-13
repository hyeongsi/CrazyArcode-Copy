﻿#pragma once
#include "DynamicObject.h"

class Character : public DynamicObject
{
public:
	//임시추가 테스트용
	bool test = true;		//테스트용캐릭터무적 F1누르면 무적 F2누르면 해제

private:
	CharacterStatsData stats{ 0 };	//캐릭터의 정보.. 물풍선개수, 속도 등을 가지고있음
	Img trappedImage;				//trapped이미지 관련 변수들 저장
	Img dieImage;					//die이미지 관련 변수들 저장

	CharacterColor color;					//플레이어 색깔
	CharacterName characterName;			//플레이어 캐릭터 이름
	CharacterState state;					//플레이어 상태
	int dir;					//플레이어 이동방향
	int prevDir;				//이전이동방향
	int waterBallonNumber;		//놓여진 물풍선 개수

	bool isRevisit;				//마지막물풍선에서 범위밖으로 나갔는지 여부
	bool isAttackPossible;		//공격가능여부
	bool isMoveable;			//이동가능여부
	bool isOneClick;			//공격키 한번만 눌렀는지 체크할 변수

	POINT printPos;								//여러개 이미지중에서 출력할 위치
	ObjectData::Position prevPos;				//이전위치
	ObjectData::Position lastWaterBallonPos;	//마지막물풍선위치
	AttackValue attack;								//공격관련 변수들 저장
	
	ULONGLONG charAnimationTick;	//움직임 애니메이션 변수
	ULONGLONG deathTime;			//죽음애니메이션 변수

	list<ObjectData::Position> waterBallonPos;

	AttackArea attackArea;	//터지는 물풍선에 대한 정보,, 좌표, top, right, bottom, left

public:
	Character(const int name, const ObjectData::POSITION pos, const ObjectData::SIZE size, int hNumber, int vNumber, HBITMAP hBitmap, CharacterStatsData characterStats);
	~Character();
	void Init();
	void LoadDefaultImage(const pImageData trappedImage, const pImageData dieImage);	//trapped, die이미지 가져와서 저장

	virtual void Input();
	virtual void Update();
	void LateUpdate(const list<Obj*>& inGameObjects);		//업데이트한 데이터보고 변경할거 변경
	virtual void Render(HDC hDC, HDC memDc);

	bool CheckmDelay(ULONGLONG& animationTick, const int delayTime);
	void SetWaterBallonPos(list<ObjectData::Position> waterBallonPos);	//물풍선 위치 가져오기
	AttackValue& GetAttack();			//공격관련 데이터전송
	void SettingBallonNumber();

	const int GetWaterBallonLength();
	void SetAttackArea(const AttackArea& attackArea);
	void CheckTrappedCollision(Character* character);	//trapped상태에서 충돌했는지검사
	CharacterState GetState() const;
	void SetState(CharacterState state);
	CharacterColor GetColor() const;

	void WaterBallonNumberUP();	//물풍선 개수 UP
	void WaterBallonLengthUP();	//물풍선 길이 UP
	void SpeedUP();				//캐릭터 속도 UP

private:
	void SettingAttackPos();	//물풍선 위치 세팅(지정된 영역에만 설치되게)
	bool OverlapCheck();		//중복설치체크
	void MapImmovableArea();	//맵이동제한
	void StaticObjectmmovableArea(const list<Obj*>& inGameObjectVector);	//블럭과 벽이동제한
	void WaterBallonImmovableArea();	//물풍선이동제한
	void Trapped();						//물풍선 맞을경우 실행
	void CheckExplosionArea();		//폭발범위체크
	void TrappedAnimation();		//trapped애니메이션출력
	void DeathAnimation();			//death애니메이션출력

};