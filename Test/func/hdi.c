#include "test.h"
#include "config.h"

#include "graphic/glhelp.h"

wchar* lang_glfw_init_err = TEXT("glfw≥ı ºªØ ß∞‹!");

test_start(hdi)
{
	GLFWwindow* window;
	if(glfwInit() == GLFW_FALSE)
	{
		MessageBox(NULL, lang_glfw_init_err, NULL, 0);
		exit(-1);
	};

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// suport opengl 1.0 ???
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	
}
test_end()