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

    //---------- camera view fisheye ----------
    /*
	//view for triangle & cube
    Vec3f origin = Vec3f(-1, -2, 1);
    float fov_horizontal[3] = { 90,30,-30 };
    float fov_vertical[3] = { 70,110,150 };
    //float fov_vertical[3];
    //for (uint8_t k = 0; k < 3; k++) { fov_vertical[k] = (fov_horizontal[k] - fov_horizontal[1]) / width * height + 70; }
    
    cout << "fov horizontal: " << fov_horizontal[0] << " " << fov_horizontal[1] << " " << fov_horizontal[2] << endl;
    cout << "fov vertical: " << fov_vertical[0] << " " << fov_vertical[1] << " " << fov_vertical[2] << endl;

    //generate rays from camera view
    //ray rays[height][width];
    auto rays = new ray[height][width];
    generate_rays_fisheye(origin, fov_horizontal, fov_vertical, rays);
    */

    //---------- camera view parallel ----------

	/*
	//view for triangle & cube
	Vec3f origin = Vec3f(300, 270, -600);
	float rotation_horizontal = 10;
	float rotation_vertical = 0;
	float range_horizontal[3] = { -400,0,400 };
	float range_vertical[3] = { -400,0,400 };
	*/

	/*
    //view for triangle & cube
    Vec3f origin = Vec3f(0, -1, 0);
    float rotation_horizontal = 110;
    float rotation_vertical = 110;
    float range_horizontal[3] = { -2,0,2 };
    float range_vertical[3] = { -2,0,2 };
	*/

	//view for cornell_box
	Vec3f origin = Vec3f(450, 300, -600);
	float rotation_horizontal = -10;
	float rotation_vertical = 10;
	float range_horizontal[3] = { -400,0,400 };
	float range_vertical[3] = { -400,0,400 };

    auto rays = new ray[height][width];
    generate_rays_parallel(origin, rotation_vertical, rotation_horizontal, range_horizontal, range_vertical, rays);

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