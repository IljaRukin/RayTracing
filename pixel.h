#pragma once

class color {
public:

    float x, y, z;

    color() : x(0), y(0), z(0) {}
    color(float xx) : x(xx), y(xx), z(xx) {}
    color(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    color operator * (const float& r) const { return color(x * r, y * r, z * r); }
    color operator * (const color& v) const { return color(x * v.x, y * v.y, z * v.z); }
    color operator - (const color& v) const { return color(x - v.x, y - v.y, z - v.z); }
    color operator + (const color& v) const { return color(x + v.x, y + v.y, z + v.z); }
    void output(ofstream* myfile) const { *myfile << x << " " << y << " " << z << "\n"; };
    void output_uint8(ofstream* myfile) const { *myfile << round(x) << " " << round(y) << " " << round(z) << "\n"; };
    void print() const { cout << x << " " << y << " " << z << "\n"; };
    float maximum() const { return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z); };

    void render_pixel(triangle_mesh model, ray r)
    {

        r.loop_triangles(model);

        if (r.hit_t > t_max) { x = 0; y = 0; z = 0; }//if object to far away
        else
        {

            color ambient; color diffuse; color specular;

            unsigned i = model.shapes[r.hit_shape].mesh.material_ids[r.hit_face];//material_index

            //cout << tilt_angle << endl;

            //diffuse
            //x = model.materials[i].diffuse[0] * tilt_angle;
            //y = model.materials[i].diffuse[1] * tilt_angle;
            //z = model.materials[i].diffuse[2] * tilt_angle;

            //ambient + diffuse * tilt_angle + specular
            x = model.materials[i].ambient[0];
            y = model.materials[i].ambient[1];
            z = model.materials[i].ambient[2];

            float tilt_angle = abs(r.hit_normal.dotProduct(r.ray_direction));
            //diffuse cos(tilt_angle)
            x += model.materials[i].diffuse[0] * tilt_angle;
            y += model.materials[i].diffuse[1] * tilt_angle;
            z += model.materials[i].diffuse[2] * tilt_angle;

            //specular cos(reflection)
            //Vec3f light_ray = Vec3f(-2, -2, 10);
            //Vec3f reflection = ( r.hit_normal * 2 * r.hit_normal.dotProduct(light_ray) - light_ray ).normalize();
            //float light_angle = abs(r.hit_normal.dotProduct(reflection));

            //x += model.materials[i].specular[0] * light_angle;
            //y += model.materials[i].specular[1] * light_angle;
            //z += model.materials[i].specular[2] * light_angle;

        }
    }
};
