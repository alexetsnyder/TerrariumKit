#pragma once

#include "Enums.h"
#include "ShaderProgram.h"

class IGameActor
{
	public:
		virtual void move(InputDirection direction) = 0;
		virtual void update() = 0;
		virtual void draw(const ShaderProgram& program) = 0;

		virtual ~IGameActor() {};
};