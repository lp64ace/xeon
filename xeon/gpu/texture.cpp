#include "texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(int width, int height, bool allocate) :
    twidth(width), theight(height), allocated(allocate)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

void Texture::write_pixel(int x, int y, const glm::u8vec4& color)
{
    int index = (y * twidth) + x;
    buffer[index * 4 + 0] = color.r;
    buffer[index * 4 + 1] = color.g;
    buffer[index * 4 + 2] = color.b;
    buffer[index * 4 + 3] = color.a;
}

void Texture::upload_to_gpu()
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, twidth, theight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
