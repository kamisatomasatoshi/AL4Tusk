#pragma once
#include "GameObject3D.h"

class Colosseum
{
public:
	Colosseum();
	~Colosseum();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="viewProjection"></param>
	/// <param name="matProjection"></param>
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	GameObject3D* wallObject = nullptr;
	GameObject3D* floorObject = nullptr;
};

