#pragma once

#include "Shape.h"
#include "Mesh.h"
#include "TextureAtlas.h"

#include <string>

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
		std::string getFaceName(int face);

		TextureAtlas _atlas;
};