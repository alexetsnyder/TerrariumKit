#pragma once

#include "Shape.h"

class Cube : public Shape
{
	public:
		Cube();
		~Cube();

		void init() override;
		void draw() override;

	private:
		void sendBufferData() override;
};

