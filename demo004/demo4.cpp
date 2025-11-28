#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

const char * vertexShaderSource =
"#version 330 core \r\n"
"layout (location = 0) in vec3 position; \r\n"
"out vec4 viewSpace; \r\n"
"void main() \r\n"
"{ \r\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0); \r\n"
"    viewSpace = vec4(position, 1.0f); \r\n"
"}";


const char * fragmentShaderSource =
"#version 330 core \r\n"
"in vec4 viewSpace; \r\n"
"out vec4 color; \r\n"
"void main() \r\n"
"{ \r\n"
"     color = viewSpace; \r\n"
"}";

char * read_shadertoy(const char * filename){
    int fd = open(filename, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    char *buf = (char *)malloc(st.st_size);
    read(fd, buf,st.st_size);
    return buf;
}


//basic unifrom params
GLint count = 8;
GLfloat r=1.39694462981453571600448659850713f,i=-0.00368452841936819587933234026143f;
GLfloat t=3.0;
GLfloat colors[8] = {10.0,7.0,3.0,2.0,4.34,3.14,6.5,7.8};
GLfloat fixedPrecision=0.00000001f;

GLfloat params[14]= {
    1,1,
    0,1,
    0,-1,
    -1,0.171,
    -1,-0171,
    1,0,
    -1,0,
};

std::vector<bool> key_states(256,false);

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS){
        key_states[key] = true;
    }else if(action == GLFW_RELEASE){
        key_states[key] = false;
    }

    if((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void update_state(){
    if(key_states[GLFW_KEY_J]){
        i += 0.03f/ (t*t);
    }
    if(key_states[GLFW_KEY_K]){
        i -= 0.03f / (t*t);
    }
    if(key_states[GLFW_KEY_L]){
        r -= 0.03f / (t*t);
    }
    if(key_states[GLFW_KEY_H]){
        r += 0.03f / (t*t);
    }
    if(key_states[GLFW_KEY_U]){
        t += 0.03f;
    }
    if(key_states[GLFW_KEY_I]){
        t -= 0.03f;
    }
    if(key_states[GLFW_KEY_N]){
        count++;
    }
    if(key_states[GLFW_KEY_M]){
        count--;
    }

    int j;
    for (j = 0; j < 7;j++) {
        if(key_states[GLFW_KEY_E] && key_states[GLFW_KEY_1 + j]){
            if(key_states[GLFW_KEY_LEFT_SHIFT]){
                params[j*2] += 0.003f / (t * t);
            }else{
                params[j*2] -= 0.003f / (t * t);
            }
            printf("update param%d(%lf,%lf)\n",j,params[j*2],params[j*2+1]);
        }
        if(key_states[GLFW_KEY_R] && key_states[GLFW_KEY_1 + j]){
            if(key_states[GLFW_KEY_LEFT_SHIFT]){
                params[j*2+1] += 0.003f / (t * t);
            }else{
                params[j*2+1] -= 0.003f / (t * t);
            }
            printf("update param%d(%lf,%lf)\n",j,params[j*2],params[j*2+1]);
        }
    }

    for (j = 0; key_states[GLFW_KEY_SPACE] && j < 8;j++) {
        if(key_states[GLFW_KEY_0 + j]){
            if(key_states[GLFW_KEY_LEFT_SHIFT]){
                colors[j]--;
            }else{
                colors[j]++;
            }
        }
    }
}

int main(int argc, char **args){
    if(!(argc == 2 || argc == 4 || argc == 5)){
        perror("./a.out <shader path> [width] [height] [fractal]");
        return -1;
    }
    fragmentShaderSource = read_shadertoy(args[1]);

    int width = 1000, height = 600;
    int fractal = 1;
    if (argc == 4) {
        width = atoi(args[2]);
        height = atoi(args[3]);
    }
    if(argc == 5) {
        width = atoi(args[2]);
        height = atoi(args[3]);
        fractal = atoi(args[4]);
    }

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
       1.0f, 1.0f, 0.0f,   // 右上角
       1.0f, -1.0f, 0.0f,  // 右下角
       // 第二个三角形
       -1.0f, -1.0f, 0.0f, // 左下角
       -1.0f, 1.0f, 0.0f   // 左上角
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

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update_state();

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
        glUniform2d(iResolutionLocation,width,height);

        GLfloat time = glfwGetTime();
        GLint iTimeLocation = glGetUniformLocation(shaderProgram,"iTime");
        glUniform1f(iTimeLocation,time);

        GLint iPointer = glGetUniformLocation(shaderProgram,"iPointer");
        glUniform2f(iPointer,r,i);

        GLint iCount = glGetUniformLocation(shaderProgram,"iCount");
        glUniform1i(iCount,count);

        GLint iViewSize = glGetUniformLocation(shaderProgram,"iViewSize");
        glUniform1f(iViewSize,t);

        GLint ic1 = glGetUniformLocation(shaderProgram,"c1");
        GLint ic2 = glGetUniformLocation(shaderProgram,"c2");
        GLint ic3 = glGetUniformLocation(shaderProgram,"c3");
        GLint ic4 = glGetUniformLocation(shaderProgram,"c4");
        GLint it1 = glGetUniformLocation(shaderProgram,"t1");
        GLint it2 = glGetUniformLocation(shaderProgram,"t2");
        GLint it3 = glGetUniformLocation(shaderProgram,"t3");
        GLint it4 = glGetUniformLocation(shaderProgram,"t4");
        glUniform1f(ic1,colors[0]);
        glUniform1f(ic2,colors[1]);
        glUniform1f(ic3,colors[2]);
        glUniform1f(ic4,colors[3]);
        glUniform1f(it1,colors[4]);
        glUniform1f(it2,colors[5]);
        glUniform1f(it3,colors[6]);
        glUniform1f(it4,colors[7]);

        GLint fixedPrecisionLocation = glGetUniformLocation(shaderProgram,"fixedPrecision");
        glUniform1f(fixedPrecisionLocation,fixedPrecision);

        GLint fractalOption = glGetUniformLocation(shaderProgram,"fractalOption");
        glUniform1i(fractalOption,fractal);

        GLint param1 = glGetUniformLocation(shaderProgram,"param1");
        glUniform2f(param1,params[0],params[1]);
        GLint param2 = glGetUniformLocation(shaderProgram,"param2");
        glUniform2f(param2,params[2],params[3]);
        GLint param3 = glGetUniformLocation(shaderProgram,"param3");
        glUniform2f(param3,params[4],params[5]);
        GLint param4 = glGetUniformLocation(shaderProgram,"param4");
        glUniform2f(param4,params[6],params[7]);
        GLint param5 = glGetUniformLocation(shaderProgram,"param5");
        glUniform2f(param5,params[8],params[9]);
        GLint param6 = glGetUniformLocation(shaderProgram,"param6");
        glUniform2f(param5,params[10],params[11]);
        GLint param7 = glGetUniformLocation(shaderProgram,"param7");
        glUniform2f(param7,params[12],params[13]);

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
