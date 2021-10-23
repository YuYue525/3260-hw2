/*
Student Information
Student ID: 1155124490
Student Name: YU Yue
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

GLfloat lastX = SCR_WIDTH / 2.0;
GLfloat lastY = SCR_HEIGHT / 2.0;
GLfloat fov = 45.0f;
glm::vec3 cameraPosBefore;

//camera views
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool keys[1024];
//Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//penguin pos
GLfloat delta_Z = 0.0f;
GLfloat delta_angle = 0.0f;

//brightness
GLfloat lightBrightness = 1;

bool firstMouse = true;
bool firstClick = true;
bool mouseLeftDown = false;




struct keyboardcontroller
{
    int theme_penguin;
    int theme_snow;
};
keyboardcontroller keyboardCtl = {0, 0};

// struct for storing the obj file
struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

Model loadOBJ(const char* objPath)
{
    // function to load the obj file
    // Note: this simple function cannot load all obj files.

    struct V {
        // struct for identify if a vertex has showed up
        unsigned int index_position, index_uv, index_normal;
        bool operator == (const V& v) const {
            return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
        }
        bool operator < (const V& v) const {
            return (index_position < v.index_position) ||
                (index_position == v.index_position && index_uv < v.index_uv) ||
                (index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
        }
    };

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::map<V, unsigned int> temp_vertices;

    Model model;
    unsigned int num_vertices = 0;

    std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

    std::ifstream file;
    file.open(objPath);

    // Check for Error
    if (file.fail()) {
        std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
        exit(1);
    }

    while (!file.eof()) {
        // process the object file
        char lineHeader[128];
        file >> lineHeader;

        if (strcmp(lineHeader, "v") == 0) {
            // geometric vertices
            glm::vec3 position;
            file >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            // texture coordinates
            glm::vec2 uv;
            file >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            // vertex normals
            glm::vec3 normal;
            file >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            // Face elements
            V vertices[3];
            for (int i = 0; i < 3; i++) {
                char ch;
                file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
            }

            // Check if there are more than three vertices in one face.
            std::string redundency;
            std::getline(file, redundency);
            if (redundency.length() >= 5) {
                std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
                std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
                std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
                exit(1);
            }

            for (int i = 0; i < 3; i++) {
                if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
                    // the vertex never shows before
                    Vertex vertex;
                    vertex.position = temp_positions[vertices[i].index_position - 1];
                    vertex.uv = temp_uvs[vertices[i].index_uv - 1];
                    vertex.normal = temp_normals[vertices[i].index_normal - 1];

                    model.vertices.push_back(vertex);
                    model.indices.push_back(num_vertices);
                    temp_vertices[vertices[i]] = num_vertices;
                    num_vertices += 1;
                }
                else {
                    // reuse the existing vertex
                    unsigned int index = temp_vertices[vertices[i]];
                    model.indices.push_back(index);
                }
            } // for
        } // else if
        else {
            // it's not a vertex, texture coordinate, normal or face
            char stupidBuffer[1024];
            file.getline(stupidBuffer, 1024);
        }
    }
    file.close();

    std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
    return model;
}

void get_OpenGL_info()
{
    // OpenGL information
    const GLubyte* name = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    std::cout << "OpenGL company: " << name << std::endl;
    std::cout << "Renderer name: " << renderer << std::endl;
    std::cout << "OpenGL version: " << glversion << std::endl;
}

Shader shader;
//Objects
GLuint snowVAO, snowVBO,snowEBO;
Model snowObj;
Texture snowTexture[2];

GLuint penguinVAO, penguinVBO,penguinEBO;
Model penguinObj;
Texture penguinTexture[2];


void sendDataToOpenGL()
{
    //TODO
    //Load objects and bind to VAO and VBO
    //Load textures
    
    //load the snow data from the OBJ file
    snowObj = loadOBJ("./resources/snow/snow.obj");
    //load snow textures
    snowTexture[0].setupTexture("resources/snow/snow_01.jpg");
    snowTexture[1].setupTexture("resources/snow/snow_02.jpg");
    //vertex array object
    glGenVertexArrays(1, &snowVAO);
    glBindVertexArray(snowVAO);
    
    //vertex buffer object
    glGenBuffers(1, &snowVBO);
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glBufferData(GL_ARRAY_BUFFER, snowObj.vertices.size() * sizeof(Vertex), &snowObj.vertices[0], GL_STATIC_DRAW);
    //vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    //vertex texture coordinate
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    //vertex normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //index buffer
    glGenBuffers(1, &snowEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, snowObj.indices.size() * sizeof(unsigned int), &snowObj.indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //load the penguin data from the OBJ file
    penguinObj = loadOBJ("./resources/penguin/penguin.obj");
    //load snow textures
    penguinTexture[0].setupTexture("resources/penguin/penguin_01.png");
    penguinTexture[1].setupTexture("resources/penguin/penguin_02.png");
    //vertex array object
    glGenVertexArrays(1, &penguinVAO);
    glBindVertexArray(penguinVAO);
    
    //vertex buffer object
    glGenBuffers(1, &penguinVBO);
    glBindBuffer(GL_ARRAY_BUFFER, penguinVBO);
    glBufferData(GL_ARRAY_BUFFER, penguinObj.vertices.size() * sizeof(Vertex), &penguinObj.vertices[0], GL_STATIC_DRAW);
    //vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    //vertex texture coordinate
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    //vertex normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //index buffer
    glGenBuffers(1, &penguinEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, penguinEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, penguinObj.indices.size() * sizeof(unsigned int), &penguinObj.indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

void initializedGL(void) //run only once
{
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW not OK." << std::endl;
    }

    get_OpenGL_info();
    sendDataToOpenGL();

    //TODO: set up the camera parameters
    //TODO: set up the vertex shader and fragment shader
    shader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void setupLight(Shader shader)
{
    
    //environment light
    glm::vec3 env_ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
    shader.setVec3("env_ambientLight", env_ambientLight);
    
    glm::vec3 env_lightDirection = glm::vec3(1.0f, 1.0f, 0.0f);
    shader.setVec3("env_lightDirection", env_lightDirection);
    
    shader.setVec3("eyePositionWorld", cameraPos);
    shader.setFloat("lightBrightness", lightBrightness);
    
    //point light 1
    glm::vec3 point1_ambientLight = glm::vec3(0.0f, 0.1f, 0.0f);
    shader.setVec3("point1_ambientLight", point1_ambientLight);
    glm::vec3 point1_lightPos = glm::vec3(-0.6f, -0.5f, -0.8f);
    shader.setVec3("point1_lightPos", point1_lightPos);
    
    //point light 2
    glm::vec3 point2_ambientLight = glm::vec3(0.1f, 0.0f, 0.0f);
    shader.setVec3("point2_ambientLight", point2_ambientLight);
    glm::vec3 point2_lightPos = glm::vec3(0.6f, -0.5f, 0.6f);
    shader.setVec3("point2_lightPos", point2_lightPos);
    
    //point light 3
    glm::vec3 point3_ambientLight = glm::vec3(0.0f, 0.0f, 0.1f);
    shader.setVec3("point3_ambientLight", point3_ambientLight);
    glm::vec3 point3_lightPos = glm::vec3(-0.6f, -0.5f, 0.6f);
    shader.setVec3("point3_lightPos", point3_lightPos);
    
    //point light 4
    glm::vec3 point4_ambientLight = glm::vec3(0.1f, 0.1f, 0.0f);
    shader.setVec3("point4_ambientLight", point4_ambientLight);
    glm::vec3 point4_lightPos = glm::vec3(0.6f, -0.5f, -0.8f);
    shader.setVec3("point4_lightPos", point4_lightPos);
    
    
}

void paintGL(void)  //always run
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f); //specify the background color, this is just an example
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //TODO:
    //Set lighting information, such as position and color of lighting source
    //Set transformation matrix
    //Bind different textures
    shader.use();

    //light
    setupLight(shader);
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(fov), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT,0.1f, 20.0f);
    
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
    shader.setMat4("model", model);
    snowTexture[keyboardCtl.theme_snow].bind(0);
    glBindVertexArray(snowVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowEBO);
    glDrawElements(GL_TRIANGLES, snowObj.indices.size(), GL_UNSIGNED_INT, 0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    
    model = glm::translate(model, glm::vec3(0.0f, -0.7f, delta_Z+0.0f));
    model = glm::rotate(model, glm::radians(delta_angle+0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    penguinTexture[keyboardCtl.theme_penguin].bind(0);
    glBindVertexArray(penguinVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, penguinEBO);
    glDrawElements(GL_TRIANGLES, penguinObj.indices.size(), GL_UNSIGNED_INT, 0);
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Sets the mouse-button callback for the current window.
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            firstClick = true;
            mouseLeftDown = true;
        }
        else if (action == GLFW_RELEASE)
        {
            firstMouse = true;
            mouseLeftDown = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    // Sets the cursor position callback for the current window
    if (!mouseLeftDown) {
        if (firstMouse)
        {
            lastX = x;
            lastY = y;
            cameraPosBefore = cameraPos;
            firstMouse = false;
        }

        GLfloat xoffset = x - lastX;
        GLfloat yoffset = lastY - y;

        GLfloat sensitivity = 0.01;    // Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        GLfloat cameraPosLen = glm::sqrt(cameraPosBefore.x * cameraPosBefore.x + cameraPosBefore.y * cameraPosBefore.y + cameraPosBefore.z * cameraPosBefore.z);

        cameraPos.x = (cos(glm::radians(-xoffset)) * cameraPosBefore.x + sin(glm::radians(-xoffset)) * cameraPosBefore.z);
        if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) > glm::radians(89.0f)) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(89.0f));
        }
        else if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) < glm::radians((-89.0f))) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(-89.0f));
        }
        else {
            cameraPos.y = cameraPosLen * glm::sin(glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset));
        }
        cameraPos.z = -sin(glm::radians(-xoffset)) * cameraPosBefore.x + cos(glm::radians(-xoffset)) * cameraPosBefore.z;

        cameraFront.x = -0.5 * cameraPos.x;
        cameraFront.y = -0.5 * cameraPos.y;
        cameraFront.z = -0.5 * cameraPos.z;
    }
    else {
        if (firstClick)
        {
            lastX = x;
            lastY = y;
            cameraPosBefore = cameraPos;
            firstClick = false;
        }

        GLfloat xoffset = x - lastX;
        GLfloat yoffset = lastY - y;

        GLfloat sensitivity = 0.5;    // Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        GLfloat cameraPosLen = glm::sqrt(cameraPosBefore.x * cameraPosBefore.x + cameraPosBefore.y * cameraPosBefore.y + cameraPosBefore.z * cameraPosBefore.z);

        cameraPos.x = (cos(glm::radians(-xoffset)) * cameraPosBefore.x + sin(glm::radians(-xoffset)) * cameraPosBefore.z);
        if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) > glm::radians(89.0f)) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(89.0f));
        }
        else if (glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset) < glm::radians((-89.0f))) {
            cameraPos.y = cameraPosLen * glm::sin(glm::radians(-89.0f));
        }
        else {
            cameraPos.y = cameraPosLen * glm::sin(glm::asin(cameraPosBefore.y / cameraPosLen) + glm::radians(-yoffset));
        }
        cameraPos.z = -sin(glm::radians(-xoffset)) * cameraPosBefore.x + cos(glm::radians(-xoffset)) * cameraPosBefore.z;

        cameraFront.x = -0.5 * cameraPos.x;
        cameraFront.y = -0.5 * cameraPos.y;
        cameraFront.z = -0.5 * cameraPos.z;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Sets the scoll callback for the current window.
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Sets the Keyboard callback for the current window.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
        keyboardCtl.theme_penguin = 0;
    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
        keyboardCtl.theme_penguin = 1;
    if(key == GLFW_KEY_3 && action == GLFW_PRESS)
        keyboardCtl.theme_snow = 0;
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
        keyboardCtl.theme_snow = 1;

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS){
            keys[key] = true;
            firstMouse = true;
            firstClick = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
            firstMouse = true;
            firstClick = true;
        }
    }
    
}

void do_movement()
{
    //penguin controls
    GLfloat Speed = 1.0f * deltaTime;
    if (keys[GLFW_KEY_UP])
        delta_Z += Speed;
    if (keys[GLFW_KEY_DOWN])
        delta_Z -= Speed;
    if (keys[GLFW_KEY_LEFT])
        delta_angle -= Speed * 30;
    if (keys[GLFW_KEY_RIGHT])
        delta_angle += Speed * 30;
    if(keys[GLFW_KEY_W])
        if(lightBrightness < 2)
            lightBrightness += 1 * Speed;
    if(keys[GLFW_KEY_S])
        if(lightBrightness > 0)
            lightBrightness -= 1 * Speed;
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    /* glfw: configure; necessary for MAC */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*register callback functions*/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);                                                                  //
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    initializedGL();

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        paintGL();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        do_movement();
    }

    glfwTerminate();

    return 0;
}







