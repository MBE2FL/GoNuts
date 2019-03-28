#version 420
in vec2 texcoord;
out vec4 color;

layout(binding = 0) uniform sampler2D text;
uniform vec4 uTextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texcoord).r);
    color = uTextColor * sampled;
}  