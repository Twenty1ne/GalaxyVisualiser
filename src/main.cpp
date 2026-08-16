#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "Geometry.h"
#include "Renderer.h"

#include <iostream>
#include <vector>

int main(){
    std::cout << std::endl;

    if(!glfwInit()){
        std::cerr << "Failed to initialise GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Galaxy Viewer", nullptr, nullptr);

    if(!window){
        std::cerr << "Faile to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "Failed to initialise GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    {
        std::vector<float> triangleVertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        Geometry triangle(triangleVertices);
        Renderer renderer;

        renderer.upload(triangle);

        while(!glfwWindowShouldClose(window)){
            glClearColor(0.5f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderer.render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // renderer is destroyed here, while OpenGL context still exists

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << std::endl;
    return 0;
}
