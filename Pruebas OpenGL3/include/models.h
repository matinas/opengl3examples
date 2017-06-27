#ifndef MODELS_H
#define MODELS_H

#include <SDL.h>
#include <SDL_opengl.h>

using namespace std;

const uint32_t cube_vertex_count = 36;
const uint32_t cube_vertex_depth = 3;
//const GLfloat cube[cube_vertex_count][cube_vertex_depth] =	// Enable this for drawing the cube as GL_QUADS (must enable also drayArray(GL_QUAD) in draw() function)
//{
//	
//	{1.0f, 1.0f,  1.0f},	// Top face (y = 1.0f)
//	{-1.0f, 1.0f,  1.0f},
//	{-1.0f, 1.0f, -1.0f},
//	{1.0f, 1.0f, -1.0f},
//	{1.0f, -1.0f, -1.0f},	// Bottom face (y = -1.0f)
//	{-1.0f, -1.0f, -1.0f},
//	{-1.0f, -1.0f,  1.0f},
//	{1.0f, -1.0f,  1.0f},
//	{1.0f,  1.0f, -1.0f},	// Front face (z = -1.0f)
//	{-1.0f,  1.0f, -1.0f},
//	{-1.0f, -1.0f, -1.0f},
//	{1.0f, -1.0f, -1.0f},	
//	{1.0f, -1.0f, 1.0f},	// Back face  (z = 1.0f)
//	{1.0f,  1.0f, 1.0f},	
//	{-1.0f,  1.0f, 1.0f},
//	{-1.0f, -1.0f, 1.0f},
//	{-1.0f, -1.0f,  1.0f},	// Left face (x = -1.0f)
//	{-1.0f, -1.0f, -1.0f},
//	{-1.0f,  1.0f, -1.0f},
//	{-1.0f,  1.0f,  1.0f},	
//	{1.0f, -1.0f, -1.0f},	// Right face (x = 1.0f)
//	{1.0f, -1.0f,  1.0f},
//	{1.0f,  1.0f,  1.0f},
//	{1.0f,  1.0f, -1.0f},
//};
const GLfloat cube[cube_vertex_count][cube_vertex_depth] =	// Enable this for drawing the cube as GL_TRIANGLES (must enable also drayArray(GL_TRIANGLES) in draw() function)
{
	{-0.5, 0.5, 0.5},	// Back face  (z = 0.5f)
	{0.5, 0.5, 0.5},
	{0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},

	{-0.5, 0.5, 0.5},	// Top face (y = 0.5f)
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{0.5, 0.5, -0.5},

	{0.5, -0.5, 0.5},	// Right face (x = 0.5f)
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},

	{-0.5, 0.5, -0.5},	// Front face (z = -0.5f)
	{0.5, 0.5, -0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},

	{-0.5, -0.5, 0.5},	// Bottom face (y = -0.5f)
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},

	{-0.5, -0.5, 0.5},	// Left face (x = -0.5f)
	{-0.5,  0.5, 0.5},
	{-0.5,  0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5}
};

const uint32_t cube_color_count = 36;
const uint32_t cube_color_depth = 4;
//const GLfloat cube_color[cube_color_count][cube_color_depth] =	// Enable this for drawing the cube as a GL_QUAD (must enable also drayArray(GL_QUAD) in draw() function)
//{
//	{0.0f, 1.0f, 0.0f, 1.0f},	// Green
//	{0.0f, 1.0f, 0.0f, 1.0f},
//	{0.0f, 1.0f, 0.0f, 1.0f},
//	{0.0f, 1.0f, 0.0f, 1.0f},
//	{1.0f, 0.5f, 0.0f, 1.0f},	// Orange
//	{1.0f, 0.5f, 0.0f, 1.0f},
//	{1.0f, 0.5f, 0.0f, 1.0f},
//	{1.0f, 0.5f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 0.0f, 1.0f},	// Red
//	{1.0f, 0.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 0.0f, 1.0f},
//	{1.0f, 0.0f, 0.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f, 1.0f},	// Yellow
//	{1.0f, 1.0f, 0.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f, 1.0f},
//	{1.0f, 1.0f, 0.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f, 1.0f},	// Blue
//	{0.0f, 0.0f, 1.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f, 1.0f},
//	{0.0f, 0.0f, 1.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f, 1.0f},	// Magenta
//	{1.0f, 0.0f, 1.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f, 1.0f},
//	{1.0f, 0.0f, 1.0f, 1.0f}
//};
const GLfloat cube_color[cube_color_count][cube_color_depth] =	// Enable this for drawing the cube as GL_TRIANGLES (must enable also drayArray(GL_TRIANGLES) in draw() function)
{
	{0.0, 0.0, 1.0, 1.0f},	// Blue
	{0.0, 0.0, 1.0, 1.0f},
	{0.0, 0.0, 1.0, 1.0f},
	{0.0, 0.0, 1.0, 1.0f},
	{0.0, 0.0, 1.0, 1.0f},
	{0.0, 0.0, 1.0, 1.0f},

	{1.0, 0.0, 0.0, 1.0f},	// Red
	{1.0, 0.0, 0.0, 1.0f},
	{1.0, 0.0, 0.0, 1.0f},
	{1.0, 0.0, 0.0, 1.0f},
	{1.0, 0.0, 0.0, 1.0f},
	{1.0, 0.0, 0.0, 1.0f},

	{0.0, 1.0, 0.0, 1.0f},	// Green
	{0.0, 1.0, 0.0, 1.0f},
	{0.0, 1.0, 0.0, 1.0f},
	{0.0, 1.0, 0.0, 1.0f},
	{0.0, 1.0, 0.0, 1.0f},
	{0.0, 1.0, 0.0, 1.0f},

	{0.0, 1.0, 1.0, 1.0f},	// Cyan
	{0.0, 1.0, 1.0, 1.0f},
	{0.0, 1.0, 1.0, 1.0f},
	{0.0, 1.0, 1.0, 1.0f},
	{0.0, 1.0, 1.0, 1.0f},
	{0.0, 1.0, 1.0, 1.0f},

	{1.0, 1.0, 0.0, 1.0f},	// Yellow
	{1.0, 1.0, 0.0, 1.0f},
	{1.0, 1.0, 0.0, 1.0f},
	{1.0, 1.0, 0.0, 1.0f},
	{1.0, 1.0, 0.0, 1.0f},
	{1.0, 1.0, 0.0, 1.0f},

	{1.0, 0.0, 1.0, 1.0f},	// Magenta
	{1.0, 0.0, 1.0, 1.0f},
	{1.0, 0.0, 1.0, 1.0f},
	{1.0, 0.0, 1.0, 1.0f},
	{1.0, 0.0, 1.0, 1.0f},
	{1.0, 0.0, 1.0, 1.0f}
};
const uint32_t cube_position_attr_index = 0, cube_color_attr_index = 1;

const uint32_t points = 4;
const uint32_t floatsPerPoint = 3;
const GLfloat square[points][floatsPerPoint] =
{
    { -0.5,  0.5,  0.5 }, // Top left
    {  0.5,  0.5,  0.5 }, // Top right
    {  0.5, -0.5,  0.5 }, // Bottom right 
    { -0.5, -0.5,  0.5 }, // Bottom left
};
const uint32_t colorPoints = 4;
const uint32_t colorfloatsPerPoint = 4;
const GLfloat square_color[colorPoints][colorfloatsPerPoint] =
{
    { 0.0, 1.0, 0.0, 1.0 },	 // Top left
	{ 1.0, 1.0, 0.0, 1.0 }, // Top right
	{ 1.0, 0.0, 0.0, 1.0 }, // Bottom right 
	{ 0.0, 0.0, 1.0, 1.0 }, // Bottom left
};
const uint32_t square_position_attr_index = 0, square_color_attr_index = 1;

const uint32_t points_2 = 12;
const GLfloat square_2[points_2][floatsPerPoint] =
{
    {  0.2,  0.2,  0.5 }, // Top right
    { -0.2,  0.2,  0.5 }, // Top left
    {  0.0,  0.0,  0.5 }, // Center

    {  0.2,  0.2,  0.5 }, // Top right
    {  0.2, -0.2,  0.5 }, // Bottom right 
    {  0.0,  0.0,  0.5 }, // Center

    { -0.2, -0.2,  0.5 }, // Bottom left
    {  0.2, -0.2,  0.5 }, // Bottom right 
    {  0.0,  0.0,  0.5 }, // Center

    { -0.2, -0.2,  0.5 }, // Bottom left
    { -0.2,  0.2,  0.5 }, // Top left
    {  0.0,  0.0,  0.5 }, // Center
};
const uint32_t colorPoints_2 = 12;
const GLfloat square_color_2[colorPoints_2][colorfloatsPerPoint] =
{
    { 0.5, 0.5, 0.5, 1.0f }, // Top right
    { 0.5, 0.5, 0.5, 1.0f }, // Bottom right 
    { 0.0, 0.0, 0.0, 1.0f }, // Center

    { 0.5, 0.5, 0.5, 1.0f }, // Top left
    { 0.5, 0.5, 0.5, 1.0f }, // Top right
    { 0.0, 0.0, 0.0, 1.0f }, // Center

    { 0.5, 0.5, 0.5, 1.0f }, // Bottom left
    { 0.5, 0.5, 0.5, 1.0f }, // Bottom right 
    { 0.0, 0.0, 0.0, 1.0f }, // Center

    { 0.5, 0.5, 0.5, 1.0f }, // Bottom left
    { 0.5, 0.5, 0.5, 1.0f }, // Top left
    { 0.0, 0.0, 0.0, 1.0f }, // Center
};

// Example using an only vector to specify both positions and colors
// Also it will use an Element Buffer to make a square (two triangles) with only four vertices, by reusing vertices
// This may not seem like much of a big deal at this point, but when our graphics application loads many models into the relatively
// small graphics memory, element buffers will be an important area of optimization. Repetition of vertex data is a waste of memory
const uint32_t vertices = 4;
const uint32_t floatsPerVertex = 7;
const GLfloat square_3[vertices][floatsPerVertex] =
{
	// Position			// Color	
	{-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},	// Top-left
	{0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},		// Top-right
	{0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},	// Bottom-right
	{-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f}	// Bottom-left
};

const GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

const uint32_t floatsPerVertexTex = 9;
const GLfloat square_tex[vertices][floatsPerVertexTex] =
{
	// Position		     // Color				 // Textcoords
    { -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },	// Top left
	{  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },	// Top right
    {  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },	// Bottom right 
    { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }	// Bottom left
};

const uint32_t cube_tex_vertex_count = 8;
const GLfloat cube_tex[cube_tex_vertex_count][floatsPerVertexTex] =	// Enable this for drawing the cube as GL_TRIANGLES (must enable also drayArray(GL_TRIANGLES) in draw() function)
{
	// Position		      // Color				  // Textcoords
	{ -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },		// 0. Top left front
	{  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },		// 1. Top right front
    {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },	// 2. Bottom right front
    { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 3. Bottom left front

	{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },		// 4. Top left back
	{  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },		// 5. Top right back
    {  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },		// 6. Bottom right back
    { -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }		// 7. Bottom left back
};

const GLuint cube_elements[] =	// This cube_elements index array works fine with cube_tex, but the rendering gets messed because we haven't all the texture coordinates we need -> cube_tex_2
{
    0, 1, 2,
    2, 3, 0,
	1, 5, 6,
	6, 2, 1,
	4, 5, 6,
	6, 7, 4,
	0, 4, 7,
	7, 3, 0,
	4, 5, 1,
	1, 0, 4,
	6, 7, 2,
	2, 3, 7
};

const GLfloat cube_tex_2[] =	// Enable this for drawing the cube as GL_TRIANGLES (must enable also drawArray(GL_TRIANGLES) in draw() function)
{
	// Position		     // Color				// Textcoords
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,		// Front face (z = -0.5f)
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,		// Back face (z = 0.5f)
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,		// Left face (x = -0.5f)
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,		// Right face (x = 0.5f)
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,		// Top face (y = 0.5f)
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, 1.0f,

	-1.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f, 0.0f,		// Planar base
     1.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0f, 0.0f,
     1.0f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0f, 1.0f,
     1.0f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0f, 1.0f,
    -1.0f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f, 1.0f,
    -1.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f, 0.0f
};

const GLfloat small_square_tex[vertices][floatsPerVertexTex] =	// This coords should be relative to the OpenGL texture system coordinates, (1,1) at bottom right and (-1,-1) at top left
																// But are defined just with trial and error because we have a little transformation at shader level that inverts and scales OpenGL system coordinate
{
	// Position		       // Color				   // Textcoords
    { 0.45f, -0.45f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },	// Top left
	{ 0.22f, -0.45f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f },	// Top right
    { 0.22f, -0.22f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },	// Bottom right 
    { 0.45f, -0.22f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }		// Bottom left
};

#endif