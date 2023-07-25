#version 330 core

// Tret de https://learnopengl.com/In-Practice/Text-Rendering

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;
uniform bool esBack;

void main()
{    
    if(esBack){
        color = vec4(0,0,0,0.3);
        return;
    }
    float alfa = texture(text, TexCoords).r;
    if(alfa == 0) {
        discard;
    }
    vec4 sampled = vec4(1.0, 1.0, 1.0, alfa);
    color = vec4(textColor, 1.0) * sampled;
}  