#version 150

// in_Position was bound to attribute index 0, in_Color was bound to attribute index 1 and in_Texcoords was bound to attribute index 2
attribute vec3 in_Position;
attribute vec4 in_Color;
attribute vec2 in_Texcoord;
 
// We output the ex_Color and tex_Coords variables to the next shader in the chain
out vec4 ex_Color;
out vec2 tex_Coords;

void main(void) {

    gl_Position = vec4(-in_Position*2.0f, 1.0f);  // We multiply by 2 in order to reutilize square_tex vertices (used for RENDER_2D_TEX) and cover all the screen, (-1.0,1.0) range
												  // We negate the in_Position in order to get the frame buffer texture correctly on the screen
												  // In order to avoid doing this we should have created another set of vertices similar to square_tex in model.h and mapping the correct texture coordinates there
 
    // Pass the color and texture coordinates on to the fragment shader
    ex_Color = in_Color;
	tex_Coords = in_Texcoord;
}