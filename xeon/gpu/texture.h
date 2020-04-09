#pragma once
#include <glm/glm.hpp>
#include <cstdint>

class Texture {
public:
	Texture(int width, int height);
	~Texture();

	void write_pixel(int x, int y, const glm::u8vec4& color);
	uint8_t* get_buffer() { return buffer; }

	void upload_to_gpu();
	void unbind();
	void bind();

private:
	int twidth, theight;
	uint32_t texture, pbo;
	uint8_t* buffer;
};