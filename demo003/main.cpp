#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "object.hpp"
#include "physics_simulator.hpp"

const char * vertexShaderSource =
"#version 330 core \r\n"
"layout (location = 0) in vec3 position; \r\n"
"out vec4 viewSpace; \r\n"
"uniform mat4 view; \r\n"
"uniform mat4 projection; \r\n"
"void main() \r\n"
"{ \r\n"
"    gl_Position = projection*view*vec4(position.x, position.y, position.z, 1.0); \r\n"
"    viewSpace = vec4(1.0f,1.0f,1.0f, 1.0f); \r\n"
"}";


const char * fragmentShaderSource =
"#version 330 core \r\n"
"in vec4 viewSpace; \r\n"
"out vec4 color; \r\n"
"void main() \r\n"
"{ \r\n"
"     color = viewSpace; \r\n"
"}";

// 摄像机参数
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 创建视图矩阵
glm::mat4 view;

// 创建投影矩阵
glm::mat4 projection;

//摄像机的速度
float camera_speed = 10;

// 物体运动速度系数
float move_seed = 1000000000;

std::vector<Object*> global_objects;

std::vector<Vertex> global_vertices = {
    {{-0.5f, -0.5f, -0.75f},{0.0f,0.0f,0.0f}}, // 顶点 1
    {{0.5f, -0.5f, -0.5f},{0.5f,0.0f,0.2f}}, // 顶点 2
    {{0.5f,  0.5f, -0.5f},{0.5f,0.0f,0.2f}}, // 顶点 3
    {{-0.5f,  0.5f, -0.5f},{0.5f,0.0f,0.2f}}, // 顶点 4
    {{-0.5f, -0.5f,  0.5f},{0.5f,0.0f,0.2f}}, // 顶点 5
    {{0.5f, -0.5f,  0.5f},{0.5f,0.0f,0.2f}}, // 顶点 6
    {{0.5f,  0.5f,  0.5f},{0.5f,0.0f,0.2f}}, // 顶点 7
    {{-0.5f,  0.5f,  0.5f},{0.5f,0.0f,0.2f}}  // 顶点 8
};

std::vector<Triangle_Link> global_indices = {
    // 底面
    {0, 1, 2},
    {0, 2, 3},

    // 顶面
    {4, 7, 6},
    {4, 6, 5},

    // 前面
    {0, 4, 5},
    {0, 5, 1},

    // 后面
    {2, 6, 7},
    {2, 7, 3},

    // 左面
    {0, 3, 7},
    {0, 7, 4},

    // 右面
    {1, 5, 6},
    {1, 6, 2}
};

void update_camera(int width,int height){
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void update_state(){
    size_t i,j;
    for(i=0;i<global_objects.size();i++){
        for(j=0;j<global_objects.size();j++){
            if(i==j) continue;
            float r,F,dx,dy,dz;
            glm::vec3 v;
            r = global_objects[i]->distance(*global_objects[j]);
            F = -G*(global_objects[i]->m * global_objects[j]->m / r*r);
            dx = global_objects[i]->dx(*global_objects[j]) * F;
            dy = global_objects[i]->dy(*global_objects[j]) * F;
            dz = global_objects[i]->dz(*global_objects[j]) * F;
            v = glm::vec3(dx,dy,dz);
            global_objects[i]->v-=v;
            global_objects[j]->v+=v;
            std::cout << "move " << dx << "," << dy << "," << dz << std::endl;
        }
    }
    for(i=0;i<global_objects.size();i++){
        if (!global_objects[i]->get_trajectory())
            continue;
        auto [x1, y1, z1] = global_objects[i]->get_position();
        global_objects[i]->move(global_objects[i]->v);
        auto [x2, y2, z2] = global_objects[i]->get_position();

        draw_line(x1, y1, z1, x2, y2, z2);
    }
}

void _debug(){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cout << "ERROR::"<< error << std::endl;
    }
}

char * read_shadertoy(const char * filename){
    int fd = open(filename, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    char *buf = (char *)malloc(st.st_size);
    read(fd, buf,st.st_size);
    return buf;
}

int main(int argc, char **args){
    int width = 1000, height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(width,height, "Mandel", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0,width, height);

    glfwSetKeyCallback(window,key_callback);

    GLint success;
    GLchar infoLog[512];
    GLuint EBO, VBO, vertexShader, fragmentShader;

    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1 , &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check if success to compile framentShader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILE" << infoLog << std::endl;
        return -1;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    // check if success to compile framentShader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILE" << infoLog << std::endl;
        return -1;
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK" << infoLog << std::endl;
        return -1;
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    test();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update_key_state();
        update_camera(width, height);
        update_state();

        glClearColor(0.f, 0.f, 0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //bind
        glBindVertexArray(VAO);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINE);

        //1.
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,global_vertices.size() * sizeof(Vertex) ,global_vertices.data(),GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,global_indices.size() * sizeof(Triangle_Link),global_indices.data(),GL_DYNAMIC_DRAW);

        //2.
        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        //unbind
        glBindVertexArray(0);

        //3.
        glUseProgram(shaderProgram);

        GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(viewLocation,1, GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation,1, GL_FALSE, glm::value_ptr(projection));

        //bind
        glBindVertexArray(VAO);
        //4.
        /* glDrawArrays(GL_TRIANGLES, 0,sizeof(vertices)/sizeof(GLfloat)); */
        glDrawElements(GL_TRIANGLES,global_indices.size() * sizeof(Triangle_Link),GL_UNSIGNED_INT, 0);
        //unbind
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}
