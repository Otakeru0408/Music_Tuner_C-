#include "FisheyePerspective.h"

FisheyePerspective::FisheyePerspective() {

}

void FisheyePerspective::Init() {
	//fisheye
	//魚眼のためのスクリーン作成
	SceneScreen = MakeScreen(GameData::windowWidth, GameData::windowHeight, TRUE);

	//魚眼のためのshaderをロード
	FishEyePS = LoadPixelShader("Data/pixelshader_1.pso");
	FishEyeCB = CreateShaderConstantBuffer(sizeof(FishEyeParam));

	param = (FishEyeParam*)GetBufferShaderConstantBuffer(FishEyeCB);

	param->Strength = 1.0f;
	param->Zoom = 0.8f;
	param->Padding[0] = 0.0f;
	param->Padding[1] = 0.0f;
	SetupCamera_Perspective(DX_PI_F / 3);

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

void FisheyePerspective::Update(float deltaTime) {
	//魚眼のため、描画先を変更
	//シェーダー用画面に描画先を切り替える
	SetDrawScreen(SceneScreen);
	ClearDrawScreen();
}

void FisheyePerspective::Draw() {
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
}

void FisheyePerspective::FisheyeEnabled() {
	param->Strength = 2.0f;
	param->Zoom = 0.4f;
	SetupCamera_Perspective(DX_PI_F / 2);
}

void FisheyePerspective::FisheyeDisabled() {
	param->Strength = 1.0f;
	param->Zoom = 0.8f;
	SetupCamera_Perspective(DX_PI_F / 3);
}