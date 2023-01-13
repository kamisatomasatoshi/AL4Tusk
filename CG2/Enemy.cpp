#include"Enemy.h"
#include<cmath>

Enemy::Enemy() {
}

Enemy::~Enemy() {
	delete gameObject;
}

void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	type = Type::pos1;
	//敵本体生成
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(L"Resources/tofu/tofu.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	
	gameObject->Initialize();
	gameObject->worldTransform.scale.x = 10;
	gameObject->worldTransform.scale.y = 10;
	gameObject->worldTransform.scale.z = 10;


	//弾生成
	for (int i = 0; i < bulletNum; i++) 
	{
		bullets_[i] = new GameObject3D();
		bullets_[i]->PreLoadModel("Resources/tofu/tofu.obj");
		bullets_[i]->PreLoadTexture(L"Resources/tofu.png");
		bullets_[i]->SetViewProjection(viewProjection);
		bullets_[i]->SetMatProjection(matProjection);
		bullets_[i]->Initialize();

		bullets_[i]->worldTransform.scale = { 2,2,2 };
	}

	Reset();
}

void Enemy::Reset() {
	//本体の座標初期
	gameObject->worldTransform.translation = pos;

	hp = hpMax;

	isInvincible = false;
	invincibleTimer = invincibleTime;

	//弾のフラグリセット
	for (int i = 0; i < bulletNum; i++) {
		bulletIsDead[i] = true;
	}
	num = { 0,0,0 };
	time = 60;
}

void Enemy::typeC()
{
	typeTime++;

	if (typeTime >= 60 * 10 && type == Type::pos1)
	{
		type = Type::pos2;
		typeTime = 0;
	}

	if (typeTime >= 60 * 10 && type == Type::pos2)
	{
		type = Type::pos3;
		typeTime = 0;
	}

	if (typeTime >= 60 * 10 && type == Type::pos3)
	{
		type = Type::pos4;
		typeTime = 0;
	}

	if (typeTime >= 60 * 10 && type == Type::pos4)
	{
		type = Type::pos1;
		typeTime = 0;
	}

	

}

 
void Enemy::Update() {
	Rota();
	Attack();
	typeC();
	Move();

	//出てる弾の処理
	for (int i = 0; i < bulletNum; i++) {
		if (bulletIsDead[i] == false) {
			//時間経過で弾を削除
			if (--deathTimer_[i] <= 0)
			{
				bulletIsDead[i] = true;
			}
			//弾の移動
			bullets_[i]->worldTransform.translation += velocity[i];
			bullets_[i]->Update();
			//x
			if (bullets_[i]->worldTransform.translation.x > stageEdge) {
				bulletIsDead[i] = true;
			}
			else if (bullets_[i]->worldTransform.translation.x < -stageEdge) {
				bulletIsDead[i] = true;
			}
			//z
			if (bullets_[i]->worldTransform.translation.z> stageEdge) {
				bulletIsDead[i] = true;
			}
			else if (bullets_[i]->worldTransform.translation.z < -stageEdge) {
				bulletIsDead[i] = true;
			}
		}
	}

	if (isInvincible == true) {
		invincibleTimer--;
		if (invincibleTimer <= 0) {
			isInvincible = false;
		}
	}

	gameObject->Update();
}

void Enemy::Attack() {
	attackTimer--;
	//一定時間で弾発射
	if (attackTimer <= 0)
	{
		for (int i = 0; i < bulletNum; i++) {
			//出ていない弾を選ぶ
			if (bulletIsDead[i] == true) {
				bulletIsDead[i] = false;
				//弾の寿命をセット
				deathTimer_[i] = kLiteTime;
				//弾の速度
				const float kBulletSpeed = 0.6f;
				velocity[i] = { kBulletSpeed, 0, 0 };
				//速度ベクトルを自機の向きに合わせて回転させる
				velocity[i] = bVelocity(velocity[i], gameObject->worldTransform);

				switch (type)
				{
				case Enemy::Type::pos1:

					//弾の初期位置を敵と同じに
					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					break;
				case Enemy::Type::pos2:

					//弾の初期位置を敵と同じに
					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					break;
				case Enemy::Type::pos3:
					//弾の初期位置を敵と同じに
					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					break;
				case Enemy::Type::pos4:
					//弾の初期位置を敵と同じに
					bullets_[i]->worldTransform.translation.x = gameObject->worldTransform.translation.x;
					bullets_[i]->worldTransform.translation.y = 0;
					bullets_[i]->worldTransform.translation.z = gameObject->worldTransform.translation.z;

					break;

				default:
					break;
				}

				

				break;
			}
		}
		//攻撃の時間リセット
		attackTimer = delayTime;
	}


	


}

void Enemy::Move()
{

	
	switch (type)
	{
	case Enemy::Type::pos1:
		
	gameObject->worldTransform.translation.x = 0;
	gameObject->worldTransform.translation.y = 44;
	gameObject->worldTransform.translation.z = 90;
		break;
	case Enemy::Type::pos2:

	gameObject->worldTransform.translation.x = 90;
	gameObject->worldTransform.translation.y = 44;
	gameObject->worldTransform.translation.z = 0;
		break;
	case Enemy::Type::pos3:
		
	gameObject->worldTransform.translation.x = -90;
	gameObject->worldTransform.translation.y = 44;
	gameObject->worldTransform.translation.z = 0;

		break;
	case Enemy::Type::pos4:
		
		gameObject->worldTransform.translation.x = 0;
		gameObject->worldTransform.translation.y = 44;
		gameObject->worldTransform.translation.z = 90;
		break;

	default:
		break;
	}
	

	

	
	


}

//プレイヤーのほうを向く
void Enemy::Rota() {
	WorldTransform playerWorldTransform = player_->GetWorldTransform();

	gameObject->worldTransform.rotation.y = -atan2(playerWorldTransform.translation.z - gameObject->worldTransform.translation.z, playerWorldTransform.translation.x - gameObject->worldTransform.translation.x);
}


void Enemy::Draw() {
	if (invincibleTimer % 2 == 0) {
		gameObject->Draw();
	}
	for (int i = 0; i < bulletNum; i++) {
		if (bulletIsDead[i] == false) {
			bullets_[i]->Draw();
		}
	}
}

void Enemy::OnCollision() {
	if (isInvincible == false) {
		hp--;
		invincibleTimer = invincibleTime;
		isInvincible = true;
	}
	//gameObject->worldTransform.translation.x += 10;
}

Vector3 Enemy::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[0][2] +
		velocity.y * worldTransform.matWorld.m[1][2] +
		velocity.z * worldTransform.matWorld.m[2][2];

	result.x = velocity.x * worldTransform.matWorld.m[0][0] +
		velocity.y * worldTransform.matWorld.m[1][0] +
		velocity.z * worldTransform.matWorld.m[2][0];

	result.y = velocity.x * worldTransform.matWorld.m[0][1] +
		velocity.y * worldTransform.matWorld.m[1][1] +
		velocity.z * worldTransform.matWorld.m[2][1];

	return result;
}