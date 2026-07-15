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
	box01->SetPosition(VGet(0.0f, 0.0f, 500.0f));
	actors.emplace_back(box01);
	MV1SetupCollInfo(box01->GetModelHandle(), -1, 2, 2, 2);

	std::shared_ptr<Prop> plane01 = std::make_shared<Prop>(this);
	plane01->SetModelHandle(MV1LoadModel("Data/SamplePlane01.mv1"));
	plane01->SetPosition(VGet(0.0f, -100.0f, 0.0f));
	plane01->SetScale(VGet(10.0f, 1.0f, 10.0f));
	actors.emplace_back(plane01);

	player01 = std::make_shared<Character>(this);
	player01->SetModelHandle(MV1LoadModel("Data/SampleCharacter.mv1"));
	player01->SetScale(VGet(0.5f, 0.5f, 0.5f));
	actors.emplace_back(player01);
	MV1SetFrameVisible(player01->GetModelHandle(), 0, FALSE);
	MV1SetupCollInfo(player01->GetModelHandle(), 0, 2, 2, 2);

	mainCam = std::make_shared<Camera>(this, player01);
	mainCam->SetCamRelatedPos(VGet(0.0f, 200.0f, -400.0f));
	mainCam->SetTargetRelatedPos(VGet(0.0f, 100.0f, 0.0f));
	//処理の順番として、先頭にカメラを挿入する
	actors.insert(actors.begin(), mainCam);

	//light settings
	SetUseLighting(TRUE);
	SetLightDirection(VGet(0.0f, 1.0f, 0.0f));

	//fisheye
	//魚眼のためのスクリーン作成
	SceneScreen = MakeScreen(GameData::windowWidth, GameData::windowHeight, TRUE);

	//魚眼のためのshaderをロード
	FishEyePS = LoadPixelShader("Data/pixelshader_1.pso");
	FishEyeCB = CreateShaderConstantBuffer(sizeof(FishEyeParam));

	param.Strength = 2.0f;
	param.Zoom = 0.7f;
	param.Padding[0] = 0.0f;
	param.Padding[1] = 0.0f;

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

}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {

	//魚眼のため、描画先を変更
	//シェーダー用画面に描画先を切り替える
	SetDrawScreen(SceneScreen);
	ClearDrawScreen();


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

	SetupCamera_Perspective(DX_PI_F / 2);

	for (auto actor : actors) {
		actor->Draw();
	}


	//描画が終わったら描画先を変更
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	//SceneScreenに描画した内容を魚眼に変更していくための設定
	SetUsePixelShader(FishEyePS);
	SetUseTextureToShader(0, SceneScreen);

	//シェーダーへ渡す値の更新
	UpdateShaderConstantBuffer(FishEyeCB);
	SetShaderConstantBuffer(
		FishEyeCB,
		DX_SHADERTYPE_PIXEL,
		0);

	//実際に描画していく
	DrawPolygon2DToShader(Vert, 2);
	//DrawGraph(0, 0, SceneScreen, FALSE);

	//描画が終わったらシェーダー解除
	SetUsePixelShader(-1);

	//この後にUIなど魚眼にしたくないものを描画する

	//DrawSphere3D(VGet(1000.0f, 100.0f, 0.0f), 100.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	/*VECTOR playerPos = player01->GetPosition();
	DrawFormatString(50, 200, GetColor(0, 0, 0), "player x:%.2f,y:%.2f,z:%.2f", playerPos.x, playerPos.y, playerPos.z);
	VECTOR camPos = mainCam->GetPosition();
	DrawFormatString(50, 400, GetColor(0, 0, 0), "camera x:%.2f,y:%.2f,z:%.2f", camPos.x, camPos.y, camPos.z);*/

}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}