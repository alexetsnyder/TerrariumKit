#pragma once
#include "Shape.h"

class Triangle : public Shape
{
	public:
		Triangle();
		~Triangle();

		void init() override;

		void draw() override;

	private:
		void sendBufferData() override;
};

