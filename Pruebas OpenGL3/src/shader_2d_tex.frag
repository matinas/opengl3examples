#version 150

// It was expressed that some drivers required this next line to function properly
precision highp float;

in vec4 ex_Color;
in vec2 tex_Coord;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex_decal;

void main(void) {
	
	vec4 base_tex = texture(tex, tex_Coord);
	vec4 decal_tex = texture(tex_decal, tex_Coord);

	fragColor = mix(base_tex,decal_tex,0.2f);
}