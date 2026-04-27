#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/shader_m.h"
#include "utils/camera.h"
#include "utils/model.h"

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_ASP_OFFSET = SCR_WIDTH - SCR_HEIGHT;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    if (!glfwInit())
    {
        cerr << "Could not initialize GLFW" << endl;
        return -1;
    }

    // glfw: initialize and configure
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("src/shaders/model.vert", "src/shaders/model.frag");
    Shader lightShader("src/shaders/lighting.vert", "src/shaders/lighting.frag");
    Model ourModel("src/assets/backpack/backpack.obj");

    // plane: unit square in XZ at y=0
    vector<Vertex> planeVerts(4);
    // vertex           // position                     // normals      // tex coords
    planeVerts[0] = { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0,1,0), glm::vec2(0,1) };
    planeVerts[1] = { glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0,1,0), glm::vec2(1,1) };
    planeVerts[2] = { glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0,1,0), glm::vec2(1,0) };
    planeVerts[3] = { glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0,1,0), glm::vec2(0,0) };
    vector<unsigned int> planeIdx = { 0,1,2, 0,2,3 };

    Texture planeTex;
    planeTex.id = TextureFromFile("container.jpg", "src/assets");
    planeTex.type = "texture_diffuse";
    planeTex.path = "container.jpg";

    Texture planeTexSpec;
    planeTexSpec.id = TextureFromFile("container_specular.png", "src/assets");
    planeTexSpec.type = "texture_specular";
    planeTexSpec.path = "container_specular.png";

    Mesh planeMesh(planeVerts, planeIdx, { planeTex, planeTexSpec });

    // light cube: unit cube, positions only, 36 vertices
    float cubeVerts[] = {
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
         0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {    
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // animated light data: two lights orbiting the y-axis, 180° apart 
        float t      = currentFrame;                                                                                                                                     
            float radius = 3.0f;                                                                                                                                             
            glm::vec3 lightPos[2] = {                                                                                                                                        
                { radius * cos(t),            2.0f, radius * sin(t)            },                                                                                            
            { radius * cos(3.1416f*t + 3.1416f),  2.0f, radius * sin(3.1416f*t + 3.1416f) },                                                                                             
        };                                                                                                                                                               
        // rainbow colour cycling                                                                                
        glm::vec3 lightDiff[2] = {                                                                                                                                       
            { 0.5f+0.5f*sin(2*t),          0.5f+0.5f*sin(2*t+2.094f),          0.5f+0.5f*sin(2*t+4.189f)          },                                                           
            { 0.5f+0.5f*sin(t+3.1416f),  0.5f+0.5f*sin(t+3.1416f+2.094f),  0.5f+0.5f*sin(t+3.1416f+4.189f) },                                                            
        };    

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setVec3("viewPos", camera.Position);

        for (int i = 0; i < 2; i++)
        {
            string idx = "pointLights[" + to_string(i) + "].";
            ourShader.setVec3 (idx + "position",  lightPos[i]);
            ourShader.setVec3 (idx + "ambient",   glm::vec3(0.05f));
            ourShader.setVec3 (idx + "diffuse",   lightDiff[i]);
            ourShader.setVec3 (idx + "specular",  lightDiff[i]);
            ourShader.setFloat(idx + "constant",  1.0f);
            ourShader.setFloat(idx + "linear",    0.09f);
            ourShader.setFloat(idx + "quadratic", 0.032f);
        }

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        glm::mat4 planeModel = glm::translate(model, glm::vec3(0.0f,-1.75f,-0.5f));
        planeModel = glm::scale(planeModel, glm::vec3(8.0f));
        ourShader.setMat4("model", planeModel);
        planeMesh.Draw(ourShader);

        // render light cubes
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        glBindVertexArray(cubeVAO);
        for (int i = 0; i < 2; i++)
        {
            glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.05f));
            lightShader.setMat4("model", lightModel);
            lightShader.setVec3("lightColor", lightDiff[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // to keep viewport looking square
    glViewport(0, 0, width, height + SCR_ASP_OFFSET/2);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}