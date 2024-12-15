#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <iostream>

const char * vertexShaderSource =
"#version 330 core \r\n"
"layout (location = 0) in vec3 position; \r\n"
//"uniform vec2 iResolution; \r\n"
"uniform float time; \r\n"
"out vec4 vertexColor; \r\n"
"void main() \r\n"
"{ \r\n"
//"    vec2 uv = vec2(position.x,position.y) / iResolution.xy - 0.5f; \r\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0); \r\n"
"    vertexColor = vec4(position * sin(time), 1.0f); \r\n"
"}";

const char * fragmentShaderSource =
"#version 330 core \r\n"
"in vec4 vertexColor; \r\n"
"out vec4 color; \r\n"
"void main() \r\n"
"{ \r\n"
//"    color = vec4(1.0f,1.0f,1.0f, 1.0f); \r\n"
"     color = vertexColor; \r\n"
"}";

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(1000,600, "Mandel", nullptr, nullptr);
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0,width, height);

    glfwSetKeyCallback(window,key_callback);

    /* GLfloat vertices[] = { */
    /*    // 第一个三角形 */
    /*    0.8f, 0.5f, 0.0f,   // 右上角 */
    /*    0.5f, -0.5f, 0.0f,  // 右下角 */
    /*    -0.5f, 0.5f, 0.0f,   // 左上角 */
    /*    // 第二个三角形 */
    /*    0.5f, -0.5f, 0.0f,  // 右下角 */
    /*    -0.5f, -0.5f, 0.0f, // 左下角 */
    /*    -0.5f, 0.5f, 0.0f   // 左上角 */
    /* }; */
    GLfloat vertices[] = {
       // 第一个三角形
       0.8f, 0.5f, 0.0f,   // 右上角
       0.5f, -0.5f, 0.0f,  // 右下角
       // 第二个三角形
       -0.5f, -0.5f, 0.0f, // 左下角
       -0.5f, 0.5f, 0.0f   // 左上角
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

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

    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.f, 0.f, 0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //bind
        glBindVertexArray(VAO);

        //1.
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

        //2.
        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        //unbind
        glBindVertexArray(0);

        //3.
        glUseProgram(shaderProgram);

        GLint iResolutionLocation = glGetUniformLocation(shaderProgram, "iResolution");
        glUniform2f(iResolutionLocation,1000.0f,600.0f);

        GLfloat time = glfwGetTime();
        GLint iTimeLocation = glGetUniformLocation(shaderProgram,"time");
        glUniform1f(iTimeLocation,time);

        //bind
        glBindVertexArray(VAO);
        //4.
        /* glDrawArrays(GL_TRIANGLES, 0,6); */
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        //unbind
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}
