#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <vector>

#include "physics_simulator.hpp"

std::vector<bool> key_states(349,false), key_released_states(349,false);
extern glm::vec3 cameraPos;
extern glm::vec3 cameraTarget;
extern glm::vec3 cameraUp;
extern float camera_speed;

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS){
        key_states[key] = true;
    }else if(action == GLFW_RELEASE){
        key_states[key] = false;
        key_released_states[key] = true;
    }

    if((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void update_key_released_state(){
    /* if(key_released_states[GLFW_KEY_ENTER]){ */
    /*     /1* obj = &draw_cube(1,1,1,2); *1/ */
    /*     obj = draw_circle(1,1,1,rand() % 30); */
    /*     key_released_states[GLFW_KEY_ENTER] = false; */
    /* } */
}

void update_key_state(){
    if(key_states[GLFW_KEY_J]){
        cameraTarget.x -= dx*camera_speed;
    }
    if(key_states[GLFW_KEY_K]){
        cameraTarget.x += dx*camera_speed;
    }
    if(key_states[GLFW_KEY_H]){
        cameraTarget.y -= dx*camera_speed;
    }
    if(key_states[GLFW_KEY_L]){
        cameraTarget.y += dx*camera_speed;
    }
    if(key_states[GLFW_KEY_W]){
        cameraPos.z -= dx*camera_speed*3;
        cameraTarget.z -= dx*camera_speed*3;
    }
    if(key_states[GLFW_KEY_A]){
        cameraPos.x -= dx*camera_speed*3;
        cameraTarget.x -= dx*camera_speed*3;
    }
    if(key_states[GLFW_KEY_S]){
        cameraPos.z += dx*camera_speed*3;
        cameraTarget.z += dx*camera_speed*3;
    }
    if(key_states[GLFW_KEY_D]){
        cameraPos.x += dx*camera_speed*3;
        cameraTarget.x += dx*camera_speed*3;
    }

    if(key_states[GLFW_KEY_SPACE]){
        cameraPos.y += dx*camera_speed*3;
        cameraTarget.y += dx*camera_speed*3;
    }

    if(key_states[GLFW_KEY_LEFT_SHIFT]){
        cameraPos.y -= dx*camera_speed*3;
        cameraTarget.y -= dx*camera_speed*3;
    }
}
