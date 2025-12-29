#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Specifying three vertex coordinates -> We only need three, because intend to
// draw a single triangle
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

const char *vertexShaderSourceCode = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f)"
    "}\0";

const char *fragmentShaderSourceCode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n" 
    "{\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() { 
  unsigned int VBO; // We use VBO, to store the generated buffer object names produced by "glGenBuffers" call.
  
  // Generating "1" buffer with a unique buffer ID,
  // and storing it inside the VBO
  glGenBuffers(1, &VBO);

  // Binding the newly created buffer (which is stored in "VBO") to the "GL_ARRAY_BUFFER" target
  // via the "glBindBuffer" function
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Copying the previously defined vertex data into buffer's memory 
  glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

  // NOTE: At this point, we have stored the vertex data within the memory of the graphics card
  // as managed by a vertex buffer object named "VBO".

  // Creating a vertex shader by calling "glCreateShader" function
  // and setting the type of shader we want to "GL_VERTEX_SHADER".
  // then we store the returned shader's ID in a variable called 
  // "vertexShader" so that we could access it in our C++ code 
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Attaching the shader source code to the shader object
  // that we created above
  glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
  // Calling "glCompileShader" to compile the shader's source code
  // on the GPU during program runtime.
  glCompileShader(vertexShader);

  // Error handling
  // Below, we check whether the shader compilation was successfull 
  // after calling "glCompileShader" or not.
  int success {0};
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    return 1;
  }

  // Creating a fragment shader by calling "glCreateShader" function
  // and setting the type of shader to "GL_FRAGMENT_SHADER",
  // then we store the returned shader ID in a variable called
  // "fragmentShader" so that we could reference it later on
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  
  // Attaching shaders sourvce code to the shader object
  // that we created above
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
  // Calling "glCompileShader" to compile shader's source code
  // on the GPU during program runtime
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return 2;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  return 0; 
}
