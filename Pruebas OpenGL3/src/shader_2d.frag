#version 120

// It was expressed that some drivers required this next line to function properly
precision highp float;

uniform vec4 passed_Color;

bool interpolate = true;
bool invertColors = false;

in vec4 ex_Color;

void main(void) {

	if (interpolate)
	{
		if (!invertColors)
			gl_FragColor = vec4(ex_Color);
		else
			gl_FragColor = vec4(1.0f-ex_Color.r,1.0f-ex_Color.g,1.0f-ex_Color.b,1.0);
	}
	else
	{
		if (!invertColors)
			gl_FragColor = vec4(passed_Color);
		else
			gl_FragColor = vec4(1.0f-passed_Color.r,1.0f-passed_Color.g,1.0f-passed_Color.b,1.0f);
	}
}