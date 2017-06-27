#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#define GLEW_STATIC

#include "shader.h"
#include "models.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SOIL.h>

#include <glm.hpp>

using namespace std;

class SceneManager {
public:
	~SceneManager();

	static SceneManager* getInstance();

	int getFin() { return app_end; }

	int init();
	void update();
	void draw();
	void exit();

private:
	SceneManager();

	static SceneManager* instance;

	Shader* shader;
	Shader* screen_shader;

	SDL_Window* companion_window;
	SDL_GLContext gl_context;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model_view_projection;

	bool app_end, cube_selected;
	int w_width, w_height, w_pos_x, w_pos_y;
	float time_1;

	GLuint square_vbo[2];
	GLuint square_vao;

	GLuint square_2_vbo[2];
	GLuint square_2_vao;

	GLuint square_3_vbo;
	GLuint square_3_vao;
	GLuint square_3_ebo;

	GLuint square_texture[2];
	GLuint square_texture_vbo;
	GLuint square_texture_vao;
	GLuint square_texture_ebo;

	GLuint small_square_texture[2];
	GLuint small_square_texture_vbo;
	GLuint small_square_texture_vao;
	GLuint small_square_texture_ebo;

	GLuint cube_vbo[2];
	GLuint cube_vao;

	GLuint cube_texture;
	GLuint cube_texture_vbo;
	GLuint cube_texture_vao;
	GLuint cube_texture_ebo;

	GLuint cube_texture_2;
	GLuint cube_texture_2_vbo;
	GLuint cube_texture_2_vao;

	GLuint cube_frame_buffer;
	GLuint tex_color_buffer;
	GLuint render_buffer_depth_stencil;

	GLuint small_cube_frame_buffer;
	GLuint small_tex_color_buffer;
	GLuint small_render_buffer_depth_stencil;

	int initSDL();
	int initOpenGL();
	
	bool setup_buffers();
	bool setup_buffers_2D();
	bool setup_buffers_2D_2();
	bool setup_buffers_2D_3();
	bool setup_buffers_2D_tex();
	bool setup_buffers_3D_tex();
	bool setup_buffers_3D_tex_2();
	bool setup_buffers_3D_tex_3();
	bool setup_buffers_3D_tex_4();
};

#endif