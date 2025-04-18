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
        void move(float dx, float dy, float dz);
        void move(glm::vec3 v);
        float distance(Object& obj);
        float dx(Object& obj) {return obj.x - this->x;};
        float dy(Object& obj) {return obj.y - this->y;};
        float dz(Object& obj) {return obj.z - this->z;};
        std::tuple<float, float, float> get_position();
        void set_trajectory(bool show);
        bool get_trajectory();
        float get_x(){return this->x;};
        float get_y(){return this->y;};
        float get_z(){return this->z;};
        std::vector<size_t>& get_vertices() {return this->vertices;};
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
        float dx1 = 0, dy1 = 0, dz1 = 0;
        float dx2 = 0, dy2 = 0, dz2 = 0;
        float x2, y2, z2;
    public:
        Line(float x1, float y1, float z1, float x2, float y2, float z2) : Object(x1,y1,z1,0,glm::vec3(0,0,0)), x2(x2), y2(y2), z2(z2) {};
        std::tuple<float, float, float> get_position_dst();
        void move_src(float dx, float dy, float dz);
        void move_dst(float dx, float dy, float dz);
};

class Grid3D: public Object{
    private:
        std::vector<Line*> lines;
    public:
        Grid3D() :Object(0,0,0,0,glm::vec3(0,0,0)) {};
        void push_lines(std::vector<Line*> lines){
            this->lines.insert(this->lines.begin(),lines.begin(),lines.end());
        }
        void transform(float x, float y, float z, float m);
};

class Grid2D: public Object{
    private:
        std::vector<Line*> lines;
    public:
        Grid2D() :Object(0,0,0,0,glm::vec3(0,0,0)) {};
        void push_lines(std::vector<Line*> lines){
            this->lines.insert(this->lines.begin(),lines.begin(),lines.end());
        }
        void transform(float x, float z, float m);
};

Object* draw_sphere(float x,float y,float z, float m, glm::vec3 v, float r);
Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float side_length);
Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float z_side_length,float x_side_length,float y_side_length);
Object* draw_line(float x1, float y1, float z1, float x2, float y2, float z2);
Object* draw_grid_3d(float granularity, float range);
Object* draw_grid_2d(float granularity, float range, float y);

#endif
