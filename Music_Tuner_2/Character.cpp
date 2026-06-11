#include "Character.h"

Character::Character()
{
	Velocity = VGet(0.0f, 0.0f, 0.0f);

	MoveSpeed = 100.0f;

	HP = 100;
}

Character::~Character()
{
}

void Character::Update(const InputState* input, float deltaTime)
{
	Velocity = VGet(0.0f, 0.0f, 0.0f);

	//vertical movement
	if (input->IsKeyStay(KEY_INPUT_A)) {
		Velocity = VAdd(Velocity, VGet(-MoveSpeed * deltaTime, 0.0f, 0.0f));
	}
	if (input->IsKeyStay(KEY_INPUT_D)) {
		Velocity = VAdd(Velocity, VGet(MoveSpeed * deltaTime, 0.0f, 0.0f));
	}
	//horizontal movement
	if (input->IsKeyStay(KEY_INPUT_S)) {
		Velocity = VAdd(Velocity, VGet(0.0f, 0.0f, -MoveSpeed * deltaTime));
	}
	if (input->IsKeyStay(KEY_INPUT_W)) {
		Velocity = VAdd(Velocity, VGet(0.0f, 0.0f, MoveSpeed * deltaTime));
	}

	VECTOR newDiff = VAdd(VScale(GetForward(), Velocity.z), VScale(GetRight(), Velocity.x));

	Position = VAdd(Position, newDiff);
	SetPosition(Position);

	//rotation
	/*if (input->IsKeyStay(KEY_INPUT_Q)) {
		Rotation = VAdd(Rotation, VGet(0.0f, DX_PI_F / 180.0f * 90.0f * deltaTime, 0.0f));
	}
	if (input->IsKeyStay(KEY_INPUT_E)) {
		Rotation = VAdd(Rotation, VGet(0.0f, -DX_PI_F / 180.0f * 90.0f * deltaTime, 0.0f));
	}

	SetRotation(Rotation);*/
	auto [mx, my] = input->GetMousePos();
	//ここでマウスの左右移動から視点を回転させたい。

	Actor::Update(input, deltaTime);
}

void Character::Draw() {
	Actor::Draw();
	DrawFormatString(10, 300, GetColor(0, 0, 0), "Vel : %.2f, %.2f, %.2f", Velocity.x, Velocity.y, Velocity.z);
}

void Character::Move(float deltaTime)
{
	Position = VAdd(
		Position,
		VScale(Velocity, deltaTime));
}

void Character::SetVelocity(const VECTOR& velocity)
{
	Velocity = velocity;
}

VECTOR Character::GetVelocity() const
{
	return Velocity;
}

void Character::SetMoveSpeed(float speed)
{
	MoveSpeed = speed;
}

float Character::GetMoveSpeed() const
{
	return MoveSpeed;
}

void Character::SetHP(int hp)
{
	HP = hp;
}

int Character::GetHP() const
{
	return HP;
}

void Character::Damage(int damage)
{
	HP -= damage;

	if (HP <= 0)
	{
		HP = 0;
		IsActive = false;
	}
}