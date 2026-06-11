#pragma once
#include "DxLib.h"

class Actor
{
public:

	Actor();
	virtual ~Actor();

	virtual void Update(float deltaTime);
	virtual void Draw();

	void SetPosition(const VECTOR& pos);
	VECTOR GetPosition() const;

	void SetRotation(const VECTOR& rot);
	VECTOR GetRotation() const;

	void SetScale(const VECTOR& scale);
	VECTOR GetScale() const;

	VECTOR GetForward() const;

	void SetModelHandle(int handle);

protected:

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
};