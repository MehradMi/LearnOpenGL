#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

constexpr int  WINDOW_WIDTH_PX = 800;
constexpr int  WINDOW_HEIGHT_PX = 600;
const char* WINDOW_TITLE {"HelloWindow"};

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height);
void processInput(GLFWwindow* window);

int main() {
    glfwInit();                                                      // 1. Initializing GLFW
    // Telling GLFW which version of OpenGL we want
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                   // 2. Configuring GLFW using the glfwWindowHint
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                   // 3. Same as (2)
                                                                     
    // Telling GLFW we want to use the core-profile
    // core-profile: smaller subset of OpenGL feature without backwards-compatible features we no longer need.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // 4. Same as (2)
    
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Telling GLFW to make the context of the window the main context of the "current thread"
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_HEIGHT_PX, WINDOW_HEIGHT_PX);
    
    // Telling GLFW which callback function to use on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    while (!glfwWindowShouldClose(window)) {
        // Input Handling
        processInput(window);
        
        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height) {
    glViewport(0, 0, new_width, new_height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}