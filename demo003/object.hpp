#ifndef __OBJECT_H
#define __OBJECT_H

#include <cmath>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <sys/types.h>
#include <tuple>
#include <vector>

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
};

struct Triangle_Link{
    GLuint a,b,c;
};

extern std::vector<Vertex> global_vertices;
extern std::vector<Triangle_Link> global_indices;

class Object{
    private:
        float x,y,z;
        std::vector<size_t> vertices;
        std::vector<size_t> indices;
        bool show_trajectory;
    public:
        float m = 0;
        glm::vec3 v;

        Object(float x,float y, float z, float m, glm::vec3 v) :x(x), y(y),z(z),m(m), v(v) {};

        void push_triangles(std::vector<Vertex>& vertices, std::vector<Triangle_Link>& indices);
        void move(float dx,float dy, float dz);
        void move(glm::vec3 v);
        float distance(Object& obj);
        float dx(Object& obj) {return obj.x - this->x;};
        float dy(Object& obj) {return obj.y - this->y;};
        float dz(Object& obj) {return obj.z - this->z;};
        std::tuple<float, float, float> get_position();
        void set_trajectory(bool show);
        bool get_trajectory();
};

class Cube: public Object{
    private:
        float z_side_length, y_side_length, x_side_length;
    public:
        Cube(float x,float y, float z, float m, glm::vec3 v) : Object(x, y, z, m, v) {};
};

class Sphere: public Object{
    private:
        float r;
    public:
        Sphere(float x, float y, float z, float m, glm::vec3 v, float r) : Object(x, y, z, m, v), r(r) {};
};

class Line: public Object{
    private:
        float x2,y2,z2;
    public:
        Line(float x1, float y1, float z1, float x2, float y2, float z2) : Object(x1,y1,z1,0,glm::vec3(0,0,0)), x2(x2), y2(y2), z2(z2) {};
};

class Grid: public Object{
    private:
        std::vector<Line*> lines;
    public:
        Grid() :Object(0,0,0,0,glm::vec3(0,0,0)) {};
        void push_lines(std::vector<Line*> lines){
            this->lines.insert(this->lines.begin(),lines.begin(),lines.end());
        }
        void transform(float x, float y, float z, float coord);
};

Object* draw_sphere(float x,float y,float z, float m, glm::vec3 v, float r);
Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float side_length);
Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float z_side_length,float x_side_length,float y_side_length);
Object* draw_line(float x1, float y1, float z1, float x2, float y2, float z2);
Object* draw_grid(float granularity, float range);

#endif
