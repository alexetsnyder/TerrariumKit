#pragma once
class Shape
{
	public:
		virtual void init() = 0;

		virtual void draw() = 0;	

	protected:
		virtual void sendBufferData() = 0;

		void genBuffers();
		void bindBuffers();

		void bindVertexArray();
		void bindVertexBuffer();

		void unbindVertexArray();

		unsigned int _vao;
		unsigned int _vbo;
};

