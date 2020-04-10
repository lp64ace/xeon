#pragma once
#include <glm/glm.hpp>
#include <cstdint>

class Texture {
public:
	Texture(int width, int height, bool allocate = true);
	~Texture();

	void write_pixel(int x, int y, const glm::u8vec4& color);

	void upload_to_gpu();
	void unbind();
	void bind();

public:
	int twidth, theight;
	uint32_t texture;

	bool allocated;
	uint8_t* buffer;
};