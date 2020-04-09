#pragma once
#include <glm/glm.hpp>
#include <string>

enum class ShaderType {
    Vertex = 0x8B31,
    Fragment = 0X8B30
};

class Shader {
public:
    Shader() = default;
    ~Shader();

    void bind();
    void unbind();

    void set_int(const char* str, int val);
    void set_vec2(const char* str, const glm::vec2& val);

    void load(const std::string& filepath, ShaderType type);
    void build();

    uint32_t raw();

private:
    void check_errors(uint32_t shader, ShaderType type);

private:
    uint32_t shader_id;
    uint32_t vertex, fragment;
};