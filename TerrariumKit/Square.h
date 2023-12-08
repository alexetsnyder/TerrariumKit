#pragma once

#include "Shape.h"

class Square : public Shape
{
public:
	Square();
	~Square();

	void init() override;
	void draw() override;

private:
	void sendBufferData() override;

	GLuint _ebo{0};
};

