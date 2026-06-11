#pragma once
#include "Actor.h"

class Prop : public Actor {
public:
	Prop();

	virtual ~Prop();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
protected:

};