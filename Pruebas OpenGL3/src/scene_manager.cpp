#include "scene_manager.h"

#include <string>

#include <gtc/matrix_transform.hpp> // Needed for importing GLM Extensions (including matrix transformations, etc)
									// To solve the issue reggarding the error LNK2001: símbolo externo "public: __thiscall glm::tvec4<float,0>::tvec4<float,0>(struct glm::tvec4<float,0> const &)" (??0?$tvec4@M$0A@@glm@@QAE@ABU01@@Z) sin resolver
									// The solution is here (adding a little code to setup.hpp): https://github.com/g-truc/glm/issues/377

#define V_BLANK 1
#define RENDER_3D_TEX_4	// RENDER_2D		: Render a 2D
						// RENDER_2D_2		: Render a 2D
						// RENDER_2D_3		: Render a 2D
						// RENDER_2D_TEX	: Render a 2D object with a texture
						// RENDER_3D		: Render a 3D object
						// RENDER_3D_TEX	: Render a 3D object with a texture
						// RENDER_3D_TEX_2	: Render a 3D object with a texture, and use depth and stencil buffers to do some effects (reflection and object outlining)
						// RENDER_3D_TEX_3	: Render a 3D object with a texture, and use frame buffers and render buffers to do some post processing effects
						// RENDER_3D_TEX_4	: Render a 3D object with a texture, and use frame buffers and render buffers to show a little viewer inside the scene (same scene from different angle)

SceneManager* SceneManager::instance = NULL;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

SceneManager* SceneManager::getInstance() {

	if (instance == NULL)
		instance = new SceneManager();

	return instance;
}

int SceneManager::init() {

	app_end = cube_selected = false;
	w_width = 800; w_height = 600;
	w_pos_x = 300; w_pos_y = 300;

	time_1 = (float) SDL_GetTicks();

	// Calculate model-view-projection matrix

	model = glm::translate(glm::mat4(1.0f),glm::vec3(0.f,0.f,0.f));
	view = glm::lookAt(glm::vec3(0.f,1.f,-5.f),glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f));
	projection = glm::perspective(glm::radians(45.0f),(float)(w_width/w_height),0.1f,100.0f);

	// FIXME: for better performance this must be done in the GPU as part of the vertex shader (matrices should be passed as uniforms an filled by the CPU)
	model_view_projection = projection * view * model;

//	cout << "Matrix model-view-projection is " << endl;
//	cout << "Row 1: [ " << model_view_projection[0][0] << " " << model_view_projection[0][1] << " " << model_view_projection[0][2] << " " << model_view_projection[0][3] << " ]" << endl;
//	cout << "Row 2: [ " << model_view_projection[1][0] << " " << model_view_projection[1][1] << " " << model_view_projection[1][2] << " " << model_view_projection[1][3] << " ]" << endl;
//	cout << "Row 3: [ " << model_view_projection[2][0] << " " << model_view_projection[2][1] << " " << model_view_projection[2][2] << " " << model_view_projection[2][3] << " ]" << endl;
//	cout << "Row 4: [ " << model_view_projection[3][0] << " " << model_view_projection[3][1] << " " << model_view_projection[3][2] << " " << model_view_projection[3][3] << " ]" << endl;

#ifdef RENDER_2D
	shader = new Shader(".\\.\\src\\shader_2d.vert",".\\.\\src\\shader_2d.frag","");
#elif defined RENDER_2D_2
	shader = new Shader(".\\.\\src\\shader_2d_2.vert",".\\.\\src\\shader_2d_2.frag",".\\.\\src\\shader_2d_2.geom");
#elif defined RENDER_2D_3
	shader = new Shader(".\\.\\src\\shader_2d.vert",".\\.\\src\\shader_2d.frag","");
#elif defined RENDER_2D_TEX
	shader = new Shader(".\\.\\src\\shader_2d_tex.vert",".\\.\\src\\shader_2d_tex.frag","");
#elif defined RENDER_3D
	shader = new Shader(".\\.\\src\\shader.vert",".\\.\\src\\shader.frag","");
#elif defined RENDER_3D_TEX
	shader = new Shader(".\\.\\src\\shader_tex.vert",".\\.\\src\\shader_tex.frag","");
#elif defined RENDER_3D_TEX_2
	shader = new Shader(".\\.\\src\\shader_tex.vert",".\\.\\src\\shader_tex.frag","");
#elif defined RENDER_3D_TEX_3
	shader = new Shader(".\\.\\src\\shader_tex.vert",".\\.\\src\\shader_tex.frag","");
	screen_shader = new Shader(".\\.\\src\\shader_square_tex.vert",".\\.\\src\\shader_square_tex.frag","");
#elif defined RENDER_3D_TEX_4
	shader = new Shader(".\\.\\src\\shader_tex.vert",".\\.\\src\\shader_tex.frag","");
	screen_shader = new Shader(".\\.\\src\\shader_square_tex.vert",".\\.\\src\\shader_square_tex.frag","");
#endif

	//if ((initOpenVR() == -1) || (initSDL() == -1) || (initOpenGL() == -1))
	if ((initSDL() == -1) || (initOpenGL() == -1))
		return -1;

	return 0;
}

void SceneManager::update() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_KEYDOWN:
			{ 
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					cout << "Exiting app..." << endl;
					app_end = true;
				}
				else
					if (event.key.keysym.sym == SDLK_s) {
						(cube_selected = !cube_selected) ? cout << "Se selecciono el cubo" << endl : cout << "Se deselecciono el cubo" << endl;
					}
			}
			break;
		case SDL_QUIT: // In case the user presses the window's X button we malso must properly shutdown down the app
			{
				cout << "Exiting app..." << endl;
				app_end = true;
			}
			break;
		}
	}
}

void SceneManager::draw() {

	// Make our background black
	glClearColor(1.f,1.f,1.f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef RENDER_2D
	SDL_Delay(100);
	float time_2 = (float)SDL_GetTicks();
	shader->set_passed_color(glm::vec4((sin((time_2-time_1)*1.5f)+1.0f)/3.0f, 0.0f, 0.0f,1.0f)); // Change color based on some crazy math...

	glBindVertexArray(square_vao);

	// Invoke glDrawArrays telling that our data is a triangle fan and we want to draw 4 vertices
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#elif defined RENDER_2D_2
	glBindVertexArray(square_2_vao);

	// Invoke glDrawArrays telling that our data are triangles and we want to draw 12 vertices
	glDrawArrays(GL_TRIANGLES, 0, 12);
#elif defined RENDER_2D_3
	glBindVertexArray(square_3_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_3_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // The only real difference here is that we're talking about indices instead of vertices as in glDrawArrays
#elif defined RENDER_2D_TEX
	glBindVertexArray(square_texture_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
#elif defined RENDER_3D
	glBindVertexArray(cube_vao);
	
	// Invoke glDrawArrays telling that our data are quads and we want to draw 24 vertices
	// The first parameter specifies the kind of primitive (commonly point, line or triangle), the second parameter specifies how many
	// vertices to skip at the beginning and the last parameter specifies the number of vertices (not primitives!) to process

	//glDrawArrays(GL_QUADS, 0, cube_vertex_count);
	glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);
#elif defined RENDER_3D_TEX
	float time_2 = (float)SDL_GetTicks();
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(50.0f),glm::vec3(0.0f, 1.0f, 0.0f));
	time_1 = time_2;
	shader->set_model_matrix(model);
		
	glBindVertexArray(cube_texture_vao);
	glDrawElements(GL_TRIANGLES, sizeof(cube_elements), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#elif defined RENDER_3D_TEX_2
	float time_2 = (float)SDL_GetTicks();
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(50.0f),glm::vec3(0.0f, 1.0f, 0.0f));	// Rotate along Y
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(20.0f),glm::vec3(1.0f, 0.0f, 0.0f));	// Rotate along X
	time_1 = time_2;

	shader->set_model_matrix(model);

	glBindVertexArray(cube_texture_2_vao);

	// Draw top cube
	if (!cube_selected) glDrawArrays(GL_TRIANGLES, 0, 30);

	glEnable(GL_STENCIL_TEST);

		if (cube_selected)
		{
			// Draw the top cube and write it to stencil buffer
			glStencilFunc(GL_ALWAYS, 2, 0xF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			glDrawArrays(GL_TRIANGLES, 0, 30);

			// Draw outline (algorithm from http://www.flipcode.com/archives/Object_Outlining.shtml)
			glLineWidth(10);
			glStencilFunc(GL_NOTEQUAL, 2, 0xF); // Pass test if stencil value is 1
			glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
			glDepthMask(GL_TRUE); // Write to depth buffer
			glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 30);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		}

		// Draw floor and write it to stencil
		glStencilFunc(GL_ALWAYS, 1, 0xF); // Set any stencil to 1 (mask is 4 bits because stencil buffer was created as 4-bit buffer with SDL_GL_SetAttribute)
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glDrawArrays(GL_TRIANGLES, 30, 6);
			
		// Draw bottom cube (reflection)
		glStencilFunc(GL_EQUAL, 1, 0xF); // Pass test if stencil value is 1
		glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
		glDepthMask(GL_TRUE); // Write to depth buffer
		shader->set_model_matrix(glm::translate(model,glm::vec3(0.0f,-1.0f,0.0f)));
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 0.3f, 0.3f, 0.3f);
		glDrawArrays(GL_TRIANGLES, 0, 30);
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

	glDisable(GL_STENCIL_TEST);

	glBindVertexArray(0);
#elif defined RENDER_3D_TEX_3
	
	float time_2 = (float)SDL_GetTicks();
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(50.0f),glm::vec3(0.0f, 1.0f, 0.0f));	// Rotate along Y
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(20.0f),glm::vec3(1.0f, 0.0f, 0.0f));	// Rotate along X
	time_1 = time_2;
	
	glBindFramebuffer(GL_FRAMEBUFFER, cube_frame_buffer);
	glBindVertexArray(cube_texture_2_vao);
    glEnable(GL_DEPTH_TEST);

	// Clear the screen to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use_program();
	shader->set_model_matrix(model);
	glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture_2);
	
	// Draw top cube to frame buffer with the same code as in RENDER_3D_TEX_2
	if (!cube_selected) glDrawArrays(GL_TRIANGLES, 0, 30);

	glEnable(GL_STENCIL_TEST);

		if (cube_selected)
		{
			// Draw the top cube and write it to stencil buffer
			glStencilFunc(GL_ALWAYS, 2, 0xF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			glDrawArrays(GL_TRIANGLES, 0, 30);

			// Draw outline (algorithm from http://www.flipcode.com/archives/Object_Outlining.shtml)
			glLineWidth(10);
			glStencilFunc(GL_NOTEQUAL, 2, 0xF); // Pass test if stencil value is not equal to 2
			glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
			glDepthMask(GL_TRUE); // Write to depth buffer
			glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE); // Disable write color buffer for all colors (only red channel)
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 30);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); // Enable write color buffer for all colors
		}

		// Draw floor and write it to stencil
		glStencilFunc(GL_ALWAYS, 1, 0xF); // Set any stencil to 1 (mask is 4 bits because stencil buffer was created as 4-bit buffer with SDL_GL_SetAttribute)
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glDrawArrays(GL_TRIANGLES, 30, 6);
			
		// Draw bottom cube (reflection)
		glStencilFunc(GL_EQUAL, 1, 0xF); // Pass test if stencil value is 1
		glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
		glDepthMask(GL_TRUE); // Write to depth buffer
		shader->set_model_matrix(glm::translate(model,glm::vec3(0.0f,-1.0f,0.0f)));
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 0.3f, 0.3f, 0.3f);
		glDrawArrays(GL_TRIANGLES, 0, 30);
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

	glDisable(GL_STENCIL_TEST);

	screen_shader->use_program();

	// Bind default framebuffer and draw contents of our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(square_texture_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_texture_ebo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_color_buffer); // Do mind that calls like glBindTexture which change the OpenGL state are relatively expensive, so we must try to keeping them to a minimum
	glUniform1i(glGetUniformLocation(screen_shader->get_shader(),"frame_buffer_tex"),0);

    glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
#elif defined RENDER_3D_TEX_4
	
	float time_2 = (float)SDL_GetTicks();
	model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(50.0f),glm::vec3(0.0f, 1.0f, 0.0f));	// Rotate along Y
	//model *= glm::rotate(glm::mat4(1.f),((time_2-time_1)/1000)*glm::radians(20.0f),glm::vec3(1.0f, 0.0f, 0.0f));	// Rotate along X
	time_1 = time_2;
	
	glBindFramebuffer(GL_FRAMEBUFFER, cube_frame_buffer);
	glBindVertexArray(cube_texture_2_vao);
    glEnable(GL_DEPTH_TEST);

	// Clear the screen to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use_program();
	shader->set_model_matrix(model);
	shader->set_view_matrix(view);
	glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture_2);
	
	// Draw top cube to frame buffer with the same code as in RENDER_3D_TEX_2
	if (!cube_selected) glDrawArrays(GL_TRIANGLES, 0, 30);

	glEnable(GL_STENCIL_TEST);

		if (cube_selected)
		{
			// Draw the top cube and write it to stencil buffer
			glStencilFunc(GL_ALWAYS, 2, 0xF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			glDrawArrays(GL_TRIANGLES, 0, 30);

			// Draw outline (algorithm from http://www.flipcode.com/archives/Object_Outlining.shtml)
			glLineWidth(10);
			glStencilFunc(GL_NOTEQUAL, 2, 0xF); // Pass test if stencil value is not equal to 2
			glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
			glDepthMask(GL_TRUE); // Write to depth buffer
			glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE); // Disable write color buffer for all colors (only red channel)
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 30);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); // Enable write color buffer for all colors
		}

		// Draw floor and write it to stencil
		glStencilFunc(GL_ALWAYS, 1, 0xF); // Set any stencil to 1 (mask is 4 bits because stencil buffer was created as 4-bit buffer with SDL_GL_SetAttribute)
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xF); // Mask is all ones in order to write to stencil buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glDrawArrays(GL_TRIANGLES, 30, 6);
			
		// Draw bottom cube (reflection)
		glStencilFunc(GL_EQUAL, 1, 0xF); // Pass test if stencil value is 1
		glStencilMask(0x0); // Mask is all zeros in order to do not write anything to stencil buffer
		glDepthMask(GL_TRUE); // Write to depth buffer
		shader->set_model_matrix(glm::translate(model,glm::vec3(0.0f,-1.0f,0.0f)));
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 0.3f, 0.3f, 0.3f);
		glDrawArrays(GL_TRIANGLES, 0, 30);
		glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

	glDisable(GL_STENCIL_TEST);

	screen_shader->use_program();

	// Bind default framebuffer and draw contents of our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(square_texture_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_texture_ebo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_color_buffer); // Do mind that calls like glBindTexture which change the OpenGL state are relatively expensive, so we must try to keeping them to a minimum
	glUniform1i(glGetUniformLocation(screen_shader->get_shader(),"frame_buffer_tex"),0);

    glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);


	// Draw little screen showing only the cube (no reflection) from a top camera
	// FIXME: This should have been rendered to the cube_frame_buffer we used before
	//		  Even using the cube_frame_buffer was unnecesary for rendering the main scene (it could have been rendered directly to default color buffer)

	glBindFramebuffer(GL_FRAMEBUFFER, small_cube_frame_buffer);
	glBindVertexArray(cube_texture_2_vao);
    glEnable(GL_DEPTH_TEST);

	// Clear the screen to soft green
    glClearColor(218.f/255.f,1.0f,202.f/255.f,1.0f); // Color from http://www.rapidtables.com/web/color/RGB_Color.htm
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use_program();
	shader->set_model_matrix(model);
	glm::mat4 tmp_view = glm::lookAt(glm::vec3(0.f,3.f,0.f),glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,0.f,1.f)); // View from above
	shader->set_view_matrix(tmp_view);
	glUniform3f(glGetUniformLocation(shader->get_shader(),"override_Color"), 1.0f, 1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture_2);

	// Draw top cube to the new frame buffer
	glDrawArrays(GL_TRIANGLES, 0, 30);

	// Draw floor to the new frame buffer
	glDrawArrays(GL_TRIANGLES, 30, 6);

	screen_shader->use_program();

	// Bind default framebuffer and draw contents of our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(small_square_texture_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,small_square_texture_ebo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, small_tex_color_buffer); // Do mind that calls like glBindTexture which change the OpenGL state are relatively expensive, so we must try to keeping them to a minimum
	glUniform1i(glGetUniformLocation(screen_shader->get_shader(),"frame_buffer_tex"),0);

    glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
#endif

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(companion_window);
}

void SceneManager::exit() {

	// Delete our OpengL context
	SDL_GL_DeleteContext(gl_context);
	// Destroy our window
	SDL_DestroyWindow(companion_window);
	companion_window = NULL;
	// Shutdown SDL2
	SDL_Quit();

	shader->clean_up();

	glDeleteTextures(1, square_texture);
	glDeleteTextures(1, &cube_texture);
	glDeleteTextures(1, &cube_texture_2);
	glDisableVertexAttribArray(0);
	glDeleteBuffers(2,cube_vbo);
	glDeleteVertexArrays(1,&cube_vao);
    glDeleteBuffers(1, square_vbo);
    glDeleteVertexArrays(1,&square_vao);
	glDeleteBuffers(1,&square_3_vbo);
	glDeleteBuffers(1,&square_3_ebo);
	glDeleteVertexArrays(1,&square_3_vao);
	glDeleteBuffers(1,&square_texture_vbo);
	glDeleteBuffers(1,&square_texture_ebo);
	glDeleteVertexArrays(1,&square_texture_vao);
	glDeleteBuffers(1,&cube_texture_vbo);
	glDeleteBuffers(1,&cube_texture_ebo);
	glDeleteVertexArrays(1,&cube_texture_vao);
	glDeleteBuffers(1,&cube_texture_2_vbo);
	glDeleteVertexArrays(1,&cube_texture_2_vao);
	glDeleteFramebuffers(1, &cube_frame_buffer);
	glDeleteRenderbuffers(1, &render_buffer_depth_stencil);
}

int SceneManager::initSDL() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Enable to tell SDL that the old, deprecated GL code are disabled, only the newer versions can be used
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	companion_window = SDL_CreateWindow("Hello world VR", w_pos_x, w_pos_y, w_width, w_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (companion_window == NULL) {
		SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	gl_context = SDL_GL_CreateContext(companion_window);
	if (gl_context == NULL) {
		SDL_Log("Unable to create GL Context: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	if (SDL_GL_SetSwapInterval(V_BLANK) < 0) // Configure VSync
	{
		SDL_Log("Warning: Unable to set VSync!: %s", SDL_GetError());
		return -1;
	}

	glewExperimental = GL_TRUE; // Enable to tell OpenGL that we want to use OpenGL 3.0 stuff and later
	GLenum nGlewError = glewInit(); // Initialize glew
	if (nGlewError != GLEW_OK)
	{
		SDL_Log( "Error initializing GLEW! %s", glewGetErrorString( nGlewError ) );
		return -1;
	}

	string strWindowTitle = "OpenGL3 Test";
	SDL_SetWindowTitle(companion_window, strWindowTitle.c_str());

	return 0;
}

int SceneManager::initOpenGL() {

	int success = 0;
    GLenum error = GL_NO_ERROR;

	// Initialize clear color
    glClearColor(0.f,0.f,0.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if((error = glGetError()) != GL_NO_ERROR)
    {
        cout << "Error initializing OpenGL!" << gluErrorString(error) << endl;
		return -1;
    }

	glViewport(0, 0, w_width, w_height);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth testing so that closer triangles will hide the triangles farther away
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	cout << "Setting up VBO+VAO..." << endl;
#ifdef RENDER_2D
	success = setup_buffers_2D() ? 0 : -1;
#elif defined RENDER_2D_2
	success = setup_buffers_2D_2() ? 0 : -1;
#elif defined RENDER_2D_3
	success = setup_buffers_2D_3() ? 0 : -1;
#elif defined RENDER_2D_TEX
	success = setup_buffers_2D_tex() ? 0 : -1;
#elif defined RENDER_3D
	success = setup_buffers() ? 0 : -1;
#elif defined RENDER_3D_TEX
	success = setup_buffers_3D_tex() ? 0 : -1;
#elif defined RENDER_3D_TEX_2
	success = setup_buffers_3D_tex_2() ? 0 : -1;
#elif defined RENDER_3D_TEX_3
	success = setup_buffers_3D_tex_3() ? 0 : -1;
#elif defined RENDER_3D_TEX_4
	success = setup_buffers_3D_tex_4() ? 0 : -1;
#endif
	cout << "VBO+VAO setup completed" << endl;

	return success;
}

bool SceneManager::setup_buffers() {

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(2,cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,cube_vertex_count*cube_vertex_depth*sizeof(GLfloat),cube,GL_STATIC_DRAW);

	// Create and bind a vertex buffer for cube vertex colors and copy the data from the cube vertex colors
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,cube_color_count*cube_color_depth*sizeof(GLfloat),cube_color,GL_STATIC_DRAW);

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&cube_vao);
	glBindVertexArray(cube_vao);

	// Bind the corresponding vertex buffers (VBO) and copy the data to the vertex attribute array (VAO), and enable each VAO
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbo[0]);
	glVertexAttribPointer(cube_position_attr_index,3,GL_FLOAT,GL_FALSE,0,0);	// The fourth param specifies the stride, or how many bytes are between each position attribute in the array. The value 0 means that there
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbo[1]);									// is no data in between. This is currently the case as the position of each vertex is immediately followed by the position of the next vertex.  
	glVertexAttribPointer(cube_color_attr_index,4,GL_FLOAT,GL_FALSE,0,0);		// The last parameter specifies the offset, or how many bytes from the start of the array the attribute occurs. Since there are no other attributes, this is 0 as well

	glEnableVertexAttribArray(cube_position_attr_index);
	glEnableVertexAttribArray(cube_color_attr_index);
	
	cout << "Initializing shaders..." << endl;

	if (shader->init())
	{
		shader->use_program();
		shader->set_mvp_matrix(model_view_projection);
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_2D() {

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(2,square_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,points*floatsPerPoint*sizeof(GLfloat),square,GL_STATIC_DRAW);

	// Create and bind a vertex buffer for cube vertex colors and copy the data from the cube vertex colors
	glBindBuffer(GL_ARRAY_BUFFER,square_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,colorPoints*colorfloatsPerPoint*sizeof(GLfloat),square_color,GL_STATIC_DRAW);

	// Create and bind a verte array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&square_vao);
	glBindVertexArray(square_vao);

	// Bind the corresponding vertex buffers (VBO) and copy the data to the vertex attribute array (VAO), and enable each VAO
	glBindBuffer(GL_ARRAY_BUFFER,square_vbo[0]);
	glVertexAttribPointer(square_position_attr_index,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(square_position_attr_index);
	glBindBuffer(GL_ARRAY_BUFFER,square_vbo[1]);
	glVertexAttribPointer(square_color_attr_index,4,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(square_color_attr_index);
	
	cout << "Initializing shaders..." << endl;

	if (shader->init())
	{
		shader->use_program();
		shader->set_passed_color(glm::vec4(1.0f,0.0f,0.0f,1.0f));
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_2D_2() {

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(2,square_2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_2_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,points_2*floatsPerPoint*sizeof(GLfloat),square_2,GL_STATIC_DRAW);

	// Create and bind a vertex buffer for cube vertex colors and copy the data from the cube vertex colors
	glBindBuffer(GL_ARRAY_BUFFER,square_2_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,colorPoints_2*colorfloatsPerPoint*sizeof(GLfloat),square_color_2,GL_STATIC_DRAW);

	// Create and bind a verte array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&square_2_vao);
	glBindVertexArray(square_2_vao);

	// Bind the corresponding vertex buffers (VBO) and copy the data to the vertex attribute array (VAO), and enable each VAO
	glBindBuffer(GL_ARRAY_BUFFER,square_2_vbo[0]);
	glVertexAttribPointer(square_position_attr_index,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(square_position_attr_index);
	glBindBuffer(GL_ARRAY_BUFFER,square_2_vbo[1]);
	glVertexAttribPointer(square_color_attr_index,4,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(square_color_attr_index);
	
	cout << "Initializing shaders..." << endl;

	if (shader->init())
		shader->use_program();
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_2D_3() {

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&square_3_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_3_vbo);
	glBufferData(GL_ARRAY_BUFFER,vertices*floatsPerVertex*sizeof(GLfloat),square_3,GL_STATIC_DRAW);

	// Create and bind an element buffer with the indexes in the VBO
	glGenBuffers(1,&square_3_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_3_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&square_3_vao);
	glBindVertexArray(square_3_vao);

	cout << "Initializing shaders..." << endl;

	if (shader->init())
	{
		shader->use_program();
		shader->set_passed_color(glm::vec4(1.0f,0.0f,0.0f,1.0f));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_3_ebo);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertex*sizeof(GLfloat), 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertex*sizeof(GLfloat), (void*)(3*sizeof(GLfloat))); // The fifth parameter is set to floatsPerVertex*sizeof(float) now, because each vertex consists of 'floatsPerVertex'
																															  // floating point attribute values. The offset of 3*sizeof(float) for the color attribute is there
																															  // because each vertex starts with 3 floating point values for the position that it has to skip over
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_2D_tex() {

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&square_texture_vao);
	glBindVertexArray(square_texture_vao);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&square_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER,vertices*floatsPerVertexTex*sizeof(GLfloat),square_tex,GL_STATIC_DRAW);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&square_texture_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_texture_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	// Create a texture buffer
	glGenTextures(2,square_texture);

	// TEXTURE 1...

	glBindTexture(GL_TEXTURE_2D,square_texture[0]); // The pixels in the texture will be addressed using texture coordinates during drawing operations.
													// These coordinates range from 0.0 to 1.0 where (0,0) is conventionally the bottom-left corner and (1,1) is the top-right corner of the texture image

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height;
	unsigned char* image = SOIL_load_image(".\\.\\portal_cube.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// SOIL_save_image(".\\.\\portal_cube_copy.bmp",SOIL_SAVE_TYPE_BMP,width,height,3,image);

	// Mipmaps are smaller copies of your texture that have been sized down and filtered in advance. It is recommended that you use them because they result in both a higher quality and higher performance
	// Generating them is as simple as calling the following function, so there's no excuse for not using them
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	// TEXTURE 2...

	glBindTexture(GL_TEXTURE_2D,square_texture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	// FIXME: in order to load it as a decal I think we have to load it as GL_RGBA
	image = SOIL_load_image(".\\.\\portal_cube_decal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// SOIL_save_image(".\\.\\portal_cube_copy.bmp",SOIL_SAVE_TYPE_BMP,width,height,3,image);

	// Mipmaps are smaller copies of your texture that have been sized down and filtered in advance. It is recommended that you use them because they result in both a higher quality and higher performance
	// Generating them is as simple as calling the following function, so there's no excuse for not using them
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image); // Clean up the image data right after loading it into the texture

	cout << "Initializing shaders..." << endl;
	
	if (shader->init())
	{
		shader->use_program();

		glBindVertexArray(square_texture_vao);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, square_texture[0]);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex"),0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, square_texture[1]);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex_decal"),1);

		glBindVertexArray(0); // Unbind VAO
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_3D_tex() {

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&cube_texture_vao);
	glBindVertexArray(cube_texture_vao);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&cube_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,cube_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER,cube_tex_vertex_count*floatsPerVertexTex*sizeof(GLfloat),cube_tex,GL_STATIC_DRAW);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&cube_texture_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_texture_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cube_elements),cube_elements,GL_STATIC_DRAW);

	// Create a texture buffer
	glGenTextures(1,&cube_texture);

	// TEXTURE 1...

	glBindTexture(GL_TEXTURE_2D,cube_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height;
	unsigned char* image = SOIL_load_image(".\\.\\portal_cube.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// SOIL_save_image(".\\.\\portal_cube_copy.bmp",SOIL_SAVE_TYPE_BMP,width,height,3,image);

	// Mipmaps are smaller copies of your texture that have been sized down and filtered in advance. It is recommended that you use them because they result in both a higher quality and higher performance
	// Generating them is as simple as calling the following function, so there's no excuse for not using them
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	cout << "Initializing shaders..." << endl;
	
	if (shader->init())
	{
		shader->use_program();
		shader->set_model_matrix(model);
		shader->set_view_matrix(view);
		shader->set_projection_matrix(projection);

		glBindVertexArray(cube_texture_vao);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex"),0);

		glBindVertexArray(0); // Unbind VAO
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_3D_tex_2() {

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&cube_texture_2_vao);
	glBindVertexArray(cube_texture_2_vao);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&cube_texture_2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,cube_texture_2_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube_tex_2),cube_tex_2,GL_STATIC_DRAW);

	// Create a texture buffer
	glGenTextures(1,&cube_texture_2);

	// TEXTURE 1...

	glBindTexture(GL_TEXTURE_2D,cube_texture_2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height;
	unsigned char* image = SOIL_load_image(".\\.\\portal_cube.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// SOIL_save_image(".\\.\\portal_cube_copy.bmp",SOIL_SAVE_TYPE_BMP,width,height,3,image);

	// Mipmaps are smaller copies of your texture that have been sized down and filtered in advance. It is recommended that you use them because they result in both a higher quality and higher performance
	// Generating them is as simple as calling the following function, so there's no excuse for not using them
	//glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	cout << "Initializing shaders..." << endl;
	
	if (shader->init())
	{
		shader->use_program();
		shader->set_model_matrix(model);
		shader->set_view_matrix(view);
		shader->set_projection_matrix(projection);

		glBindVertexArray(cube_texture_2_vao);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture_2);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex"),0);

		glBindVertexArray(0); // Unbind VAO
	}
	else
		return false;

	cout << "Shaders successfully initialized" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool SceneManager::setup_buffers_3D_tex_3() {

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&cube_texture_2_vao);
	glBindVertexArray(cube_texture_2_vao);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&cube_texture_2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,cube_texture_2_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube_tex_2),cube_tex_2,GL_STATIC_DRAW);

	// Create a texture buffer
	glGenTextures(1,&cube_texture_2);

	// TEXTURE 1...

	glBindTexture(GL_TEXTURE_2D,cube_texture_2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height;
	unsigned char* image = SOIL_load_image(".\\.\\portal_cube.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	
	// Create and bind a vertex array for storing both square attributes (positions, colors and textures)
	// In this square we are going to map the texture from the frame buffer containing the scene
	glGenVertexArrays(1,&square_texture_vao);
	glBindVertexArray(square_texture_vao);

	// Create and bind a vertex buffer for square vertex positions and copy the data from the square vertices
	glGenBuffers(1,&square_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER,vertices*floatsPerVertexTex*sizeof(GLfloat),square_tex,GL_STATIC_DRAW);

	// Create and bind an element buffer for square vertex indexes and copy the data from the square vertices
	glGenBuffers(1,&square_texture_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_texture_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	cout << "Initializing shaders..." << endl;
	
	if (shader->init())
	{
		shader->use_program();
		shader->set_model_matrix(model);
		shader->set_view_matrix(view);
		shader->set_projection_matrix(projection);

		glBindVertexArray(cube_texture_2_vao);
		glBindBuffer(GL_ARRAY_BUFFER, cube_texture_2_vbo);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,cube_texture_2);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex"),0);

		cout << "Scene shaders successfully initialized" << endl;
	}
	else
		return false;

	if (screen_shader->init())
	{
		screen_shader->use_program();

		glBindVertexArray(square_texture_vao);
		glBindBuffer(GL_ARRAY_BUFFER, square_texture_vbo);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		cout << "Screen shaders successfully initialized" << endl;
	}
	else
		return false;

	// Create and bind the frame buffer
	glGenFramebuffers(1, &cube_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, cube_frame_buffer);

	// Create texture where we are going to bulk the contents of the frame buffer
	glGenTextures(1, &tex_color_buffer);
	glBindTexture(GL_TEXTURE_2D, tex_color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w_width, w_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach the image to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_buffer, 0); // The second parameter implies that you can have multiple color attachments. A fragment shader can output
																									  // different data to any of these by linking 'out' variables to attachments with the glBindFragDataLocation function

	// Create the render buffer to host the depth and stencil buffers
	// Although we could do this by creating another texture, it is more efficient to store these buffers in a Renderbuffer Object, because we're only interested in reading the color buffer in a shader
	glGenRenderbuffers(1, &render_buffer_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w_width, w_height);

	// Attach the render buffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_depth_stencil);

	// Check whether the frame buffer is complete (at least one buffer attached, all attachmentes are complete, all attachments same number multisamples)
	(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) ?	cout << "The frame buffer is complete!" << endl : cout << "The frame buffer is invalid, please re-check. Code: " << glCheckFramebufferStatus(cube_frame_buffer) << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
	glBindVertexArray(0); // Unbind VAO

	return true;
}

bool SceneManager::setup_buffers_3D_tex_4() {

	// Create and bind a vertex array for storing both cube attributes (positions and colors)
	glGenVertexArrays(1,&cube_texture_2_vao);
	glBindVertexArray(cube_texture_2_vao);

	// Create and bind a vertex buffer for cube vertex positions and copy the data from the cube vertices
	glGenBuffers(1,&cube_texture_2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,cube_texture_2_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube_tex_2),cube_tex_2,GL_STATIC_DRAW);

	// Create a texture buffer
	glGenTextures(1,&cube_texture_2);

	// TEXTURE 1...

	glBindTexture(GL_TEXTURE_2D,cube_texture_2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	int width, height;
	unsigned char* image = SOIL_load_image(".\\.\\portal_cube.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	
	// Create and bind a vertex array for storing square attributes (positions, colors and textures)
	// In this square we are going to map the texture from the frame buffer containing the scene
	glGenVertexArrays(1,&square_texture_vao);
	glBindVertexArray(square_texture_vao);

	// Create and bind a vertex buffer for square vertex positions and copy the data from the square vertices
	glGenBuffers(1,&square_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,square_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER,vertices*floatsPerVertexTex*sizeof(GLfloat),square_tex,GL_STATIC_DRAW);

	// Create and bind an element buffer for square vertex indexes and copy the data from the square vertices
	glGenBuffers(1,&square_texture_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_texture_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	// Create and bind a vertex array for storing square attributes (positions, colors and textures)
	// In this square we are going to map the texture from the frame buffer containing the modified view of the scene
	glGenVertexArrays(1,&small_square_texture_vao);
	glBindVertexArray(small_square_texture_vao);

	// Create and bind a vertex buffer for square vertex positions and copy the data from the square vertices
	glGenBuffers(1,&small_square_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,small_square_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER,vertices*floatsPerVertexTex*sizeof(GLfloat),small_square_tex,GL_STATIC_DRAW);

	// Create and bind an element buffer for square vertex indexes and copy the data from the square vertices
	glGenBuffers(1,&small_square_texture_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,small_square_texture_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	cout << "Initializing shaders..." << endl;
	
	if (shader->init())
	{
		shader->use_program();
		shader->set_model_matrix(model);
		shader->set_view_matrix(view);
		shader->set_projection_matrix(projection);

		glBindVertexArray(cube_texture_2_vao);
		glBindBuffer(GL_ARRAY_BUFFER, cube_texture_2_vbo);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,cube_texture_2);
		glUniform1i(glGetUniformLocation(shader->get_shader(),"tex"),0);

		cout << "Scene shaders successfully initialized" << endl;
	}
	else
		return false;

	if (screen_shader->init())
	{
		screen_shader->use_program();

		glBindVertexArray(square_texture_vao);
		glBindBuffer(GL_ARRAY_BUFFER, square_texture_vbo);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		GLint colAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		GLint texAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		glBindVertexArray(small_square_texture_vao);
		glBindBuffer(GL_ARRAY_BUFFER, small_square_texture_vbo);

		// Specify the layout of the vertex data
		posAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*) 0);

		colAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

		texAttrib = glGetAttribLocation(screen_shader->get_shader(),"in_Texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, floatsPerVertexTex*sizeof(GLfloat), (GLvoid*)(7*sizeof(GLfloat)));

		cout << "Screen shaders successfully initialized" << endl;
	}
	else
		return false;

	// Create and bind the frame buffer
	glGenFramebuffers(1, &cube_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, cube_frame_buffer);

	// Create texture where we are going to bulk the contents of the frame buffer
	glGenTextures(1, &tex_color_buffer);
	glBindTexture(GL_TEXTURE_2D, tex_color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w_width, w_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach the image to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_buffer, 0); // The second parameter implies that you can have multiple color attachments. A fragment shader can output
																									  // different data to any of these by linking 'out' variables to attachments with the glBindFragDataLocation function

	// Create the render buffer to host the depth and stencil buffers
	// Although we could do this by creating another texture, it is more efficient to store these buffers in a Renderbuffer Object, because we're only interested in reading the color buffer in a shader
	glGenRenderbuffers(1, &render_buffer_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w_width, w_height);

	// Attach the render buffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_depth_stencil);

	// Check whether the frame buffer is complete (at least one buffer attached, all attachmentes are complete, all attachments same number multisamples)
	(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) ?	cout << "The frame buffer is complete!" << endl : cout << "The frame buffer is invalid, please re-check. Code: " << glCheckFramebufferStatus(cube_frame_buffer) << endl;


	// Create and bind the frame buffer
	glGenFramebuffers(1, &small_cube_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, small_cube_frame_buffer);

	// Create texture where we are going to bulk the contents of the frame buffer
	glGenTextures(1, &small_tex_color_buffer);
	glBindTexture(GL_TEXTURE_2D, small_tex_color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w_width, w_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach the image to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, small_tex_color_buffer, 0);

	// Create the render buffer to host the depth and stencil buffers
	// Although we could do this by creating another texture, it is more efficient to store these buffers in a Renderbuffer Object, because we're only interested in reading the color buffer in a shader
	glGenRenderbuffers(1, &small_render_buffer_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, small_render_buffer_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w_width, w_height);

	// Attach the render buffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, small_render_buffer_depth_stencil);

	// Check whether the frame buffer is complete (at least one buffer attached, all attachmentes are complete, all attachments same number multisamples)
	(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) ?	cout << "The frame buffer is complete!" << endl : cout << "The frame buffer is invalid, please re-check. Code: " << glCheckFramebufferStatus(cube_frame_buffer) << endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
	glBindVertexArray(0); // Unbind VAO

	return true;
}