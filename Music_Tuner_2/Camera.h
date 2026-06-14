#pragma once
#include "Actor.h"
#include "Character.h"

#include <math.h>

class Camera : public Actor {
public:
	Camera(std::shared_ptr<Character> player);
	virtual ~Camera();

	virtual void Update(const InputState* input, float deltaTime) override;
	virtual void Draw() override;

protected:
	VECTOR camPos;
	VECTOR camRelatedPos;
	VECTOR camAdditionalPos;

	float cameraLength;
	float camRotValue;
	float camRotationY;
	float temp_angleY;
	bool isMouseClicked;
	VECTOR mouseStartPos;
	VECTOR mouseMovingPos;
	std::shared_ptr<Character> player;
};