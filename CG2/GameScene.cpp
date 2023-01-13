#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	soundManager_.SoundUnload(selectSound);
}

void GameScene::Initialize(WinApp* winApp) {
	scene = Scene::Title;

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	viewProjection_.Initialize();
	camera.translation = Vector3{ 0,3,-8 };

	//XAudioエンジンのインスタンスを生成
	soundManager_.Initialize();



	//プレイヤー
	player_ = new Player();
	player_->Initialize(&viewProjection_, &matProjection_);


	colosseum_ = new Colosseum;
	colosseum_->Initialize(&viewProjection_, &matProjection_);

	//enemy
	enemy_ = new Enemy();
	enemy_->Initialize(&viewProjection_, &matProjection_);
	enemy_->SetPlayer(player_);
	
//----------------
	//スプライト読み込み
	Sprite::LoadTexture(1, L"Resources/title.png");
	Sprite::LoadTexture(2, L"Resources/GameClear.png");
	Sprite::LoadTexture(3, L"Resources/GameOver.png");
	Sprite::LoadTexture(4, L"Resources/plife2.png");
	Sprite::LoadTexture(5, L"Resources/elife2.png");
	Sprite::LoadTexture(6, L"Resources/a.png");

	//スプライトの設定
	title_ = Sprite::Create(1, { 0 , 0 });
	clear_ = Sprite::Create(2, { 0 , 0 });
	gameOver_ = Sprite::Create(3, { 0 , 0 });
	sousa_ = Sprite::Create(6, {  0, 464 });

	life_1  = Sprite::Create(4, { lifeSize*0 ,0 }); 
	life_2  = Sprite::Create(4, { lifeSize*1 ,0 });
	life_3  = Sprite::Create(4, { lifeSize*2 ,0 });
	life_4  = Sprite::Create(4, { lifeSize*3 ,0 });
	life_5  = Sprite::Create(4, { lifeSize*4 ,0 });
	life_6  = Sprite::Create(4, { lifeSize*5 ,0 });
	life_7  = Sprite::Create(4, { lifeSize*6 ,0 });
	life_8  = Sprite::Create(4, { lifeSize*7 ,0 });
	life_9  = Sprite::Create(4, { lifeSize*8 ,0 });
	life_10 = Sprite::Create(4, { lifeSize*9 ,0 });

	eLife_1  = Sprite::Create(5, { lifeSize * 39 ,0 });
	eLife_2  = Sprite::Create(5, { lifeSize * 38 ,0 });
	eLife_3  = Sprite::Create(5, { lifeSize * 37 ,0 });
	eLife_4  = Sprite::Create(5, { lifeSize * 36 ,0 });
	eLife_5  = Sprite::Create(5, { lifeSize * 35 ,0 });
	eLife_6  = Sprite::Create(5, { lifeSize * 34 ,0 });
	eLife_7  = Sprite::Create(5, { lifeSize * 33 ,0 });
	eLife_8  = Sprite::Create(5, { lifeSize * 32 ,0 });
	eLife_9  = Sprite::Create(5, { lifeSize * 31 ,0 });
	eLife_10 = Sprite::Create(5, { lifeSize * 30 ,0 });
}

void GameScene::Update() {
	switch (scene)
	{
	case Scene::Title:
		GameScene::Reset();

		if (input_.TriggerKey(DIK_SPACE)) {
			//カーソルの非表示
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
			scene = Scene::Play;
			ShowCursor(FALSE);
			/*enemy_->Reset();
			player_->Reset();*/
			
		}
		


		break;
	case Scene::Play:

		//当たり判定
		CheckAllCollisions();

		//プレイヤー
		player_->Update();

		//------カメラ-----
			//情報取得
		playerWorldTransform = player_->GetWorldTransform();

		//ターゲットはPlayerのaim座標
		viewProjection_.target = player_->GetAimPos();

		//カメラの座標はプレイヤーの子
		camera.UpdateMatWorld();
		camera.matWorld *= playerWorldTransform.matWorld;
		viewProjection_.eye.x = camera.matWorld.m[3][0];
		viewProjection_.eye.y = camera.matWorld.m[3][1];
		viewProjection_.eye.z = camera.matWorld.m[3][2];

		viewProjection_.UpdateView();

		//----------
		enemy_->Update();
		//colosseum->Update();
		colosseum_->Update();


		if (enemy_->GetHp() <= 0) {
			scene = Scene::Clear;
			//カーソルの表示
			ShowCursor(TRUE);
		}
		else if (player_->GetHp() <= 0) {
			scene = Scene::Gameover;
			//カーソルの表示
			ShowCursor(TRUE);
		}


		break;
	case Scene::Clear:
		if (input_.TriggerKey(DIK_SPACE)) {
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
			scene = Scene::Title;
		}

		break;
	case Scene::Gameover:
		if (input_.TriggerKey(DIK_SPACE)) {
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
			scene = Scene::Title;
		}

		break;
	}
}

void GameScene::Draw() {

	//3D描画
	switch (scene)
	{
	case Scene::Title:
		
		break;
	case Scene::Play:
		player_->Draw();
		enemy_->Draw();
		//colosseum->Draw();
		colosseum_->Draw();

		break;
	case Scene::Clear:

		break;
	case Scene::Gameover:

		break;
	}


	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());
	switch (scene)
	{
	case Scene::Title:
		title_->Draw();
		sousa_->Draw();
		break;
	case Scene::Play:
		lifeDraw();
		break;
	case Scene::Clear:
		clear_->Draw();

		break;
	case Scene::Gameover:
		gameOver_->Draw();

		break;
	}

	Sprite::PostDraw();
}

void GameScene::Reset() {
	enemy_->Reset();
	player_->Reset();
	

}

void GameScene::CheckAllCollisions() {
	//敵と自弾
	//情報の取得
	WorldTransform playerWorldTransform = player_->GetWorldTransform();
	Vector3 bulletStart = player_->GetStart();
	Vector3 bulletEnd = player_->GetEnd();
	float bulletRadius = player_->GetRadius();
	WorldTransform enemyWorldTransform = enemy_->GetWorldTransform();
	float enemyRadius = enemy_->GetRadius();
//------弾の当たり判定生成--------
	//棒の長さ
	float bulletLength = sqrt(pow(bulletEnd.x - bulletStart.x, 2) + pow(bulletEnd.y - bulletStart.y, 2) + pow(bulletEnd.z - bulletStart.z, 2));
	//球の必要数
	int colliderNum = bulletLength / bulletRadius;
	//球の同士の間隔
	Vector3 Distance = { (bulletEnd.x - bulletStart.x) / colliderNum, (bulletEnd.y - bulletStart.y) / colliderNum, (bulletEnd.z - bulletStart.z) / colliderNum };
	//球の位置設定
	Vector3 bulletCollider[100];
	for (int i = 0; i < colliderNum; i++) {
		bulletCollider[i] = bulletStart + Distance * i;
	}

//-------------自弾と敵の当たり判定------------------
	//自弾と敵の半径の合計
	float length = bulletRadius + enemyRadius;

	//自機弾が存在するなら
	if (player_->GetIsBulletDead() == false) {
		//弾の球全てに対して
		for (int i = 0; i < colliderNum; i++) {
			//球と敵の距離
			float Distance = sqrt(
				pow(enemyWorldTransform.translation.x - bulletCollider[i].x, 2) +
				pow(enemyWorldTransform.translation.y - bulletCollider[i].y, 2) +
				pow(enemyWorldTransform.translation.z - bulletCollider[i].z, 2));


			if (Distance <= length) {
				enemy_->OnCollision();
				player_->bulletEnemyOncollision();
				break;
			}
		}
	}

//---------------敵弾と自機の判定-----------------------
	//自機と敵弾の半径の合計
	length = playerWorldTransform.scale.x + enemy_->bullets_[0]->worldTransform.scale.x;

	for (int i = 0; i < enemy_->bulletNum; i++) {
		//出てる弾のみ
		if (enemy_->bulletIsDead[i] == false) {
			//自機と敵弾の距離
			float Distance = sqrt(
				pow(playerWorldTransform.translation.x - enemy_->bullets_[i]->worldTransform.translation.x, 2) +
				pow(playerWorldTransform.translation.y - enemy_->bullets_[i]->worldTransform.translation.y, 2) +
				pow(playerWorldTransform.translation.z - enemy_->bullets_[i]->worldTransform.translation.z, 2));

			//当たってるなら
			if (Distance <= length) {
				enemy_->bulletIsDead[i] = true;
				player_->OnCollision();
			}
		}
	}


	
	
}

void GameScene::lifeDraw()
{
	int playerHp = player_->GetHp();
	if (playerHp >= 1)
	{
		life_1->Draw();
	}
}
