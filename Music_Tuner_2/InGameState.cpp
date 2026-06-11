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

	camPos = VGet(0.0f, 0.0f, -1000.0f);
	focusPos = VGet(0.0f, 0.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(camPos, focusPos);

	//load Models
	box01 = std::make_shared<Prop>();
	box01->SetModelHandle(MV1LoadModel("Data/SampleBox01.mv1"));
	box01->SetPosition(VGet(0.0f, 0.0f, 0.0f));

	actors.emplace_back(box01);
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
		actor->Update(deltaTime);
	}


	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void InGameState::Draw() {
	for (auto actor : actors) {
		actor->Draw();
	}
}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}