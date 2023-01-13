#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"

#include"Player.h"

class Player;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update();

	void Attack();

	void Draw();

	void Move();

	void Rota();

	void Reset();
	
	void typeC();

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	void SetPlayer(Player* player) { player_ = player; }

	WorldTransform GetWorldTransform() { return gameObject->worldTransform; }
	float GetRadius() { return gameObject->worldTransform.scale.x; }

	void OnCollision();

	int GetHp() { return hp; }
	int GetCt() { return ct; }
private:
	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject = nullptr;
	Vector3 pos = { 0,0,10 };

	Player* player_ = nullptr;

	//�̗�
	const int hpMax = 10;
	int hp = hpMax;

	int time = 60;//�v���C���[�ɂ��Ă���悤�Ƀx�N�g���̍X�V
	float speed = 0.2f;

	int typeTime = 0;

	int ct = 0;

//--------�U��------------
	//���˂̊Ԋu
	const int delayTime = 50;
	//���˃^�C�}�[
	int attackTimer = delayTime;

	//����<frm>
	static const int32_t kLiteTime = 60 * 5;

	const int stageEdge = 88;

public:
	//�e�̐�
	static const int bulletNum = 64;
	GameObject3D* bullets_[bulletNum];
	//�e�̐���
	bool bulletIsDead[bulletNum];

private:
	//�f�X�^�C�}�[
	int32_t deathTimer_[bulletNum];
	//�e�̑��x
	Vector3 velocity[bulletNum];

	WorldTransform pPos;// = player_->GetWorldTransform();
	Vector3 num = {0,0,0};// = pPos.translation - gameObject->worldTransform.translation;

	bool isInvincible;
	const int invincibleTime = 60;
	int invincibleTimer = invincibleTime;

	enum class Type
	{
		pos1,
		pos2,
		pos3,
		pos4
	};
	Type type;
};