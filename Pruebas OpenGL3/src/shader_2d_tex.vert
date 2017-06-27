#version 150

// in_Position was bound to attribute index 0, in_Color was bound to attribute index 1 and in_Texcoord was bound to attribute index 2
attribute vec3 in_Position;
attribute vec4 in_Color;
attribute vec2 in_Texcoord;

// We output the ex_Color and tex_Coord variable to the next shader in the chain
out vec4 ex_Color;
out vec2 tex_Coord;

void main(void) {

    gl_Position = vec4(in_Position, 1.0f);

	// Pass the color and texture on to the fragment shader
    ex_Color = in_Color;
	tex_Coord = in_Texcoord;
}