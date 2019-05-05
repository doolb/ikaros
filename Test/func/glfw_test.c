#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "test.h"
#include "Mat.h"
#include "graphic/shader.h"
#include "graphic/texture.h"

#define GLSL_VERSION "#version 330 core\n"
GLenum err;

void processInput (GLFWwindow *window) {
	if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose (window, true);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport (0, 0, width, height);
}

// opengl debug message callback
// mini version 4.3
void GLAPIENTRY
MessageCallback (GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf (stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

uint vert_buff;
uint varr_buff;
uint vidx_buff;
#define VERTEX_SIZE (3 * sizeof (float))
#define COLOR_SIZE (3 * sizeof (float))
#define COL_OFFSET (VERTEX_SIZE)
#define UV_SIZE (2 * sizeof (float))
#define UV_OFFSET  (VERTEX_SIZE + COLOR_SIZE)
#define STRIP_SIZE (VERTEX_SIZE + COLOR_SIZE + UV_SIZE)
vec3 vert_data[] = {
	// position			// color			// uv
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// right-top
	0.5f, -0.5f, 0.0f,  .25f, .75f, 0.0f,	1.0f, 0.0f,		// right-bottom
	-0.5f, -0.5f, 0.0f, 0.0f, .25f, .75f,	0.0f, 0.0f,		// left-bottom
	-0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		// left-top
};
uint vidx_data[] = {
	0, 1, 3, // rigth-top
	1, 2, 3  // left-bottom
};

pTexture tex;

void init_buffer () {

	glGenVertexArrays (1, &varr_buff);
	glBindVertexArray (varr_buff);

	glGenBuffers (1, &vert_buff);
	glBindBuffer (GL_ARRAY_BUFFER, vert_buff);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vert_data), vert_data, GL_STATIC_DRAW);
	err = glGetError ();  test_func_l ("vertex buff", !err, { printf ("%x\n", err); });

	glGenBuffers (1, &vidx_buff);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vidx_buff);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (vidx_data), vidx_data, GL_STATIC_DRAW);
	err = glGetError ();  
	test_func_l ("vertex index buff", !err, { printf ("%x\n", err); });

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIP_SIZE, NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIP_SIZE, (void*)COL_OFFSET);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIP_SIZE, (void*)UV_OFFSET);
	glEnableVertexAttribArray(2);

}

Shader shader;

void init_shader () {
	_(Shader).Init(&shader);
	_(Shader).ParseFile(&shader,"data/raindrop.shader");
	// set shader sampler
	glUseProgram (shader.id);
	tex = _(Texture).Load("res/wall.jpg");

}

double tm_real_last;
double tm_real_now;
double tm_real_delta;
double tm_real_fps;
void time () {
	// time
	tm_real_now = glfwGetTime ();
	tm_real_delta = tm_real_now - tm_real_last;
	tm_real_last = tm_real_now;
	tm_real_fps = 1 / tm_real_delta;

	printf ("\r%lf, %lf ", tm_real_fps, tm_real_now);
}

void draw () {

	glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT);

	// shader
	double f = (sin (tm_real_now * M_PI_4) + 2) / 3;
	printf ("%f", f);
	glUseProgram(shader.id); err = glGetError(); if (err) printf("%x\n", err);
	//glUniform4d(shaderLocation(&shader, "_color", sizeof("_color") - 1), 0.3f * f, 0.5f * f, 0.7f * f, 1.0f);

	//texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture (GL_TEXTURE0 + 1);
	glBindTexture (GL_TEXTURE_2D, tex->id);
	glUniform1i(shaderLocation(&shader, "uTex", sizeof("uTex")-1), 1);

	// model
	glBindVertexArray (varr_buff); err = glGetError (); if (err) printf ("%x\n", err);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); err = glGetError (); if (err) printf ("%x\n", err);
	glBindVertexArray (0); err = glGetError (); if (err) printf ("%x\n", err);

}

test_start (glfw) {
	GLFWwindow* window;

	/* Initialize the library */
	test_func ("init glfw", glfwInit ());
#if GL_DEBUG
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 5);
#else
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow (640, 480, "Hello World", NULL, NULL);
	test_func ("create glfw window", window);
	/* Make the window's context current */
	glfwMakeContextCurrent (window);
	glfwSetFramebufferSizeCallback (window, framebuffer_size_callback);

	// load gl must after make glcontext
	test_func ("glad load gl", gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress));

	if (GLAD_GL_VERSION_4_3) {
		// During init, enable debug output
		glEnable (GL_DEBUG_OUTPUT);
		glDebugMessageCallback (MessageCallback, 0);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mem_init();
	init_shader ();
	init_buffer ();
	glfwSwapInterval (2);

	time ();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose (window))
	{
		// render
		// ------
		time ();

		draw ();

		processInput (window);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers (window);
		glfwPollEvents ();
	}
	mem_clear();
	glfwTerminate ();
}
test_end ()
