#include "InGameState.h"
#include "GameManager.h"
#include "ResultState.h"
#include "DxLib.h"

#include <math.h>

void InGameState::Init() {
	SetBackgroundColor(0, 174, 149);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_gameFontHandle = CreateFontToHandle("YDW あおさぎ R", 25, 3);

	//load Models
	box01 = std::make_shared<Prop>(this);
	box01->SetModelHandle(MV1LoadModel("Data/SampleBox01.mv1"));
	box01->SetPosition(VGet(0.0f, 0.0f, 0.0f));
	//actors.emplace_back(box01);

	std::shared_ptr<Prop> plane01 = std::make_shared<Prop>(this);
	plane01->SetModelHandle(MV1LoadModel("Data/SamplePlane01.mv1"));
	plane01->SetPosition(VGet(0.0f, -100.0f, 0.0f));
	plane01->SetScale(VGet(10.0f, 1.0f, 10.0f));
	actors.emplace_back(plane01);

	player01 = std::make_shared<Character>(this);
	player01->SetModelHandle(MV1LoadModel("Data/SampleBox01.mv1"));
	player01->SetScale(VGet(0.5f, 0.5f, 0.5f));
	actors.emplace_back(player01);

	mainCam = std::make_shared<Camera>(this, player01);
	actors.emplace_back(mainCam);

	//light settings
	SetUseLighting(TRUE);
	SetLightDirection(VGet(0.0f, 1.0f, 0.0f));

}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	//Spaceを押したときはゲームシーンへ移行する
	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<ResultState>(m_gameManager) };
		return trans;
	}

	//Update all actors
	for (auto actor : actors) {
		actor->Update(input, deltaTime);
	}

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void InGameState::Draw() {
	for (auto actor : actors) {
		actor->Draw();
	}
	VECTOR playerPos = player01->GetPosition();
	DrawFormatString(50, 200, GetColor(0, 0, 0), "player x:%.2f,y:%.2f,z:%.2f", playerPos.x, playerPos.y, playerPos.z);
	VECTOR camPos = mainCam->GetPosition();
	DrawFormatString(50, 400, GetColor(0, 0, 0), "camera x:%.2f,y:%.2f,z:%.2f", camPos.x, camPos.y, camPos.z);
}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}