#include "Player.h"
#include<cmath>

Vector3 BulletRota(Vector3 pos1, Vector3 pos2);
Vector3 BulletTrans(Vector3 pos1, Vector3 pos2);
float BulletScale(Vector3 pos1, Vector3 pos2);

Player::Player() {

}

Player::~Player() {
	delete player_;
}

void Player::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	GetCursorPos(&mousePos);

	player_ = new GameObject3D();
	player_->PreLoadModel("Resources/tofu/player.obj");
	player_->PreLoadTexture(L"Resources/tofu/player.png");
	player_->SetViewProjection(viewProjection);
	player_->SetMatProjection(matProjection);
	player_->Initialize();

	aim = new GameObject3D();
	aim->PreLoadModel("Resources/tofu/pointer.obj");
	aim->PreLoadTexture(L"Resources/tofu/pointer.png");
	aim->SetViewProjection(viewProjection);
	aim->SetMatProjection(matProjection);
	aim->Initialize();
	aim->worldTransform.scale = { 0.6f,0.6f,0.6f };

	bullet = new GameObject3D();
	bullet->PreLoadModel("Resources/tofu/tofu.obj");
	bullet->PreLoadTexture(L"Resources/bullet.png");
	bullet->SetViewProjection(viewProjection);
	bullet->SetMatProjection(matProjection);
	bullet->Initialize();
	bullet->worldTransform.scale = { 1,0.6f ,0.6f };


	start = new GameObject3D();
	start->PreLoadModel("Resources/tofu/tofu.obj");
	start->PreLoadTexture(L"Resources/bulletPoint.png");
	start->SetViewProjection(viewProjection);
	start->SetMatProjection(matProjection);
	start->Initialize();
	start->worldTransform.scale = { 0.6f,0.6f,0.6f };
	isStartDead = true;

	end = new GameObject3D();
	end->PreLoadModel("Resources/tofu/tofu.obj");
	end->PreLoadTexture(L"Resources/bulletPoint.png");
	end->SetViewProjection(viewProjection);
	end->SetMatProjection(matProjection);
	end->Initialize();
	end->worldTransform.scale = { 0.6f,0.6f,0.6f };
	isEndDead = true;

	Reset();
}

void Player::Reset() {
	player_->worldTransform.translation = pos;
	player_->worldTransform.rotation = { 0 , 0 , 0 };
	//aimは本体の子
	aim->worldTransform.translation = { 0,0,10 };
	bullet->worldTransform.translation = { 0 , 0 , 0 };
	isBulletDead = true;
	isStartDead = true;
	isEndDead = true;
	isDead = false;
	hp = hpMax;
	theta = 0;
}

void Player::Update() {

	Attack();
	Rotate();
	Move();
	
	player_->Update();

	//aimは本体の子
	aim->worldTransform.UpdateMatWorld();
	aim->worldTransform.matWorld *= player_->worldTransform.matWorld;
	//aimの制限
	//x方向の制限
	if (aim->worldTransform.matWorld.m[3][0] > stageEdge) {
		aim->worldTransform.matWorld.m[3][0] = stageEdge;
	}else if (aim->worldTransform.matWorld.m[3][0] < -stageEdge) {
		aim->worldTransform.matWorld.m[3][0] = -stageEdge;
	}
	//z方向の制限
	if (aim->worldTransform.matWorld.m[3][2] > stageEdge) {
		aim->worldTransform.matWorld.m[3][2] = stageEdge;
	}
	else if (aim->worldTransform.matWorld.m[3][2] < -stageEdge) {
		aim->worldTransform.matWorld.m[3][2] = -stageEdge;
	}
	//y方向の制限
	if (aim->worldTransform.matWorld.m[3][1] < 0) {
		aim->worldTransform.matWorld.m[3][1] = 0;
	}
	
	//aimのワールド行列の転送
	aim->forwardMatrix();

	start->Update();
	end->Update();

	bullet->Update();
}

void Player::Draw() {
	if (isDead == false) {
		player_->Draw();
		//aim->Draw();
	}
	if (isBulletDead == false) {
		bullet->Draw();
	}
	if (isStartDead == false) {
		start->Draw();
	}
	if (isEndDead == false) {
		end->Draw();
	}
}

void Player::Rotate() {
	const float PI = 3.141592;

	//マウスのモニター座標の取得
	oldMousePos = mousePos;
	GetCursorPos(&mousePos);

	//一定時間でカーソルをモニターの中心に移動
	cursorTimer++;
	if (cursorTimer > 30) {
		//カーソルの位置移動
		SetCursorPos(width / 2, height / 2);
		//保存してるマウス位置も中心に変更
		mousePos = { width / 2, height / 2 };
		oldMousePos = { width / 2, height / 2 };
		
		//タイマーのリセット
		cursorTimer = 0;
	}

	//GameSceneでカメラのターゲットをaimの座標にしているため
	//マウスのy座標に一定の変化がある時
	if (abs(mousePos.y - oldMousePos.y) > 0) {
		theta += -PI / sensitivity.y * (mousePos.y - oldMousePos.y);
		//角度制限
		if (theta < -PI / 5 * 2) {//下の制限
			theta = -PI / 5 * 2;
		}else if (theta > PI / 3) { //上の制限
			theta = PI / 3;
		}
		//aimのy座標を変化
		aim->worldTransform.translation.z = cos(theta) * aimDistance;
		aim->worldTransform.translation.y = sin(theta) * aimDistance;
	}
	//マウスのx座標に一定の変化がある時
	if (abs(mousePos.x - oldMousePos.x) > 0) {
		//本体を回転(子のaimも回転する)
		player_->worldTransform.rotation.y += (float)(mousePos.x - oldMousePos.x) / sensitivity.x;
	}
}

void Player::Move() {
	Vector3 velocity = { 0,0,0 };

	if (input.PushKey(DIK_A)) {
		velocity.x -= moveSpeed;
	}
	if (input.PushKey(DIK_D)) {
		velocity.x += moveSpeed;
	}
	if (input.PushKey(DIK_W)) {
		velocity.z += moveSpeed;
	}
	if (input.PushKey(DIK_S)) {
		velocity.z -= moveSpeed;
	}

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = bVelocity(velocity, player_->worldTransform);

	//移動
	player_->worldTransform.translation += velocity;
	
	//x方向の移動制限
	if (player_->worldTransform.translation.x > stageEdge) {
		player_->worldTransform.translation.x = stageEdge;
	}else if (player_->worldTransform.translation.x < -stageEdge) {
		player_->worldTransform.translation.x = -stageEdge;
	}
	//z方向の移動制限
	if (player_->worldTransform.translation.z > stageEdge) {
		player_->worldTransform.translation.z = stageEdge;
	}
	else if (player_->worldTransform.translation.z < -stageEdge) {
		player_->worldTransform.translation.z = -stageEdge;
	}
}

void Player::Attack() {
	//if (GetAsyncKeyState(VK_LBUTTON) == -32767) {
	//	//どっちも表示しているとき
	//	if (isEndDead == false) {
	//		//表示を消す
	//		isStartDead = true;
	//		isEndDead = true;
	//	}

	//	if (isStartDead) {
	//		//始点の設定
	//		start->worldTransform.translation.x = aim->worldTransform.matWorld.m[3][0];
	//		start->worldTransform.translation.y = aim->worldTransform.matWorld.m[3][1];
	//		start->worldTransform.translation.z = aim->worldTransform.matWorld.m[3][2];
	//		isStartDead = false;
	//	}
	//	else if (isEndDead)
	//	{
	//		//終点の設定
	//		end->worldTransform.translation.x = aim->worldTransform.matWorld.m[3][0];
	//		end->worldTransform.translation.y = aim->worldTransform.matWorld.m[3][1];
	//		end->worldTransform.translation.z = aim->worldTransform.matWorld.m[3][2];
	//		isEndDead = false;

	//		//設定された始点終点から弾のworldTransfromの計算
	//		bullet->worldTransform.rotation = BulletRota(start->worldTransform.translation, end->worldTransform.translation);
	//		bullet->worldTransform.translation = BulletTrans(start->worldTransform.translation, end->worldTransform.translation);
	//		bullet->worldTransform.scale.x = BulletScale(start->worldTransform.translation, end->worldTransform.translation);

	//		//弾の生存
	//		isBulletDead = false;
	//		bulletHp = bulletHpMax;

	//	}
	//}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform)
{
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

void Player::OnCollision()
{
	hp--;
}
void Player::bulletEnemyOncollision()
{
	isBulletDead = true;
	isStartDead = true;
	isEndDead = true;
}

void Player::bulletBulletOnCollision()
{
	bulletHp--;
	if (bulletHp <= 0) {
		isBulletDead = true;
		isStartDead = true;
		isEndDead = true;
	}
}


Vector3 Player::GetAimPos() {
	//子の為worldTransformではなくワールド行列から取得
	Vector3 pos;
	
	pos.x = aim->worldTransform.matWorld.m[3][0];
	pos.y = aim->worldTransform.matWorld.m[3][1];
	pos.z = aim->worldTransform.matWorld.m[3][2];

	return pos;
}

Vector3 BulletRota(Vector3 pos1, Vector3 pos2) {
	Vector3 rota;
	float PI = 3.141592;

	//アークタンジェントで角度の計算
	//向く面は同じ
	if (abs(pos2.z - pos1.z) <= abs(pos2.x - pos1.x) && pos2.x - pos1.x > 0) {
		rota.z = atan2(pos2.y - pos1.y, pos2.x - pos1.x);
		rota.y = -atan2(pos2.z - pos1.z, pos2.x - pos1.x);
		rota.x = 0;
	}
	else if (abs(pos2.z - pos1.z) <= abs(pos2.x - pos1.x) && pos2.x - pos1.x < 0) {
		rota.z = -atan2(pos2.y - pos1.y, pos2.x - pos1.x) + PI;
		rota.y = -atan2(pos2.z - pos1.z, pos2.x - pos1.x);
		rota.x = 0;
	}
	else if (abs(pos2.z - pos1.z) > abs(pos2.x - pos1.x) && pos2.z - pos1.z > 0) {
		rota.z = atan2(pos2.y - pos1.y, pos2.z - pos1.z);
		rota.y = -atan2(pos2.z - pos1.z, pos2.x - pos1.x);
		rota.x = 0;
	}
	else if (abs(pos2.z - pos1.z) > abs(pos2.x - pos1.x) && pos2.z - pos1.z < 0) {
		rota.z = -atan2(pos2.y - pos1.y, pos2.z - pos1.z) + PI;
		rota.y = -atan2(pos2.z - pos1.z, pos2.x - pos1.x);
		rota.x = 0;
	}


	return rota;
}

Vector3 BulletTrans(Vector3 pos1, Vector3 pos2) {
	Vector3 trans;;

	trans = pos1 + ((pos2 - pos1) / 2);

	return trans;
}

float BulletScale(Vector3 pos1, Vector3 pos2) {
	float length;
	length = sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2)) - 1;

	float scale;
	scale = length / 2;

	return scale;
}