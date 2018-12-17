#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test.h"
#include "Mat.h"

#define GLSL_VERSION "#version 330 core\n"

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

uint vert_buff;
uint varr_buff;
uint vidx_buff;
vec3 vert_data[] = {
	0.5f, 0.5f, 0.0f,   // rigth-top
	0.5f, -0.5f, 0.0f,  // right-bottom
	-0.5f, -0.5f, 0.0f, // left-bottom
	-0.5f, 0.5f, 0.0f   // left-top
};
uint vidx_data[] = {
	0, 1, 3, // rigth-top
	1, 2, 3  // left-bottom
};


void init_buffer () {
	GLenum err;
	glGenVertexArrays (1, &varr_buff);
	glBindVertexArray (varr_buff);

	glGenBuffers (1, &vert_buff);
	glBindBuffer (GL_ARRAY_BUFFER, vert_buff);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vert_data), vert_data, GL_STATIC_DRAW);
	err = glGetError ();  test_func_l ("vertex buff", !err, { printf ("%x\n", err); });

	glGenBuffers (1, &vidx_buff);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vidx_buff);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (vidx_data), vidx_data, GL_STATIC_DRAW);
	err = glGetError ();  test_func_l ("vertex index buff", !err, { printf ("%x\n", err); });

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), NULL);
	err = glGetError ();  test_func_l ("vertex array buff", !err, { printf ("%x\n", err); });
	glEnableVertexAttribArray (0); // this index indicate send data to vertex shader `aPos`
}


uint vert_shader;
uint frag_shader;
uint prog;

const char* vert_src =
GLSL_VERSION	\
"in vec3 aPos;	\
	void main(){	\
	gl_Position = vec4(aPos.x,aPos.y,aPos.z, 1.0);	\
	}";
const char* frag_src =
GLSL_VERSION	\
"out vec4 _frag_;	\
	void main () {		\
	_frag_ = vec4 (1.0f, .5f, .2f, 1.0f);	\
	}";

void init_shader () {
	int succ;
	char log [512];
	// compile vertex shader
	vert_shader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vert_shader, 1, &vert_src, NULL);
	glCompileShader (vert_shader);
	glGetShaderiv (vert_shader, GL_COMPILE_STATUS, &succ);

	test_func_l ("compile vertex shader", succ, {
		glGetShaderInfoLog (vert_shader, sizeof (log), NULL, log);
		puts (log);
		});

	// compile fragment shader
	frag_shader = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (frag_shader, 1, &frag_src, NULL);
	glCompileShader (frag_shader);
	glGetShaderiv (frag_shader, GL_COMPILE_STATUS, &succ);

	test_func_l ("compile fragment shader", succ, {
		glGetShaderInfoLog (frag_shader,sizeof (log),NULL,log);
		puts (log);
		});

	prog = glCreateProgram ();
	glAttachShader (prog, vert_shader);
	glAttachShader (prog, frag_shader);
	glLinkProgram (prog);
	glGetProgramiv (prog, GL_LINK_STATUS, &succ);

	test_func_l ("link shader", succ, {
		glGetProgramInfoLog (prog,sizeof (log),NULL,log);
		puts (log);
		});

	glDeleteShader (vert_shader);
	glDeleteShader (frag_shader);
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
	test_func ("glad load gl", gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress));

	init_shader ();
	init_buffer ();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose (window))
	{
		// render
		// ------
		glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);

		GLenum err;
		glUseProgram (prog); err = glGetError (); if (err) printf ("%x\n", err);
		glBindVertexArray (varr_buff); err = glGetError (); if (err) printf ("%x\n", err);
		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); err = glGetError (); if (err) printf ("%x\n", err);
		glBindVertexArray (0); err = glGetError (); if (err) printf ("%x\n", err);

		processInput (window);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers (window);
		glfwPollEvents ();
	}

	glfwTerminate ();
}
test_end ()