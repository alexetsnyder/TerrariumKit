#pragma once

#include "Shape.h"
#include "Mesh.h"

class Cube : public Shape
{
	public:
		Cube();
		~Cube();

		void init() override;
		void draw() override;

	private:
		void sendBufferData() override;
        Mesh getCubeMesh();
};

