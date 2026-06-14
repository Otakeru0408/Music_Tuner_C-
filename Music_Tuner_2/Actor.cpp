#include "Actor.h"

#include <math.h>

Actor::Actor(InGameState* _parent)
	:parent(_parent), IsActive(true), IsVisible(true), WorldMatrix(MGetIdent())
{
	Position = VGet(0, 0, 0);
	Rotation = VGet(0, 0, 0);
	Scale = VGet(1, 1, 1);

	ModelHandle = -1;
}

Actor::~Actor()
{
}

void Actor::Update(const InputState* input, float deltaTime)
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
	UpdateWorldMatrix();
}

VECTOR Actor::GetPosition() const
{
	return Position;
}

void Actor::SetRotation(const VECTOR& rot)
{
	Rotation = rot;
	MV1SetRotationXYZ(ModelHandle, Rotation);
	UpdateWorldMatrix();
}

VECTOR Actor::GetRotation() const
{
	return Rotation;
}

void Actor::SetScale(const VECTOR& scale)
{
	Scale = scale;
	MV1SetScale(ModelHandle, Scale);
	UpdateWorldMatrix();
}

VECTOR Actor::GetScale() const
{
	return Scale;
}

void Actor::UpdateWorldMatrix()
{
	MATRIX scaleMat = MGetScale(Scale);

	//åªç›ÇÕä»ó™âªÇÃÇΩÇﬂYé≤âÒì]ÇµÇ©ÇµÇ»Ç¢ÇÊÇ§Ç…ÇµÇƒÇ¢ÇÈÅB
	MATRIX rotMat = MGetRotY(Rotation.y);

	MATRIX transMat = MGetTranslate(Position);

	WorldMatrix =
		MMult(
			MMult(scaleMat, rotMat),
			transMat);
}

VECTOR Actor::GetForward() const
{
	VECTOR forward;

	forward.x = WorldMatrix.m[2][0];
	forward.y = WorldMatrix.m[2][1];
	forward.z = WorldMatrix.m[2][2];

	return VNorm(forward);
}

VECTOR Actor::GetRight() const
{
	VECTOR right;

	right.x = WorldMatrix.m[0][0];
	right.y = WorldMatrix.m[0][1];
	right.z = WorldMatrix.m[0][2];

	return VNorm(right);
}

VECTOR Actor::GetUp() const
{
	VECTOR up;

	up.x = WorldMatrix.m[1][0];
	up.y = WorldMatrix.m[1][1];
	up.z = WorldMatrix.m[1][2];

	return VNorm(up);
}

void Actor::SetModelHandle(int handle) {
	ModelHandle = handle;
}