#include <cstdio>
#include <algorithm> 
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <stdlib.h>
#include <chrono>
#include <windows.h>
using namespace std;

#define height 1000
#define width 1000
#define PI 3.14159265358979323846
#define epsillon 1e-8
#define t_max 1e+8
#define deg2rad PI/180
#define rad2deg 180/P
static const float infinity = (std::numeric_limits<float>::max)();

#include "obj_loader.h"
#include "triangles.h"
#include "vec.h"
#include "ray.h"
#include "pixel.h"
#include "render_loop.h"
#include "save.h"

int main(void)
{
    //---------- import 3D model ----------
    char basepath[] = "./models";
    //char filename[] = "./models/triangle.obj";
    //char filename[] = "./models/cube.obj";
    char filename[] = "./models/cornell_box.obj";
    triangle_mesh model1;
    model1 = import_obj(model1, basepath, filename);
    //PrintInfo(model1.attrib, model1.shapes, model1.materials);

	//---------- view & ray generation ----------
	/*
	left_top [0,0]                                [0,width-1]
	^
	|
	|
	left_bottom [height-1,0] ---> right_bottom [height-1,width-1]
	*/
	// note: main view direction is Vec3f(0,0,1)
	auto rays = new ray[height][width];
	float theta_y = -10 * deg2rad; //rotate around y-axis (horizontal)
	float theta_x = -10 * deg2rad; //rotate around x-axis (vertical)
	Mat3 rotX = rotXmat(theta_x); rotX.print();
	Mat3 rotY = rotYmat(theta_y); rotY.print();

	Vec3f rotation = Vec3f(sin(theta_y) * cos(theta_x), sin(theta_y) * sin(theta_x), cos(theta_y));
	Vec3f left_top = rotY.matProduct(rotX.matProduct( Vec3f(225-450, 225+450, -100) ) );
	Vec3f left_bottom = rotY.matProduct(rotX.matProduct( Vec3f(225-450, 0, -100) ) );
	Vec3f right_bottom = rotY.matProduct(rotX.matProduct( Vec3f(225+450, 0, -100) ) );

	/*
	// Perspective Projection
	Vec3f origin = Vec3f(225, 225, -800);
	generate_rays_fisheye(origin, left_bottom, left_top, right_bottom, rays);
	*/

	// Orthographic Projection
	Vec3f direction = rotY.matProduct(rotX.matProduct(Vec3f(0, 0, 1)));
	//Vec3f direction = -(left_top - left_bottom).crossProduct(right_bottom - left_bottom).normalize();
	direction.print();
    generate_rays_parallel(direction, left_bottom, left_top, right_bottom, rays);
	
    //---------- render image ----------
    auto image = new color[height][width];
    //when using multiple models, just render all models one after another in no particular order
    //render_image(0, width, 0, height, image, rays, model1);
    render_image_multithreaded(image, rays, model1);
    delete[] rays;

    //---------- save image ----------
    float max_value = maximum_value(image);
    cout << "max intensity: " << max_value << endl;
    save_img(image, 255/max_value,"output.ppm");
    delete[] image;

    return 0;
}