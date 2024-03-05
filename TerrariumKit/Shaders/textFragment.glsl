#version 460 core
out vec4 FragmentColor;

in vec2 TextureCoords;

uniform sampler2D texture1;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(texture1, TextureCoords).r);
    FragmentColor = vec4(textColor, 1.0f) * sampled;
}
