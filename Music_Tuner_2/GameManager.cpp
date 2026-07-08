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

	//SetDrawScreen(DX_SCREEN_BACK);

	//魚眼のためのスクリーン作成
	SceneScreen = MakeScreen(GameData::windowWidth, GameData::windowHeight, TRUE);
	SetDrawScreen(SceneScreen);

	//魚眼のためのshaderをロード
	FishEyePS = LoadPixelShader("Data/pixelshader_1.pso");
	FishEyeCB = CreateShaderConstantBuffer(sizeof(FishEyeParam));

	param.Strength = 100.0f;
	param.Padding[0] = 0.0f;
	param.Padding[1] = 0.0f;
	param.Padding[2] = 0.0f;

	FishEyeParam* buffer = (FishEyeParam*)GetBufferShaderConstantBuffer(FishEyeCB);
	*buffer = param;
	UpdateShaderConstantBuffer(FishEyeCB);
	SetShaderConstantBuffer(
		FishEyeCB,
		DX_SHADERTYPE_PIXEL,
		0);

	//ポリゴンの頂点の設定
	{
		Vert[0].pos = VGet(0.0f, 0.0f, 0.0f);
		Vert[0].rhw = 1.0f;
		Vert[0].dif = GetColorU8(255, 255, 255, 255);
		Vert[0].spc = GetColorU8(0, 0, 0, 0);
		Vert[0].u = 0.0f;
		Vert[0].v = 0.0f;
		Vert[0].su = 0.0f;
		Vert[0].sv = 0.0f;

		Vert[1].pos = VGet(GameData::windowWidth, 0.0f, 0.0f);
		Vert[1].rhw = 1.0f;
		Vert[1].dif = GetColorU8(255, 255, 255, 255);
		Vert[1].spc = GetColorU8(0, 0, 0, 0);
		Vert[1].u = 1.0f;
		Vert[1].v = 0.0f;
		Vert[1].su = 1.0f;
		Vert[1].sv = 0.0f;

		Vert[2].pos = VGet(0.0f, GameData::windowHeight, 0.0f);
		Vert[2].rhw = 1.0f;
		Vert[2].dif = GetColorU8(255, 255, 255, 255);
		Vert[2].spc = GetColorU8(0, 0, 0, 0);
		Vert[2].u = 0.0f;
		Vert[2].v = 1.0f;
		Vert[2].su = 0.0f;
		Vert[2].sv = 1.0f;

		Vert[3].pos = VGet(GameData::windowWidth, GameData::windowHeight, 0.0f);
		Vert[3].rhw = 1.0f;
		Vert[3].dif = GetColorU8(255, 255, 255, 255);
		Vert[3].spc = GetColorU8(0, 0, 0, 0);
		Vert[3].u = 1.0f;
		Vert[3].v = 1.0f;
		Vert[3].su = 1.0f;
		Vert[3].sv = 1.0f;

		Vert[4].pos = VGet(0.0f, GameData::windowHeight, 0.0f);
		Vert[4].rhw = 1.0f;
		Vert[4].dif = GetColorU8(255, 255, 255, 255);
		Vert[4].spc = GetColorU8(0, 0, 0, 0);
		Vert[4].u = 0.0f;
		Vert[4].v = 1.0f;
		Vert[4].su = 0.0f;
		Vert[4].sv = 1.0f;

		Vert[5].pos = VGet(GameData::windowWidth, 0.0f, 0.0f);
		Vert[5].rhw = 1.0f;
		Vert[5].dif = GetColorU8(255, 255, 255, 255);
		Vert[5].spc = GetColorU8(0, 0, 0, 0);
		Vert[5].u = 1.0f;
		Vert[5].v = 0.0f;
		Vert[5].su = 1.0f;
		Vert[5].sv = 0.0f;
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
	//魚眼のため、描画先を変更
	//シェーダー用画面に描画先を切り替える
	SetDrawScreen(SceneScreen);
	ClearDrawScreen();

	if (!m_currentState.empty())m_currentState.top()->Draw();
	else {
		DrawString(100, 100, "current Scene not exist", GetColor(0, 0, 0));
	}

	//描画が終わったら描画先を変更
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	//SceneScreenに描画した内容を魚眼に変更していくための設定
	SetUsePixelShader(FishEyePS);
	SetUseTextureToShader(0, SceneScreen);

	//実際に描画していく
	DrawPolygon2DToShader(Vert, 2);
	//DrawGraph(0, 0, SceneScreen, FALSE);

	//描画が終わったらシェーダー解除
	SetUsePixelShader(-1);


	//この後にUIなど魚眼にしたくないものを描画する
	DrawFormatString(0, 50, GetColor(0, 0, 0), "Shader is %d", FishEyePS);


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