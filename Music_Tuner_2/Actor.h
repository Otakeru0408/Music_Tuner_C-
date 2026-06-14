#pragma once
#include "DxLib.h"
#include "InputState.h"

#include <memory>

class InGameState;

class Actor
{
public:

	Actor(InGameState* _parent);
	virtual ~Actor();

	virtual void Update(const InputState* input, float deltaTime);
	virtual void Draw();

	void SetPosition(const VECTOR& pos);
	VECTOR GetPosition() const;

	void SetRotation(const VECTOR& rot);
	VECTOR GetRotation() const;

	void SetScale(const VECTOR& scale);
	VECTOR GetScale() const;

	VECTOR GetForward() const;
	VECTOR GetRight() const;
	VECTOR GetUp() const;

	void SetModelHandle(int handle);

protected:

	void UpdateWorldMatrix();

	// Transform
	VECTOR Position;
	VECTOR Rotation;
	VECTOR Scale;

	// WorldMatrix
	MATRIX WorldMatrix;

	// Model
	int ModelHandle;

	// èÛë‘
	bool IsActive;
	bool IsVisible;
	InGameState* parent;
};