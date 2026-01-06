#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include  <iostream>

class Shader {
  public: 
    unsigned int shaderProgramID;

    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
      // 1. retrieve the vertex/fragment source code from file path 
      std::string vertexShaderCode;
      std::string fragmentShaderCode; 
      std::ifstream vertexShaderInputFileStream;
      std::ifstream fragmentShaderInputFileStream;

      vertexShaderInputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fragmentShaderInputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try {
        vertexShaderInputFileStream.open(vertexShaderFilePath);
        fragmentShaderInputFileStream.open(fragmentShaderFilePath);
        // std::stringstream allows us to treat "a string in memory" as 
        // "a standard input/output stream". 
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderInputFileStream.rdbuf();
        fragmentShaderStream << fragmentShaderInputFileStream.rdbuf();

        // convering streams into strings
        vertexShaderCode = vertexShaderStream.str();
        fragmentShaderCode = fragmentShaderStream.str();
        std::cout << "SUCCESS::SHADER::FILE_SUCCESSFULLY_READ" << std::endl;
      } catch(std::ifstream::failure &exception) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << exception.what() << std::endl;
      }
      const char* vertexShaderSourseCode = vertexShaderCode.c_str();
      const char* fragmentShaderSourceCode = fragmentShaderCode.c_str();

      unsigned int vertexShader, fragmentShader;

      // vertex shader
      vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader,1, &vertexShaderSourseCode, NULL);
      glCompileShader(vertexShader);
      checkCompileErrors(vertexShader, "VERTEX");

      // fragment shader
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
      glCompileShader(fragmentShader);
      checkCompileErrors(fragmentShader, "FRAGMENT");

      // shader program
      shaderProgramID = glCreateProgram();
      glAttachShader(shaderProgramID, vertexShader);
      glAttachShader(shaderProgramID, fragmentShader);
      glLinkProgram(shaderProgramID);
      checkCompileErrors(shaderProgramID, "PROGRAM");

      // delete shaders
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
    }

    // activating and using the shader program
    void use() {
      glUseProgram(shaderProgramID);
    }

  private:
    void checkCompileErrors(unsigned int shader_or_program, std::string type) {
      int success{0};
      char infoLog[1024];
      if (type != "PROGRAM") {
        glGetShaderiv(shader_or_program, GL_COMPILE_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(shader_or_program, 1024, NULL, infoLog);
          std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        } else {
          std::cout << "SUCCESS::SHADER::COMPILED" << std::endl;
        }
      } else {
        glGetProgramiv(shader_or_program, GL_LINK_STATUS, &success);
        if (!success) {
          glGetProgramInfoLog(shader_or_program, 1024, NULL, infoLog);
          std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        } else {
          std::cout << "SUCCESS::PROGRAM::LINKED" << std::endl;
        }
      }
    }
};
#endif
