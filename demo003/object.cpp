#include "object.hpp"
#include <GL/gl.h>
#include <cmath>
#include <tuple>
#include <utility>
#include <vector>

#include "physics_simulator.hpp"

extern std::vector<Vertex> global_vertices;
extern std::vector<Triangle_Link> global_indices;
extern std::vector<Object*> global_objects;
extern Grid2D* global_grid2d;

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

void Line::move_src(float dx, float dy, float dz){
    auto t = this->get_vertices()[0];
    global_vertices[t].position.x += dx - this->dx1;
    global_vertices[t].position.y += dy - this->dy1;
    global_vertices[t].position.z += dz - this->dz1;
    this->dx1 = dx;
    this->dy1 = dy;
    this->dz1 = dz;
}

void Line::move_dst(float dx, float dy, float dz){
    auto t = this->get_vertices()[1];
    auto x = this->get_vertices()[2];
    global_vertices[t].position.x += dx - this->dx2;
    global_vertices[t].position.y += dy - this->dy2;
    global_vertices[t].position.z += dz - this->dz2;
    global_vertices[x].position.x += dx - this->dx2;
    global_vertices[x].position.y += dy - this->dy2;
    global_vertices[x].position.z += dz - this->dz2;
    this->dx2 = dx;
    this->dy2 = dy;
    this->dz2 = dz;
}

std::tuple<float, float, float> Line::get_position_dst(){
    return std::tuple(x2,y2,z2);
}


void Grid2D::transform(float x, float z, float m){
    if(m <= 0.0f) return;
    for(auto l:this->lines){
        auto [x1, y1, z1] = l->get_position();
        auto [x2, y2, z2] = l->get_position_dst();
        float p1 = -std::pow(m,-std::abs((x1-x)*(x1-x)+(z1-z)*(z1-z))/(m/1000));
        float p2 = -std::pow(m,-std::abs((x2-x)*(x2-x)+(z2-z)*(z2-z))/(m/1000));
        l->move_src(0, p1, 0);
        l->move_dst(0, p2, 0);
    }
}

void Grid3D::transform(float x, float y, float z, float m){

}

static std::pair<std::vector<Vertex>,std::vector<Triangle_Link>> build_sphere(float r) {
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

static float distance(Vertex& a, Vertex& b){
    float dx = a.position.x - b.position.x;
    float dy = a.position.y - b.position.y;
    float dz = a.position.z - b.position.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

Object* draw_line(Vertex& src, Vertex& dst){
    return draw_line(src.position.x, src.position.y, src.position.z, dst.position.x, dst.position.y, dst.position.z);
}

Object* draw_line(float x1, float y1, float z1, float x2, float y2, float z2) {
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

Object* draw_sphere(float x,float y,float z, float m, glm::vec3 v, float r) {
    Sphere* cirl = new Sphere(x,y,z,m,v,r);
    std::pair<std::vector<Vertex>, std::vector<Triangle_Link>> model = build_sphere(r);
    cirl->push_triangles(model.first,model.second);
    global_objects.push_back(static_cast<Object*>(cirl));
    return cirl;
}

Object* draw_cube(float x, float y, float z, float m, glm::vec3 v, float side_length) {
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

Object* draw_grid_2d(float granularity, float range, float y){
    std::vector<Vertex> vertices;
    std::vector<Line*> lines;

    float x, z;
    float offset = range - ((int)(range / granularity))*granularity;
    size_t line_size = 2*(int)(range / granularity) + 1;
    size_t i;
    Grid2D* grid2d = new Grid2D();

    for(x = -range + offset;x<=range;x+=granularity){
        for(z = -range + offset;z<=range;z+=granularity){
            vertices.push_back({{x,y,z}});
        }
    }

    for(i=0;i<vertices.size();i++){
        if(i+1 < vertices.size() && (i % line_size)+1 < line_size)
            lines.push_back(static_cast<Line*>(draw_line(vertices[i],vertices[i+1])));
        if(i+line_size < vertices.size())
            lines.push_back(static_cast<Line*>(draw_line(vertices[i],vertices[i+line_size])));
    }

    grid2d->push_lines(lines);
    global_grid2d = grid2d;
    return grid2d;
}

Object* draw_grid_3d(float granularity, float range){
    std::vector<Vertex> vertices;
    std::vector<Line*> lines;

    float x, y, z;
    float offset = range - ((int)(range / granularity))*granularity;
    size_t i;
    size_t line_size = 2*(int)(range / granularity) + 1;
    Grid3D* grid3d = new Grid3D();

    for(x=-range + offset;x<=range;x+=granularity){
        for(y=-range + offset;y<=range;y+=granularity){
            for(z=-range + offset;z<=range;z+=granularity){
                vertices.push_back({{x,y,z}});
            }
        }
    }

    for(i=0;i<vertices.size();i++){
        if(i+1 < vertices.size() && (i % line_size )+1 < line_size)
            lines.push_back(static_cast<Line*>(draw_line(vertices[i],vertices[i+1])));
        if(i+line_size < vertices.size() && (i % (line_size * line_size)) / line_size + 1 < line_size)
            lines.push_back(static_cast<Line*>(draw_line(vertices[i],vertices[i+line_size])));
        if(i+line_size*line_size < vertices.size())
            lines.push_back(static_cast<Line*>(draw_line(vertices[i],vertices[i+line_size*line_size])));
    }
    grid3d->push_lines(lines);
    return grid3d;
}
