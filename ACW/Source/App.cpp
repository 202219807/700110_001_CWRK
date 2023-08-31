#pragma region includes
/*
GLEW is the OpenGL Extension Wrangler Library which is a simple tool that helps C/C++ developers
initialize extensions and write portable applications.
http://glew.sourceforge.net/
*/
#include <GL/glew.h>

/*
Dear ImGui is a bloat-free graphical user interface library for C++.
https://github.com/ocornut/imgui
*/
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/*
GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
It provides a simple API for creating windows, contexts and surfaces, receiving input and events.
https://www.glfw.org/
*/
#include <GLFW/glfw3.h>

/*
OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the
OpenGL Shading Language (GLSL) specifications.
https://github.com/g-truc/glm
*/
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

/*
Additional resources for development
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>

#include "helpers/GuiHelpers.h"
#include "helpers/RenderHelpers.h"

#include "World.h"
#include "Physics.h"
#include "Camera.h"
#include "Config.h"

#pragma endregion includes

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(24.0f, 10.0f, 50.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

GLFWwindow* window;
IRenderHelpers* renderHelper;
IGuiHelpers* guiHelper;

World world;

bool isPaused;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Camera& pCamera, float pDeltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pCamera.ProcessKeyboard(FORWARD, pDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pCamera.ProcessKeyboard(BACKWARD, pDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            pCamera.ProcessKeyboard(LEFT, pDeltaTime);
        else
            pCamera.ProcessKeyboard(ROTATE_LEFT, pDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            pCamera.ProcessKeyboard(RIGHT, pDeltaTime);
        else
            pCamera.ProcessKeyboard(ROTATE_RIGHT, pDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        pCamera.ProcessKeyboard(ROTATE_FORWARD, pDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        pCamera.ProcessKeyboard(ROTATE_BACKWARD, pDeltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* setupWindow()
{
    GLFWwindow* window;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Initialize the library */
    if (!glfwInit())
        return 0;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Physical Simulation and Artificial Intelligence", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

void renderScoreCardGui()
{
    ImGui::Begin("Score Card");
    auto red_score = world.m_RedTeam._score;
    auto blue_score = world.m_BlueTeam._score;
    ImGui::Text("Red Team : %.2f", &red_score);
    ImGui::Text("Blue Team : %.2f", &blue_score);
    ImGui::End();
}

void renderTowerGui()
{
    ImGui::Begin("Tower Influence Level");
    for (auto i : world.GetTowers())
        guiHelper->RenderGui(i);

    // TO DO - Content here
    ImGui::End();
}

void renderAgentGui()
{
    ImGui::Begin("Agent Statistics");
    ImGui::Text("* Red Team * ");
    for (auto i : world.GetPlayers(1))
        guiHelper->RenderGui(i);
    
    ImGui::Text("* Blue Team * ");
    for (auto i : world.GetPlayers(2))
        guiHelper->RenderGui(i);

    ImGui::End();
}

void renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Simulation Controls");

    camera.RenderGui(*guiHelper);

    const char* items[] = { "Improved Euler", "RK4", "Verlet" };
    static int currentItem = 1;

    if (!isPaused)
    {
        world.RenderGui(*guiHelper);
        ImGui::Text("Number of players per team :");
        ImGui::SameLine();
        auto size = _NUM_OF_AGENTS_PER_TEAM;
        ImGui::Text("%d", size);
        ImGui::NewLine();

        ImGui::Text("Numerical Integration Method: %s", items[currentItem]);
        ImGui::Text("Physical constants :");
        
        ImGui::Text("Gravitational acceleration (g) :");
        ImGui::SameLine();
        auto g = _GRAVITATIONAL_ACCELERATION;
        ImGui::Text("%.2f", g);
        
        ImGui::Text("Drag coefficient (c) :");
        ImGui::SameLine();
        auto c = _DRAG_COEFFICIENT;
        ImGui::Text("%.2f", c);

        ImGui::Text("Material restitution (e) :");
        ImGui::SameLine();
        auto e = _RESTITUTION;
        ImGui::Text("%.2f", e);

    }
    else
    {
        ImGui::Text("Integration Method:");
        ImGui::SetNextItemWidth(100);
        ImGui::Combo("", &currentItem, items, IM_ARRAYSIZE(items));
        ImGui::NewLine();
    }

    if (isPaused)
    {
        if (ImGui::Button("Start"))
        {
            isPaused = false;

            auto& physicsMgr = PhysicsManager::getInstance();
            
            switch (currentItem) {
                case 0:
                    physicsMgr.setIntegrator(std::make_shared<EulerIntegrator>());
                    break;
                case 1:
                    physicsMgr.setIntegrator(std::make_shared<RK4Integrator>());
                    break;
                case 2:
                    physicsMgr.setIntegrator(std::make_shared<VerletIntegrator>());
                    break;
                default:
                    cout << "Error: Incorrect Integrator Arg" << endl;
                    break;
            }
        }
    }
    else
    {
        if (ImGui::Button("Pause"))
        {
            isPaused = true;
        }
    }

    renderScoreCardGui();
    renderTowerGui();
    renderAgentGui();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void renderOpenGL()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.Render(*renderHelper);
    world.Render(*renderHelper);
}

int main(void)
{
    srand(static_cast <unsigned> (time(0)));

    window = setupWindow();
    renderHelper = new OpenGLRenderHelpers();
    guiHelper = new ImGuiHelpers();
    renderHelper->Init();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    renderHelper->SetSize(display_w, display_h);

    double lastTime = glfwGetTime();
    double timeNow = lastTime;
    double renderTime = 1.0 / 60.0;
    double updateTime = 1.0 / 100.0f;

    isPaused = true;

    while (!glfwWindowShouldClose(window))
    {
        timeNow = glfwGetTime();
        double elaspedTime = timeNow - lastTime;
        timeNow = lastTime;
        renderTime -= elaspedTime;

        if (world.TimeUp())
            isPaused = true;

        // render 60 times a second
        if (renderTime < 0)
        {
            renderTime = 1.0 / 60.0;
            renderOpenGL();
            renderImGui();
            glfwSwapBuffers(window);
        }

        if (!isPaused)
        {
            updateTime -= elaspedTime;  
            // update physics 100 times a second
            if (updateTime < 0)
            {
                updateTime = 1.0 / 100.0f;
                world.Update(updateTime);
            }
            processInput(window, camera, updateTime);
        }
        else
        {
            processInput(window, camera, updateTime);
        }

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete renderHelper;
    delete guiHelper;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}
