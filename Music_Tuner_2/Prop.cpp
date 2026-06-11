#include "Prop.h"

Prop::Prop()
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