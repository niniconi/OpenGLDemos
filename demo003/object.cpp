#include "object.hpp"
#include <GL/gl.h>
#include <cmath>
#include <initializer_list>
#include <tuple>
#include <utility>
#include <vector>

#include "physics_simulator.hpp"

extern std::vector<Vertex> global_vertices;
extern std::vector<Triangle_Link> global_indices;
extern std::vector<Object*> global_objects;

void Object::push_triangles(std::vector<Vertex>& vertices, std::vector<Triangle_Link>& indices){
    uint offset_of_vertices = global_vertices.size();
    for (auto t : vertices) {
        t.position.x += this->x;
        t.position.y += this->y;
        t.position.z += this->z;
        global_vertices.push_back(t);
        this->vertices.push_back(global_vertices.size() - 1);
    }
    for(auto t: indices){
        t.a += offset_of_vertices;
        t.b += offset_of_vertices;
        t.c += offset_of_vertices;
        global_indices.push_back(t);
        this->indices.push_back(global_vertices.size() - 1);
    }
}

std::tuple<float, float, float> Object::get_position(){
    return std::tuple(x,y,z);
}

void Object::set_trajectory(bool show){
    this->show_trajectory = show;
    if(show){
        // TODO
        // 将已经存在的轨迹线放入 global_objects
    }else{
        // TODO
        // 将已经存在的轨迹线从 global_objects 移除
    }
}

bool Object::get_trajectory(){
    return this->show_trajectory;
}

void Object::move(glm::vec3 v){
    move(v.x,v.y,v.z);
}

void Object::move(float dx,float dy, float dz){
    this->x+=dx;
    this->y+=dy;
    this->z+=dz;
    for (auto t: this->vertices) {
        global_vertices[t].position.x+=dx;
        global_vertices[t].position.y+=dy;
        global_vertices[t].position.z+=dz;
    }
}

float Object::distance(Object& obj){
    float dx = obj.x - this->x;
    float dy = obj.y - this->y;
    float dz = obj.z - this->z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

static std::pair<std::vector<Vertex>,std::vector<Triangle_Link>> build_sphere(float r){
    float dx = 0.1f,i,j;
    GLuint k = 0;
    GLuint line_size = 2*M_PI / dx + 1;
    r=r>0?r:-r;
    std::vector<Vertex> vertices;
    std::vector<Triangle_Link> indices;
    for(i=-M_PI/2;i<M_PI/2;i+=dx){
        for(j=0;j<2*M_PI;j+=dx){
            vertices.push_back({
                    {r*cos(i)*cos(j), r*sin(i), r*cos(i)*sin(j)},
                    {0.0f,0.0f,0.0f}});
        }
    }
    for(i=M_PI/2,j=0;j<2*M_PI;j+=dx){
        vertices.push_back({
                {r*cos(i)*cos(j), r*sin(i), r*cos(i)*sin(j)},
                {0.0f,0.0f,0.0f}});
    }

    for(k=0;k+1+line_size<vertices.size();k++){
        indices.push_back({k,k+line_size,k+1});
        indices.push_back({k+1,k+1+line_size,k+line_size});
    }
    return std::make_pair(vertices,indices);
}

Object* draw_line(float x1, float y1, float z1, float x2, float y2, float z2){
    Line* line = new Line(x1,y1,z1,x2,y2,z2);
    std::vector<Vertex> vertices = {
        {{0,0,0},{}},
        {{x2-x1,y2-y1,z2-z1},{}},
        {{x2-x1,y2-y1,z2-z1},{}}
    };
    std::vector<Triangle_Link> indices = {{0,1,2}};
    line->push_triangles(vertices,indices);
    line->set_trajectory(false);
    return line;
}

Object* draw_sphere(float x,float y,float z, float m, glm::vec3 v, float r){
    Sphere* cirl = new Sphere(x,y,z,m,v,r);
    std::pair<std::vector<Vertex>, std::vector<Triangle_Link>> model = build_sphere(r);
    cirl->push_triangles(model.first,model.second);
    global_objects.push_back(static_cast<Object*>(cirl));
    return cirl;
}

Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float side_length){
    Cube* cube = new Cube(x,y,z,m,v);
    std::vector<Vertex> vertices = {
        {{-0.5f*side_length, -0.5f*side_length, -0.75f*side_length},{}}, // 顶点 1
        {{0.5f*side_length, -0.5f*side_length, -0.5f*side_length},{}}, // 顶点 2
        {{0.5f*side_length,  0.5f*side_length, -0.5f*side_length},{}}, // 顶点 3
        {{-0.5f*side_length,  0.5f*side_length, -0.5f*side_length},{}}, // 顶点 4
        {{-0.5f*side_length, -0.5f*side_length,  0.5f*side_length},{}}, // 顶点 5
        {{0.5f*side_length, -0.5f*side_length,  0.5f*side_length},{}}, // 顶点 6
        {{0.5f*side_length,  0.5f*side_length,  0.5f*side_length},{}}, // 顶点 7
        {{-0.5f*side_length,  0.5f*side_length,  0.5f*side_length},{}}  // 顶点 8
    };
    std::vector<Triangle_Link> indices = {
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
    cube->push_triangles(vertices,indices);
    return cube;
}
Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float z_side_length, float x_side_length, float y_side_length){
    return nullptr;
}
