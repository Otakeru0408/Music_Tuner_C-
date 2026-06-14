#pragma once
#include <utility>
#include <tuple>

struct InputState {
	int key[256];
	int prevKey[256];
	int mouse_input[3];
	int prev_mouse_input[3];

	bool IsKeyDown(int key_code)const
	{
		return key[key_code] >= 1 && prevKey[key_code] == 0;
	}
	bool IsKeyStay(int key_code)const
	{
		return key[key_code] >= 1 && prevKey[key_code] >= 1;
	}
	bool IsKeyUp(int key_code)const
	{
		return key[key_code] == 0 && prevKey[key_code] >= 1;
	}
	bool isMouseDown(int key)const
	{
		if (key > 3)return false;
		return mouse_input[key] >= 1 && prev_mouse_input[key] == 0;
	}
	bool isMouseStay(int key) const
	{
		if (key > 3)return false;
		return mouse_input[key] >= 1 && prev_mouse_input[key] >= 1;
	}
	bool isMouseUp(int key)const
	{
		if (key > 3)return false;
		return mouse_input[key] == 0 && prev_mouse_input[key] >= 1;
	}

	std::tuple<int, int> GetMousePos() const
	{
		int x, y;
		GetMousePoint(&x, &y);
		return { x,y };
	}
};