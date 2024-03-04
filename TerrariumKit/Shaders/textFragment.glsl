#version 460 core
out vec4 FragmentColor;

in vec2 TextureCoords;

uniform Sampler2D texture1;

void main()
{
	FragmentColor = texture(texture1, TextureCoords);
}
