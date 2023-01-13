#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"

class Player {
public:
	//�R���X�g���N�^
	Player();
	//�f�X�g���N�^
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

	//�Q�b�^�[
	Vector3 GetAimPos();
	WorldTransform GetWorldTransform() { return player_->worldTransform; }
	int GetIsDead() { return isDead; }
	Vector3 GetStart() { return start->worldTransform.translation; }
	Vector3 GetEnd() { return end->worldTransform.translation; }
	float GetRadius() { return bullet->worldTransform.scale.y; }
	Vector3 GetAngle() { return angle; }
	bool GetIsBulletDead() { return isBulletDead; }

	int GetHp() { return hp; }

	//�����o�֐�
private:
	void Rotate();
	void Move();

	//�����o�ϐ�
private:
	//�L�[����
	Input& input = Input::GetInstance();
	POINT mousePos;
	POINT oldMousePos;
	int cursorTimer;
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	Vector3 angle = {};

	

	//�}�E�X���x
	Vector2 sensitivity = { 450,450};

	//�{��
	GameObject3D* player_ = nullptr;
	Vector3 pos = { 0, 0, -10 };
	int isDead = false;
	//�ړ����x
	const float moveSpeed = 0.5f;


//----�U��----
	GameObject3D* bullet = nullptr;
	GameObject3D* aim = nullptr;
	GameObject3D* start = nullptr;
	GameObject3D* end = nullptr;

	bool isBulletDead = true;
	bool isStartDead = true;
	bool isEndDead = true;

	const int hpMax = 1;
	int hp = hpMax;


//-----����-----
	const int stageEdge = 88;
	const float aimDistance = 10;
	float theta;
	int bulletHp;
	const int bulletHpMax = 3;
};