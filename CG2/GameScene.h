#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"

#include "Audio.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include "Sprite.h"

#include "Player.h"
#include "Particle.h"

#include "Colosseum.h"
#include "Enemy.h"

class GameScene {

public: // メンバ関数

	//コンストラクタ
	GameScene();

	/// デストラクタ
	~GameScene();

	//初期化
	void Initialize(WinApp* winApp);

	//毎フレーム処理
	void Update();

	//描画処理
	void Draw();

	void Reset();

	void CheckAllCollisions();

	void lifeDraw();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	SoundManager soundManager_;

	SoundData selectSound = soundManager_.SoundLoadWave("Resources/selectSound.wav");

	ViewProjection viewProjection_;
	WorldTransform camera;


	enum class Scene
	{
		Title,
		Play,
		Clear,
		Gameover,
	};
	Scene scene;

	float lifeSize = 32;
	
//-------
	Sprite* title_ = nullptr;
	Sprite* clear_ = nullptr;
	Sprite* gameOver_ = nullptr;
	Sprite* sousa_ = nullptr;

	Sprite* life_1 = nullptr;
	Sprite* life_2 = nullptr;
	Sprite* life_3 = nullptr;
	Sprite* life_4 = nullptr;
	Sprite* life_5 = nullptr;
	Sprite* life_6 = nullptr;
	Sprite* life_7 = nullptr;
	Sprite* life_8 = nullptr;
	Sprite* life_9 = nullptr;
	Sprite* life_10 = nullptr;

	Sprite* eLife_1 = nullptr;
	Sprite* eLife_2 = nullptr;
	Sprite* eLife_3 = nullptr;
	Sprite* eLife_4 = nullptr;
	Sprite* eLife_5 = nullptr;
	Sprite* eLife_6 = nullptr;
	Sprite* eLife_7 = nullptr;
	Sprite* eLife_8 = nullptr;
	Sprite* eLife_9 = nullptr;
	Sprite* eLife_10 = nullptr;

//-------


	//プレイヤー
	Player* player_ = nullptr;
	WorldTransform playerWorldTransform;

	//敵
	Enemy* enemy_ = nullptr;
	WorldTransform enemyWorldTransform;

	//ゲームオブジェクト
	Colosseum* colosseum_ = nullptr;
	
};