#pragma once

#include "Shape.h"

class Cube : public Shape
{
	public:
		void init() override;
		void draw() override;
	private:
		void sendBufferData() override;
};

