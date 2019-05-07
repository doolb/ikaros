#include "graphic/glhelp.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "test.h"
#include "Mat.h"
#include "graphic/shader.h"
#include "graphic/texture.h"
#include "graphic/mesh.h"

#define GLSL_VERSION "#version 330 core\n"
GLenum err;
#define GL_ERR(err) err = glGetError(); if(err) printf("\n0x%x\n", err); assert(err == 0);
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

pMesh mesh;
pTexture tex;
Shader shader;

void init () {
	_(Shader).Init(&shader);
	_(Shader).ParseFile(&shader,"data/raindrop.shader");
	// set shader sampler
	glUseProgram (shader.id);
	tex = _(Texture).Load("res/wall.jpg");
	mesh = _(Mesh).Load("res/cube.obj");
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
	glUseProgram(shader.id); GL_ERR(err);
	//glUniform4d(shaderLocation(&shader, "_color", sizeof("_color") - 1), 0.3f * f, 0.5f * f, 0.7f * f, 1.0f);

	//texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture (GL_TEXTURE0 + 1);
	glBindTexture (GL_TEXTURE_2D, tex->id);
	glUniform1i(shaderLocation(&shader, "uTex", sizeof("uTex")-1), 1);
	int _time = shaderLocation(&shader, "_Time", sizeof("_Time") - 1);
	if(_time >= 0)
		glUniform1f(_time, tm_real_now);
	
	// model
	glBindVertexArray (mesh->id); GL_ERR(err);
	glPointSize(10);
	glLineWidth(10);
	glDrawElements (GL_POINTS, mesh->numTriangles, GL_UNSIGNED_INT, 0);GL_ERR(err);
	glBindVertexArray (0); GL_ERR(err);

}

test_start (glfw) {
	GLFWwindow* window;

	/* Initialize the library */
	test_func ("init glfw", glfwInit ());
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
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

	printf(glGetString(GL_VERSION));
	printGLVersion();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mem_init();
	init ();
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
