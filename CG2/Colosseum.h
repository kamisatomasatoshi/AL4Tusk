#pragma once
#include "GameObject3D.h"

class Colosseum
{
public:
	Colosseum();
	~Colosseum();
	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	/// <param name="viewProjection"></param>
	/// <param name="matProjection"></param>
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	/// <summary>
	/// XVˆ—
	/// </summary>
	void Update();
	/// <summary>
	/// •`‰æˆ—
	/// </summary>
	void Draw();

private:
	GameObject3D* wallObject = nullptr;
	GameObject3D* floorObject = nullptr;
};

