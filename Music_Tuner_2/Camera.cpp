#include "Camera.h"

Camera::Camera(std::shared_ptr<Character> player)
	:Actor(), player(player)
{
	camRelatedPos = VGet(0.0f, 100.0f, -300.0f);
	camPos = VAdd(player->GetPosition(), camRelatedPos);
	SetCameraPositionAndTarget_UpVecY(camPos, player->GetPosition());
	camAdditionalPos = { 0.0f };

	isMouseClicked = false;
	cameraLength = VSize(camRelatedPos);
	camRotValue = 1.0f;	//1s‚É90‹‰ñ“]‚·‚é‘¬“x
	mouseStartPos = { 0 };
	mouseMovingPos = { 0 };
	camRotationY = 0;
}

Camera::~Camera() {

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
		temp_angleY = camRotValue * diffVec.x;

		camRelatedPos.x = sin(DX_PI_F / 180.0f * (camRotationY + temp_angleY) * -1) * cameraLength;
		camRelatedPos.z = cos(DX_PI_F / 180.0f * (camRotationY + temp_angleY)) * cameraLength;
	}
	if (input->isMouseUp(0)) {
		camRotationY += temp_angleY;
	}

	//Update camera
	camPos = VAdd(player->GetPosition(), camRelatedPos);
	SetCameraPositionAndTarget_UpVecY(camPos, player->GetPosition());
	Actor::Update(input, deltaTime);
}

void Camera::Draw() {
	Actor::Draw();
}