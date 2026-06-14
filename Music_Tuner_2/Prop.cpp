#include "Prop.h"

Prop::Prop(InGameState* _parent)
	:Actor(_parent)
{
}

Prop::~Prop()
{
}

void Prop::Update(const InputState* input, float deltaTime)
{
	Actor::Update(input, deltaTime);
}

void Prop::Draw()
{
	Actor::Draw();
}