#include <GLFW/glfw3.h>

#include <cstdlib>

int
main()
{
  if (glfwInit() == GLFW_FALSE) {
    return EXIT_FAILURE;
  }

  GLFWwindow* p_window =
    glfwCreateWindow(800, 800, (const char*)u8"ゴース", nullptr, nullptr);
  if (p_window == nullptr) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(p_window);
  glfwSwapInterval(1);

  while (!glfwWindowShouldClose(p_window)) {
    glfwSwapBuffers(p_window);
    glfwPollEvents();
  }

  glfwDestroyWindow(p_window);
  glfwTerminate();

  return EXIT_SUCCESS;
}