#pragma once

#include "Actor.h"
#include "InputState.h"

class Character : public Actor
{
public:

	Character();
	virtual ~Character();

	virtual void Update(const InputState* input, float deltaTime) override;
	virtual void Draw() override;

	void SetVelocity(const VECTOR& velocity);
	VECTOR GetVelocity() const;

	void SetMoveSpeed(float speed);
	float GetMoveSpeed() const;

	void SetHP(int hp);
	int GetHP() const;

	void Damage(int damage);

protected:

	virtual void Move(float deltaTime);

protected:

	VECTOR Velocity;

	float MoveSpeed;

	int HP;
};