#pragma once
#include "Actor.h"

class Prop : public Actor {
public:
	Prop(InGameState* _parent);

	virtual ~Prop();

	virtual void Update(const InputState* input, float deltaTime) override;
	virtual void Draw() override;
protected:

};