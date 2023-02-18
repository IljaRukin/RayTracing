#pragma once
#include "vec.h"

struct triangle {
    Vec3f vetecies[3];
    //vec3f normals[3];
    //vec2f texture[3];
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    int exponent;
};