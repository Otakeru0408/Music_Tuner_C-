#include "Actor.h"

#include <math.h>

Actor::Actor()
	:IsActive(true), IsVisible(true), WorldMatrix(MGetIdent())
{
	Position = VGet(0, 0, 0);
	Rotation = VGet(0, 0, 0);
	Scale = VGet(1, 1, 1);

	ModelHandle = -1;
}

Actor::~Actor()
{
}

void Actor::Update(float deltaTime)
{
}

void Actor::Draw()
{
	if (ModelHandle >= 0)
	{
		MV1DrawModel(ModelHandle);
	}
}

void Actor::SetPosition(const VECTOR& pos)
{
	Position = pos;
	MV1SetPosition(ModelHandle, Position);
}

VECTOR Actor::GetPosition() const
{
	return Position;
}

void Actor::SetRotation(const VECTOR& rot)
{
	Rotation = rot;
	MV1SetRotationXYZ(ModelHandle, Rotation);
}

VECTOR Actor::GetRotation() const
{
	return Rotation;
}

void Actor::SetScale(const VECTOR& scale)
{
	Scale = scale;
	MV1SetScale(ModelHandle, Scale);
}

VECTOR Actor::GetScale() const
{
	return Scale;
}

VECTOR Actor::GetForward() const
{
	VECTOR forward;

	// Rotation.y ‚ðYŽ²‰ñ“](ƒ‰ƒWƒAƒ“)‚Æ‰¼’è
	forward.x = sinf(Rotation.y);
	forward.y = 0.0f;
	forward.z = cosf(Rotation.y);

	return VNorm(forward);
}

void Actor::SetModelHandle(int handle) {
	ModelHandle = handle;
}