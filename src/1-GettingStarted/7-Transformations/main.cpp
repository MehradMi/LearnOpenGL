#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int WINDOW_WIDTH  = 800;
const unsigned int WINDWO_HEIGHT = 600;

int main() {

  // glfw: initialization and configuration
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ 
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

  // glfw window creation 
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIDTH, "Transformations", NULL, NULL);
  if (window == nullptr) {
    std::cerr << "Faild To Create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  // TODO: Frame buffer size callback

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Faild To Initialize GLAD" << std::endl;
    return -1;
  }

  // vertex data 
  float vertices[] = {
    // coordinates                      // colors                        // texture coords
    0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0, 0.0,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left 
    -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
  };

  // indices
  unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  // Vertex Buffer Object, Vertex Array Object, Element Buffer Object 
  // ----------------------------

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO); 

  glGenBuffers(1, &VBO); 
  glGenBuffers(1, &EBO); 

  glBindVertexArray(VAO); 

  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // ----------------------------
  
  // Configuring Vertex Attributes 
  // ----------------------------
  
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  
  // ----------------------------

  // Load and Create Texture 
  // ----------------------------

  unsigned int texture1, texture2;
  // texture 1
  glGenTextures(1, &texture1); 
  glBindTexture(GL_TEXTURE_2D, texture1); 

  // setting texture "wrapping" parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // setting texture "filtering" parameters 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  return 0;
}
