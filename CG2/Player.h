#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"

class Player {
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update();

	void Attack();

	void Draw();

	void Reset();

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	void OnCollision();
	void bulletEnemyOncollision();
	void bulletBulletOnCollision();

	//ゲッター
	Vector3 GetAimPos();
	WorldTransform GetWorldTransform() { return player_->worldTransform; }
	int GetIsDead() { return isDead; }
	Vector3 GetStart() { return start->worldTransform.translation; }
	Vector3 GetEnd() { return end->worldTransform.translation; }
	float GetRadius() { return bullet->worldTransform.scale.y; }
	Vector3 GetAngle() { return angle; }
	bool GetIsBulletDead() { return isBulletDead; }

	int GetHp() { return hp; }

	//メンバ関数
private:
	void Rotate();
	void Move();

	//メンバ変数
private:
	//キー入力
	Input& input = Input::GetInstance();
	POINT mousePos;
	POINT oldMousePos;
	int cursorTimer;
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	Vector3 angle = {};

	

	//マウス感度
	Vector2 sensitivity = { 450,450};

	//本体
	GameObject3D* player_ = nullptr;
	Vector3 pos = { 0, 0, -10 };
	int isDead = false;
	//移動速度
	const float moveSpeed = 0.5f;


//----攻撃----
	GameObject3D* bullet = nullptr;
	GameObject3D* aim = nullptr;
	GameObject3D* start = nullptr;
	GameObject3D* end = nullptr;

	bool isBulletDead = true;
	bool isStartDead = true;
	bool isEndDead = true;

	const int hpMax = 1;
	int hp = hpMax;


//-----制限-----
	const int stageEdge = 88;
	const float aimDistance = 10;
	float theta;
	int bulletHp;
	const int bulletHpMax = 3;
};