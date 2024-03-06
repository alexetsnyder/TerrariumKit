#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 textureCoords;

out vec2 TextureCoords;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position = projection * model * vec4(vertex, 1.0f);
	TextureCoords = textureCoords;
}
