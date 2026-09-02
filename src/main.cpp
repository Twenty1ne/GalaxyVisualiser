#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "Camera.h"
#include "GeometryBuilder.h"
#include "Point.h"
#include "Renderer.h"
#include "Scene.h"

#include "SimulationCell.h"
#include "StarGenerator.h"
#include "GalaxyData.h"

#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

// ================
#include <chrono>
// ================

struct AppState{
    bool cameraControlActive;

    int currentTimestep;
    const std::vector<std::vector<SimulationCell>>* timesteps;

    Renderer* renderer;
    std::size_t lowMassGeometryId;
    std::size_t highMassGeometryId;
};

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, double& lastMouseX, double& lastMouseY, bool& cameraControlActive, bool& previousCameraControlActive);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void focusCallback(GLFWwindow* window, int focused);
void loadTimestep(int timestep, const std::vector<std::vector<SimulationCell>>& timesteps, Renderer& renderer, std::size_t lowMassGeometryId, std::size_t highMassGeometryId);

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
    glfwSetWindowPos(window, 100, 100);

    if(!window){
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    AppState appState{};
    appState.cameraControlActive = true;

    bool previousCameraControlActive = true;

    glfwSetWindowUserPointer(
        window,
        &appState
    );

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowFocusCallback(window, focusCallback);

    glfwFocusWindow(window);
    glfwMakeContextCurrent(window);

    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "Failed to initialise GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    {
        int currentTimestep = 0;

        // ================
        auto loadStart = std::chrono::high_resolution_clock::now();
        // ================

        std::vector<std::vector<SimulationCell>> timesteps = GalaxyData::load("/home/tj/code/thesis/less_MBSOGM_III_history.txt");

        // ================
        auto loadEnd = std::chrono::high_resolution_clock::now();

        std::cout
            << "Loaded "
            << timesteps.size()
            << " timesteps in "
            << std::chrono::duration<double>(loadEnd - loadStart).count()
            << " seconds\n";
        // ================

        std::vector<Point> highMassPoints;
        std::vector<Point> lowMassPoints;

        // ================
        auto genStart = std::chrono::high_resolution_clock::now();
        // ================

        for(const SimulationCell& cell : timesteps[currentTimestep]){
            StarGenerator::generate(cell, highMassPoints, lowMassPoints);
        }

        // ================
        auto genEnd = std::chrono::high_resolution_clock::now();

        std::cout
            << "\nStar generation: "
            << std::chrono::duration<double>(genEnd - genStart).count()
            << " seconds\n";

        std::cout
            << "Occupied sites: "
            << timesteps[currentTimestep].size()
            << '\n';

        std::cout
            << "High mass points: "
            << highMassPoints.size()
            << '\n';

        std::cout
            << "Low mass points: "
            << lowMassPoints.size()
            << '\n';

        auto geometryStart = std::chrono::high_resolution_clock::now();
        // ================

        Geometry highMassGeometry = GeometryBuilder::makePoints(highMassPoints);
        Geometry lowMassGeometry = GeometryBuilder::makePoints(lowMassPoints);

        // ================
        auto geometryEnd = std::chrono::high_resolution_clock::now();

        std::cout
            << "\nPoint geometry construction: "
            << std::chrono::duration<double>(geometryEnd - geometryStart).count()
            << " seconds\n";
        // ================

        Geometry axes = GeometryBuilder::makeAxes();
        Geometry grid = GeometryBuilder::makeSimulationGrid();

        Renderer renderer;
        Camera camera;
        Scene scene;

        // ================
        auto uploadStart = std::chrono::high_resolution_clock::now();
        // ================

        std::size_t lowMassGeometryId = renderer.upload(lowMassGeometry);
        std::size_t highMassGeometryId = renderer.upload(highMassGeometry);

        scene.addGeometry(lowMassGeometryId);
        scene.addGeometry(renderer.upload(axes));
        scene.addGeometry(renderer.upload(grid));
        scene.addGeometry(highMassGeometryId, true);

        // ================
        auto uploadEnd = std::chrono::high_resolution_clock::now();

        std::cout
            << "\nGeometry upload calls: "
            << std::chrono::duration<double>(uploadEnd - uploadStart).count()
            << " seconds\n";
        // ================

        double lastMouseX;
        double lastMouseY;

        glfwGetCursorPos(
            window,
            &lastMouseX,
            &lastMouseY
        );

        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            GLFW_CURSOR_DISABLED
        );

        appState.currentTimestep = currentTimestep;
        appState.timesteps = &timesteps;
        appState.renderer = &renderer;
        appState.lowMassGeometryId = lowMassGeometryId;
        appState.highMassGeometryId = highMassGeometryId;

        double lastFrameTime = glfwGetTime();

        // ================
        bool profileFrame = true;
        // ================

        while(!glfwWindowShouldClose(window)){
            glfwPollEvents();

            double currentTime = glfwGetTime();

            float deltaTime = static_cast<float>(currentTime - lastFrameTime);

            lastFrameTime = currentTime;

            processInput(
                window,
                camera,
                deltaTime,
                lastMouseX,
                lastMouseY,
                appState.cameraControlActive,
                previousCameraControlActive
            );

            glClearColor(0.0f, 0.0, 0.0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int width;
            int height;

            glfwGetFramebufferSize(
                window,
                &width,
                &height
            );

            float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

            glm::mat4 view = camera.viewMatrix();

            // ================
            if(profileFrame){
                auto renderStart = std::chrono::high_resolution_clock::now();

                scene.render(renderer, aspectRatio, view);

                auto renderSubmissionEnd = std::chrono::high_resolution_clock::now();

                glFinish();

                auto renderEnd = std::chrono::high_resolution_clock::now();

                std::cout
                    << "\nRender submission: "
                    << std::chrono::duration<double>(renderSubmissionEnd - renderStart).count()
                    << " seconds\n";

                std::cout
                    << "\nRender + GPU completion: "
                    << std::chrono::duration<double>(renderEnd - renderStart).count()
                    << " seconds\n";

                profileFrame = false;
            }
            // ================
            else{
                scene.render(renderer, aspectRatio, view);
            }

            glfwSwapBuffers(window);
        }
    } // renderer is destroyed here, while OpenGL context still exists

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << std::endl;
    return 0;
}

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, double& lastMouseX, double& lastMouseY, bool& cameraControlActive, bool& previousCameraControlActive){
    if(!cameraControlActive){
        previousCameraControlActive = false;
        return;
    }

    if(!previousCameraControlActive){
        glfwGetCursorPos(
            window,
            &lastMouseX,
            &lastMouseY
        );

        previousCameraControlActive = true;
        return;
    }

    double mouseX;
    double mouseY;

    glfwGetCursorPos(
        window,
        &mouseX,
        &mouseY
    );

    float mouseDeltaX = static_cast<float>(mouseX - lastMouseX);
    float mouseDeltaY = static_cast<float>(lastMouseY - mouseY);

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    const float mouseSensitivity = 0.1f;
    const float movementSpeed = 7.0f;

    camera.rotate(
        mouseDeltaX * mouseSensitivity,
        mouseDeltaY * mouseSensitivity
    );

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.moveForward(movementSpeed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.moveForward(-movementSpeed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.moveRight(movementSpeed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.moveRight(-movementSpeed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.moveVertical(movementSpeed * deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.moveVertical(-movementSpeed * deltaTime);
    }
  }

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        state->cameraControlActive = false;

        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            GLFW_CURSOR_NORMAL
        );
    }

    if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS){
        if(state->currentTimestep + 1 < static_cast<int>(state->timesteps->size())){
            ++state->currentTimestep;

            loadTimestep(
                state->currentTimestep,
                *state->timesteps,
                *state->renderer,
                state->lowMassGeometryId,
                state->highMassGeometryId
            );
        }
    }

    if(key == GLFW_KEY_COMMA && action == GLFW_PRESS){
        if(state->currentTimestep > 0){
            --state->currentTimestep;

            loadTimestep(
                state->currentTimestep,
                *state->timesteps,
                *state->renderer,
                state->lowMassGeometryId,
                state->highMassGeometryId
            );
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

        state->cameraControlActive = true;

        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            GLFW_CURSOR_DISABLED
        );
    }
}

void focusCallback(GLFWwindow* window, int focused){
    AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if(focused) return;

    else{
        state->cameraControlActive = false;

        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            GLFW_CURSOR_NORMAL
        );
    }
}

void loadTimestep(int timestep, const std::vector<std::vector<SimulationCell>>& timesteps, Renderer& renderer, std::size_t lowMassGeometryId, std::size_t highMassGeometryId){
    std::vector<Point> highMassPoints;
    std::vector<Point> lowMassPoints;

    for(const SimulationCell& cell : timesteps[timestep]){
        StarGenerator::generate(cell, highMassPoints, lowMassPoints);
    }

    Geometry highMassGeometry = GeometryBuilder::makePoints(highMassPoints);
    Geometry lowMassGeometry = GeometryBuilder::makePoints(lowMassPoints);

    renderer.update(lowMassGeometryId, lowMassGeometry);
    renderer.update(highMassGeometryId, highMassGeometry);
}
