#pragma once
#include "GameObject3D.h"

class Colosseum
{
public:
	Colosseum();
	~Colosseum();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="viewProjection"></param>
	/// <param name="matProjection"></param>
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	GameObject3D* wallObject = nullptr;
	GameObject3D* floorObject = nullptr;
};

