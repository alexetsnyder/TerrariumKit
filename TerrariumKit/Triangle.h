#pragma once
#include "Shape.h"

class Triangle : public Shape
{
	public:
		void init() override;

		void draw() override;

	private:
		void sendBufferData() override;
};

