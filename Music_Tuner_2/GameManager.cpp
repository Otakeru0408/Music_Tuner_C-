#include "GameManager.h"
#include "DxLib.h"
#include "TitleState.h"
#include "InGameState.h"
#include "ResultState.h"

GameManager::GameManager() :m_isGameFinished(false) {
	memset(m_inputState.key, 0, sizeof(m_inputState.key));
	memset(m_inputState.prevKey, 0, sizeof(m_inputState.prevKey));
	memset(m_inputState.mouse_input, 0, sizeof(m_inputState.mouse_input));
	memset(m_inputState.prev_mouse_input, 0, sizeof(m_inputState.prev_mouse_input));
}

GameManager::~GameManager() {

}

void GameManager::Initialize() {
	if (DxLib_Init() == -1) {
		return;
	}

	SetWaitVSyncFlag(TRUE);


	//ChangeState(std::make_unique<TitleState>(this));
	PushState(std::make_unique<TitleState>(this));
	currentState()->Init();

	m_prevTime = GetNowCount();
}

void GameManager::Update() {
	if (ProcessMessage() != 0) {
		return;
	}

	UpdateInputState();

	//deltaTimeの計算
	LONGLONG nowTime = GetNowHiPerformanceCount();
	float deltaTime = (nowTime - m_prevTime) / 1000000.0f;
	m_prevTime = nowTime;

	//SceneのUpdate
	SceneTransition* transition = m_currentState.top()->Update(&m_inputState, deltaTime);
	if (transition->type == TransitionType::Change) {
		m_currentState.top() = std::move(transition->nextState);
		m_currentState.top()->Init();
	}
	else if (transition->type == TransitionType::Popup) {
		PushState(std::move(transition->nextState));
		currentState()->Init();
	}
	else if (transition->type == TransitionType::Resume) {
		if (!m_currentState.empty())
			PopState();
		//Initializeはしない。画面が元に戻る
	}
}

void GameManager::Draw() {
	if (!m_currentState.empty())m_currentState.top()->Draw();
	else {
		DrawString(100, 100, "current Scene not exist", GetColor(0, 0, 0));
	}

	int x, y;
	GetMousePoint(&x, &y);
	DrawFormatString(10, 10, GetColor(0, 0, 0), "x:%d y:%d", x, y);

	ScreenFlip();
}

void GameManager::Finalize() {
	while (!m_currentState.empty()) {
		m_currentState.pop();
	}

	DxLib_End();
}

void GameManager::UpdateInputState()
{
	// 現在のキー状態を前フレームのキー状態として保存
	memcpy(m_inputState.prevKey, m_inputState.key, sizeof(m_inputState.key));

	char tmpKey[256];
	// Dxライブラリから現在のキー状態を取得
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {
			m_inputState.key[i]++;
		}
		else {
			m_inputState.key[i] = 0;
		}
	}

	// TODO: マウスやゲームパッドの入力もここに追加
	//マウスの入力を前の入力として保存
	memcpy(m_inputState.prev_mouse_input, m_inputState.mouse_input, sizeof(m_inputState.mouse_input));

	int mouse_input = GetMouseInput();
	if (mouse_input & MOUSE_INPUT_LEFT)m_inputState.mouse_input[0]++;
	else m_inputState.mouse_input[0] = 0;

	if (mouse_input & MOUSE_INPUT_RIGHT)m_inputState.mouse_input[1]++;
	else m_inputState.mouse_input[1] = 0;

	if (mouse_input & MOUSE_INPUT_MIDDLE)m_inputState.mouse_input[2]++;
	else m_inputState.mouse_input[2] = 0;
}