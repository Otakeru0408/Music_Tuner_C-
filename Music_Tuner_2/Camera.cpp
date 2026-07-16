#include "Camera.h"

Camera::Camera(InGameState* _parent, std::shared_ptr<Character> player)
	:Actor(_parent), player(player)
{
	camRelatedPos = VGet(0.0f, 100.0f, -300.0f);
	targetRelatedPos = { 0.0f };

	isMouseClicked = false;
	cameraLength = VSize(camRelatedPos);
	camRotValue = 0.7f;	//1s‚É90‹‰ñ“]‚·‚é‘¬“x
	mouseStartPos = { 0 };
	mouseMovingPos = { 0 };
	camRotationY = 180;// atan2f(camRelatedPos.z, camRelatedPos.x)* (180.0f / DX_PI_F);	//Šp“x‚É•ÏŠ·‚·‚é

	camPos = VAdd(camRelatedPos, VAdd(player->GetPosition(), targetRelatedPos));
	SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player->GetPosition(), targetRelatedPos));
}

Camera::~Camera() {

}

void Camera::SetCamRelatedPos(VECTOR relatedPos) {
	camRelatedPos = relatedPos;
	camPos = VAdd(camRelatedPos, VAdd(player->GetPosition(), targetRelatedPos));
	SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player->GetPosition(), targetRelatedPos));
	cameraLength = VSize(camRelatedPos);
}

void Camera::SetTargetRelatedPos(VECTOR relatedPos) {
	targetRelatedPos = relatedPos;
	camPos = VAdd(camRelatedPos, VAdd(player->GetPosition(), targetRelatedPos));
	SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player->GetPosition(), targetRelatedPos));
	cameraLength = VSize(camRelatedPos);
}

void Camera::Update(const InputState* input, float deltaTime)
{
	//get mouse input
	if (input->isMouseDown(0)) {
		auto [mx, my] = input->GetMousePos();
		mouseStartPos.x = mx;  mouseStartPos.y = my; mouseStartPos.z = 0; //Z‚ÍŽg‚í‚È‚¢
	}
	if (input->isMouseStay(0)) {
		auto [mx, my] = input->GetMousePos();
		mouseMovingPos.x = mx; mouseMovingPos.y = my; mouseMovingPos.z = 0;
		VECTOR diffVec = VSub(mouseMovingPos, mouseStartPos);
		temp_angleY = camRotValue * -diffVec.x;

		camRelatedPos.x = sin(DX_PI_F / 180.0f * (camRotationY + temp_angleY) * -1) * cameraLength;
		camRelatedPos.z = cos(DX_PI_F / 180.0f * (camRotationY + temp_angleY)) * cameraLength;
	}
	if (input->isMouseUp(0)) {
		camRotationY += temp_angleY;
	}

	//Update camera
	camPos = VAdd(camRelatedPos, VAdd(player->GetPosition(), targetRelatedPos));
	SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player->GetPosition(), targetRelatedPos));
	SetPosition(camPos);
	Actor::Update(input, deltaTime);
}

void Camera::Draw() {
	Actor::Draw();

	/*DrawFormatString(100, 200, GetColor(0, 0, 0), "X:%.2f,Y:%.2f,Z:%.2f", mouseStartPos.x, mouseStartPos.y, mouseStartPos.z);
	DrawFormatString(100, 250, GetColor(0, 0, 0), "X:%.2f,Y:%.2f,Z:%.2f", mouseMovingPos.x, mouseMovingPos.y, mouseMovingPos.z);
	DrawFormatString(100, 300, GetColor(0, 0, 0), "CamRotY:%.2f", camRotationY);
	DrawFormatString(100, 350, GetColor(0, 0, 0), "temp_Y:%.2f", temp_angleY);*/
}