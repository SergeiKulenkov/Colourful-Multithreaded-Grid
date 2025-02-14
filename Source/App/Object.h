#pragma once
#include "imgui.h"

class IDrawableObject
{
public:
	virtual void Update(float deltaTime) {}
	virtual void Draw() = 0;
};