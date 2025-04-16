#include <glm/glm.hpp>
#include "physics_simulator.hpp"

void test(){
    /* draw_grid_3d(5.0f,20.0f); */
    draw_grid_2d(.25f,20.0f,-3.0f);
    Object *obj1, *obj2, *obj3, *obj4, *obj5;
    obj1 = draw_sphere(.1,.1,.1, 3000,glm::vec3(.01,.01,.03), .7);
    obj2 = draw_sphere(.1,2.0,.1, 2000,glm::vec3(.01,-.01,.01), .7);
    obj3 = draw_sphere(.1,.1,2.0, 4000,glm::vec3(-.01,.02,.01), .7);
    obj4 = draw_sphere(.1,.1,.2, 4000,glm::vec3(-.01,.02,.01), .1);
    obj5 = draw_sphere(.1,.1,-1.0, 4000,glm::vec3(-.01,.02,.01), .3);
    obj1->set_trajectory(true);
    obj2->set_trajectory(true);
    obj3->set_trajectory(true);
    obj4->set_trajectory(true);
    obj5->set_trajectory(true);
}
