#version 150

// It was expressed that some drivers required this next line to function properly
precision highp float;

in vec4 ex_Color;

float rand(vec2 co);

bool randomColor = true;

void main(void) {
    gl_FragColor = vec4(ex_Color);

	if (randomColor)
    {
        gl_FragColor.r  = (ex_Color.r * 0.5) + (rand(ex_Color.ra) * 0.5);
        gl_FragColor.g  = (ex_Color.g * 0.5) + (rand(ex_Color.bg) * 0.5);
        gl_FragColor.b  = (ex_Color.b * 0.5) + (rand(ex_Color.gb) * 0.5);
        gl_FragColor.a  = ex_Color.a;
    }
    else
        gl_FragColor = ex_Color;
}

float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy,vec2(a,b));
    highp float sn = mod(dt,3.14);
    return fract(sin(sn) * c);
}