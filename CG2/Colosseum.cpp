#include "Colosseum.h"

Colosseum::Colosseum()
{
}

Colosseum::~Colosseum()
{
	delete wallObject;
	delete floorObject;
}

void Colosseum::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	wallObject = new GameObject3D();
	wallObject->PreLoadModel("Resources/colosseum/colosseum_Ver2.obj");
	wallObject->PreLoadTexture(L"Resources/colosseum/wall.png");
	wallObject->SetViewProjection(viewProjection);
	wallObject->SetMatProjection(matProjection);
	wallObject->Initialize();
	wallObject->worldTransform.translation.y = -1;
	wallObject->worldTransform.scale.x = 10;
	wallObject->worldTransform.scale.y = 10;
	wallObject->worldTransform.scale.z = 10;
	wallObject->worldTransform.UpdateMatWorld();

	floorObject = new GameObject3D();
	floorObject->PreLoadModel("Resources/colosseum/floor.obj");
	floorObject->PreLoadTexture(L"Resources/colosseum/floor.png");
	floorObject->SetViewProjection(viewProjection);
	floorObject->SetMatProjection(matProjection);
	floorObject->Initialize();
	floorObject->worldTransform.translation.y = -1;
	floorObject->worldTransform.scale.x = 10;
	floorObject->worldTransform.scale.y = 10;
	floorObject->worldTransform.scale.z = 10;
	floorObject->worldTransform.UpdateMatWorld();
}

void Colosseum::Update()
{
	wallObject->Update();
	floorObject->Update();
}

void Colosseum::Draw()
{
	wallObject->Draw();
	floorObject->Draw();
}
