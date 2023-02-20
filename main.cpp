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
	// left_top [0,0]                                [0,width-1]
	// ^
	// |
	// |
	// left_bottom [height-1,0] ---> right_bottom [height-1,width-1]
	
	// note: main view direction is Vec3f(0,0,1)
	auto rays = new ray[height][width];
	float theta_y = -10 * deg2rad; //rotate around y-axis (horizontal)
	float theta_x = -10 * deg2rad; //rotate around x-axis (vertical)
	Mat3 rotX = rotXmat(theta_x);
	Mat3 rotY = rotYmat(theta_y);

	Vec3f left_top = rotY.matProduct(rotX.matProduct( Vec3f(225-450, 225+100+450, -100) ) );
	Vec3f left_bottom = rotY.matProduct(rotX.matProduct( Vec3f(225-450, 225+100-450, -100) ) );
	Vec3f right_bottom = rotY.matProduct(rotX.matProduct( Vec3f(225+450, 225+100-450, -100) ) );

	// Perspective Projection
	//Vec3f origin = Vec3f(225, 225, -800);
	//generate_rays_fisheye(origin, left_bottom, left_top, right_bottom, rays);

	// Orthographic Projection
	//Vec3f direction = rotY.matProduct(rotX.matProduct(Vec3f(0, 0, 1)));
	Vec3f direction = -(left_top - left_bottom).crossProduct(right_bottom - left_bottom).normalize(); direction.print();
    generate_rays_parallel(direction, left_bottom, left_top, right_bottom, rays);

	/*
	//---------- pinhole array = approximation of lens array without depth blurr ----------
	//note: all coordinates are measured in units of the LCD pixel pitch
	//lcd parameters
	Vec2f left_top = Vec2f( 225-width/2 , 225+height/2-1 );
	float z_display = -50;
	float z_pinhole = -30;

	//translation vectors - hexagonal lenses
	float a = 50; //mm
	Vec2f t1 = Vec2f(a, 0); //mm
	Vec2f t2 = Vec2f(a*cos(2*PI/6), a*sin(2*PI/6)); //mm

	//translation vectors - rectangular lenses
	//Vec2f t1 = Vec2f( 7 , 0 ); //mm
	//Vec2f t2 = Vec2f( 0 , 5.4 ); //mm

	//ray generation
	auto rays = new ray[height][width];
	generate_rays_pinhole_array(z_display, z_pinhole, left_top, t1, t2, rays);
	*/

    //---------- render image ----------
    auto image = new color[height][width];
    //when using multiple models, just render all models one after another in no particular order
    //render_image(0, width, 0, height, image, rays, model1);
    render_image_multithreaded(image, rays, model1);
    delete[] rays;

    //---------- save image ----------
	float min_value = minimum_value(image);
	float max_value = maximum_value(image);
	cout << "min intensity: " << min_value << endl;
	cout << "max intensity: " << max_value << endl;
    save_img(image, -min_value, 255/max_value, "output.ppm");
    delete[] image;

    return 0;
}