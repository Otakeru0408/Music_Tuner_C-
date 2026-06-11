#pragma once
#include "IGameState.h"
#include "Actor.h"
#include "Prop.h"
#include "Character.h"

#include <memory>
#include <vector>

class GameManager;

class InGameState :public IGameState {
public:
	InGameState(GameManager* gameManager) :IGameState(gameManager) {}
	~InGameState()override = default;

	void Init()override;
	SceneTransition* Update(const InputState* input, float deltaTime)override;
	void Draw()override;
	void Terminate()override;

private:
	int m_gameFontHandle;
	VECTOR camPos;
	VECTOR camRelatedPos;

	//models
	int box01Handle;
	VECTOR box01Pos;

	//actors
	std::vector<std::shared_ptr<Actor>> actors;
	std::shared_ptr<Prop> box01;
	std::shared_ptr<Character> player01;
};