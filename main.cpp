//STD
#include <iostream>

//GLAD
#include <glad/glad.h>

//GLM
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp> //Access to the value_ptr

//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//LEARNOPENGL
#include <learnopengl/shader_m.h>
#include "model.h"

//GENERAL
#include "main.h"

using namespace std;
using namespace glm;

//Window
int windowWidth;
int windowHeight;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

//Transformations
//Relative position within world space
vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
//The direction of travel
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//Up position within world space
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

//Camera sidways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

//Time
//Time change
float deltaTime = 0.0f;
//Last value of time change
float lastFrame = 0.0f;

int main()
{

    //Initialisation of GLFW
    glfwInit();
    //Initialisation of 'GLFWwindow' object
    windowWidth = 1280;
    windowHeight = 720;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CW2", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    //Sets cursor to automatically bind to window & hides cursor pointer
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);

    //Initialisation of GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD failed to initialise\n";
        return -1;
    }

    //Loading of shaders
    Shader Shaders("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
    //Loading of models
    Model Street("media/street/street.obj");
    Model Building("media/street/building.obj");
    Model Signature("media/street/signature.obj");
    Shaders.use();

    //Sets the viewport size within the window to match the window size of 1280x720
    glViewport(0, 0, 1280, 720);

    //Sets the framebuffer_size_callback() function as the callback for the window resizing event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Sets the mouse_callback() function as the callback for the mouse movement event
    glfwSetCursorPosCallback(window, mouse_callback);

    //Model matrix
    mat4 model = mat4(1.0f);
    //Scaling to zoom in
    model = scale(model, vec3(0.025f, 0.025f, 0.025f));
    //Looking straight forward
    model = rotate(model, radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    //Elevation to look upon terrain
    model = translate(model, vec3(0.0f, -2.f, -1.5f));

    //Projection matrix
    mat4 projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);


    unsigned int amount = 35;//amount of buildings in the scene
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float zOffset = 1.0f;
    int xOffset = 1;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float x = (-20.0f) + (-50.0f);
        float y = (4.0f);
        float z = -51.0f;

        if (xOffset > 5) {
            xOffset = 0;
            zOffset++;
        }
        xOffset++;
        float rotangle = (float)(rand() % 4);
        
        model = glm::translate(model, glm::vec3(x+(xOffset*20.0f), y, z + (zOffset*17.0f)));//used to calculate the distance between the layed out buildings
        model = glm::rotate(model, 360 / rotangle, vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }


    //Render loop
    while (glfwWindowShouldClose(window) == false)
    {
        //Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        ProcessUserInput(window); //Takes user input

        //Rendering
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT); //Clears the colour buffer
        glClear(GL_DEPTH_BUFFER_BIT); 

        glEnable(GL_CULL_FACE); //Discards all back-facing triangles
        glEnable(GL_DEPTH_TEST);//disallows textures being shown in the order they are rendered and allows them to not overlap
        
        //Transformations
        mat4 view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //Sets the position of the viewer, the movement direction in relation to it & the world up direction
        mat4 projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        Shaders.use();
        Shaders.setMat4("projection", projection);
        Shaders.setMat4("view", view);
        mat4 model = mat4(1.0f);
        model = glm::translate(model, vec3(-1.0f, -1.0f, 0.0f));
        model = rotate(model, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
        model = scale(model, glm::vec3(3.0f));
        Shaders.setMat4("model",model); //Setting of uniform with Shader class


        glBegin(GL_QUADS);//draws a quad for the signature
        glVertex2f(-0.8f, 0.1f);
        glVertex2f(-0.2f, 0.1f);
        glVertex2f(-0.2f, 0.7f);
        glVertex2f(-0.8f, 0.7f);
        glEnd();
        

        //draws street map
        model = mat4(1.0f);
        float scale = 0.5f;
        model = glm::translate(model, vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale));
        Shaders.setMat4("model", model);
        Street.Draw(Shaders);

        // draws buildings
        for (unsigned int i = 0; i < amount; i++)
        {
            Shaders.setMat4("model", modelMatrices[i]);
            Building.Draw(Shaders);
        }

        model = mat4(1.0f);//draws the signature
        scale = 0.2f;
        model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale));
        Shaders.setMat4("model", model);
        Signature.Draw(Shaders);

        //Refreshing
        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all GLFW events
    }

    //Safely terminates GLFW
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Resizes window based on contemporary width & height values
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Initially no last positions, so sets last positions to current positions
    if (mouseFirstEntry)
    {
        cameraLastXPos = (float)xpos;
        cameraLastYPos = (float)ypos;
        mouseFirstEntry = false;
    }

    //Sets values for change in position since last frame to current frame
    float xOffset = (float)xpos - cameraLastXPos;
    float yOffset = cameraLastYPos - (float)ypos;

    //Sets last positions to current positions for next frame
    cameraLastXPos = (float)xpos;
    cameraLastYPos = (float)ypos;

    //Moderates the change in position based on sensitivity value
    const float sensitivity = 0.025f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    //Adjusts yaw & pitch values against changes in positions
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
    direction.y = sin(radians(cameraPitch));
    direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
    cameraFront = normalize(direction);
}

void ProcessUserInput(GLFWwindow* WindowIn)
{
    //Closes window on 'exit' key press
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowIn, true);
    }

    //Extent to which to move in one instance
    const float movementSpeed = 1.0f * deltaTime;
    //WASD controls
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_SPACE) == GLFW_PRESS)//space to go up
    {
        cameraPosition += movementSpeed * cameraUp;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)//control to go down
    {
        cameraPosition -= movementSpeed * cameraUp;
    }
}
