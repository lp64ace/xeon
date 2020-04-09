#version 330 core
in vec2 v_tex;
uniform sampler2D texSampler;
out vec4 frag_color;

void main()
{
    frag_color = texture(texSampler, v_tex);
}