#version 150

// Each version of OpenGL has its own version of the shader language with availability of a certain feature set and we will be using GLSL 1.50. This version number
// may seem a bit off when we're using OpenGL 3.2, but that's because shaders were only introduced in OpenGL 2.0 as GLSL 1.10. Starting from OpenGL 3.3, this problem
// was solved and the GLSL version is the same as the OpenGL version

// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
attribute vec3 in_Position;
attribute vec4 in_Color;
 
uniform mat4 mvp;

// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;

void main(void) {

    gl_Position = mvp * vec4(in_Position, 1.0f);
 
    // Pass the color on to the fragment shader
    ex_Color = in_Color;
}