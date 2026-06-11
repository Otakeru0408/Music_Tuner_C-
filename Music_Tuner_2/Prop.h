#pragma once
#include "Actor.h"

class Prop : public Actor {
public:
	Prop();

	virtual ~Prop();

	virtual void Update(const InputState* input, float deltaTime) override;
	virtual void Draw() override;
protected:

};