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
	initCamPos = VGet(0.f, 0.f, 0.f);
	initCamTarget = VGet(10.f, 0.f, 0.f);
	SetCameraPositionAndTarget_UpVecY(initCamPos, initCamTarget);
	angle = 0.0f;
	radius = 10.0f;
}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	//Spaceを押したときはゲームシーンへ移行する
	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<ResultState>(m_gameManager) };
		return trans;
	}

	if (input->IsKeyStay(KEY_INPUT_A)) {
		initCamPos.z -= 1.0f * deltaTime;
		SetCameraPositionAndTarget_UpVecY(initCamPos, initCamTarget);
	}
	if (input->IsKeyStay(KEY_INPUT_D)) {
		initCamPos.z += 1.0f * deltaTime;
		SetCameraPositionAndTarget_UpVecY(initCamPos, initCamTarget);
	}
	if (input->IsKeyStay(KEY_INPUT_W)) {
		initCamPos.y -= 1.0f * deltaTime;
		SetCameraPositionAndTarget_UpVecY(initCamPos, initCamTarget);
	}
	if (input->IsKeyStay(KEY_INPUT_S)) {
		initCamPos.y += 1.0f * deltaTime;
		SetCameraPositionAndTarget_UpVecY(initCamPos, initCamTarget);
	}

	angle += deltaTime * 180.0f;

	deltaValue = deltaTime;

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void InGameState::Draw() {
	GameData::DrawStringWithAnchor(100, GameData::windowHeight / 2, 0, 0.5f,
		GetColor(255, 255, 255), m_gameFontHandle, "Press Space \nto See Result");

	VECTOR top = VGet(100.0f, 50.0f, 0.0f);
	VECTOR mid = VGet(100.0f, 0.0f, 0.0f);
	VECTOR right = VGet(mid.x + cos(DX_PI_F / 180.0f * angle) * radius, mid.y, mid.z + sin(DX_PI_F / 180.0f * angle) * radius);
	VECTOR left = VGet(mid.x + cos(DX_PI_F / 180.0f * (angle + 180)) * radius, mid.y, mid.z + sin(DX_PI_F / 180.0f * (angle + 180)) * radius);

	float diff = 30.0f;
	VECTOR right2 = VGet(mid.x + cos(DX_PI_F / 180.0f * (angle - diff)) * radius, mid.y, mid.z + sin(DX_PI_F / 180.0f * (angle - diff)) * radius);
	VECTOR left2 = VGet(mid.x + cos(DX_PI_F / 180.0f * (angle + 180 - diff)) * radius, mid.y, mid.z + sin(DX_PI_F / 180.0f * (angle + 180 - diff)) * radius);

	DrawTriangle3D(
		top, right2, left2, GetColor(255, 255, 255), TRUE);
	DrawTriangle3D(
		top, right, left, GetColor(0, 0, 0), FALSE);


	DrawFormatString(0, 0, GetColor(0, 0, 0), "FPS : %.4f", deltaValue);
}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}