#pragma once
#include "ShaderParam.h"
#include "DxLib.h"
#include "GameData.h"


class FisheyePerspective {
public:
	FisheyePerspective();

	void Init();

	void Update(float deltaTime);
	void Draw();

	void FisheyeEnabled();
	void FisheyeDisabled();

private:
	int SceneScreen = 0;
	int FishEyePS = 0;
	FishEyeParam* param;
	int FishEyeCB = -1;
	VERTEX2DSHADER Vert[6];
};