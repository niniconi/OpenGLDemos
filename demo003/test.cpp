#include <glm/glm.hpp>
#include "physics_simulator.hpp"

void test(){
    draw_grid(5.0f,20.0f);
    Object *obj1, *obj2, *obj3;
    obj1 = draw_sphere(.1,.1,.1, 3000,glm::vec3(.01,.01,.01), .7);
    obj2 = draw_sphere(.1,2.0,.1, 2000,glm::vec3(.01,-.01,.01), .7);
    obj3 = draw_sphere(.1,.1,1.0, 4000,glm::vec3(-.01,.01,.01), .7);
    obj1->set_trajectory(true);
    obj2->set_trajectory(true);
    obj3->set_trajectory(true);
}
