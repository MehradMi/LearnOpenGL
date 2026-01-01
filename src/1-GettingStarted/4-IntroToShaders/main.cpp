#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// Window settings
const unsigned int SCREEN_WIDTH{800};
const unsigned int SCREEN_HEIGHT{600};
const char *WINDOW_TITLE = "Introduction To Shaders and GLSL";

// Vertext shader's source code
const char *vertexShaderSourceCode =
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(pos, 1.0);\n"
    " vertexColor = color;\n"
    "}\0";

// Fragment shader's source code with "vertexColor"
/*
const char *fragmentShaderSourceCode =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vertexColor;\n"
    "}\0";
*/ 

/*
const char *fragmentShaderSourceCode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 Color;\n"
    "void main()\n"
    "{\n"
    " FragColor = Color;\n"
    "}\0";
*/

const char *fragmentShaderSourceCode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in  vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(vertexColor, 1.0f);\n"
    "}\0";

int main() {

  // GLFW: Initialization and Configuration
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW: Window creation
  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);

  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Setting the callback function upon window resize
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD: Loading all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Build and compile shader program
  //
  // Vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
  glCompileShader(vertexShader);

  int success {0};
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -2;
  }

  // Fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return -2;
  }

  // Linking shaders and creatng shader program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for shader linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return -3;
  }

  // Deleting shader's as we don't need them anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Set up vertex data (and buffer(s)) and configuring vertex attributes
  // Vertex data
  float vertices[] = {
    // positions                      // colors
    0.5f,  -0.5f, 0.0f,   1.0f,  0.0f,  0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f,  1.0f, 0.0f,   // bottom left
    0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
  }; 

  unsigned int VBO, VAO;
  // Generate VAO
  glGenVertexArrays(1, &VAO);

  // Bind/Activate the VAO first 
  // then bind and set vertex buffer(s)
  glBindVertexArray(VAO);

  // 1. Generate VBO
  // 2. Bind the VBO
  // 3. Then send the vertex data, stored in "vertices", to the VBO on GPU
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER , sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Now we tell OpenGL how to interpret the vertex data (per vertex attribute) 
  // using "glVertexAttribPointer", then we enable the vertex attributes 
  // using "glEnableVertexAttribArray" as they are disabled initially.

  // attribute at index/location 0 : position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // attribute at index/location 1 : color 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // TODO: unbind VBO
  // TODO: unbind VAO

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // input 
    processInput(window);

    // render 
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Making sure that we are using the shader program,
    // simply put, making sure shaders are activated before we do any drawing
    glUseProgram(shaderProgram);

    // update uniform 
    /*
    float timeValue = glfwGetTime();
    float greenValue = static_cast<float>(std::sin(timeValue / 2.0f) + 0.5f);
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "Color");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    */

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap buffers 
    glfwSwapBuffers(window);

    // listen for events (key pressed/released, mouse movement etc.)
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}


// Helper Function: Processing Input 
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    glfwSetWindowShouldClose(window, true);
}

// Helper Function: Adjust Viewport Dimensions Upon Window Resize 
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
