#pragma once
#include "Actor.h"
#include "Character.h"

#include <math.h>

class Camera : public Actor {
public:
	Camera(InGameState* _parent, std::shared_ptr<Character> player);
	virtual ~Camera();

	virtual void Update(const InputState* input, float deltaTime) override;
	virtual void Draw() override;

	void SetCamRelatedPos(VECTOR relatedPos);
	void SetTargetRelatedPos(VECTOR relatedPos);

protected:
	//カメラ本体の位置
	VECTOR camPos;
	//注視点からカメラへの相対位置
	VECTOR camRelatedPos;
	//ターゲットから相対位置で指定する注視点
	VECTOR targetRelatedPos;

	//ターゲットとカメラの距離
	float cameraLength;
	//マウスの入力を角度に変換する変数。マウス感度みたいなパラメータ
	float camRotValue;
	//マウス入力開始時の基準となる回転角度
	float camRotationY;

	//マウス入力中のY軸回転値
	float temp_angleY = 0.0f;
	bool isMouseClicked;
	VECTOR mouseStartPos;
	VECTOR mouseMovingPos;
	std::shared_ptr<Character> player;
};