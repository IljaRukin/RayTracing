#pragma once

struct ray
{
public:

    Vec3f ray_origin;
    Vec3f ray_direction;
    float hit_t;
    Vec3f hit_normal;
    unsigned hit_shape;
    unsigned hit_face;

    ray() : hit_t(infinity), hit_shape(0), hit_face(0), hit_normal(Vec3f(0, 0, 0)) {}
    ray(const Vec3f a, const Vec3f b) : hit_t(infinity), hit_shape(0), hit_face(0), ray_origin(a), ray_direction(b), hit_normal(Vec3f(0, 0, 0)) {}

    void set_origin(const Vec3f o) { ray_origin = o; }
    void set_direction(const Vec3f d) { ray_direction = d; }
    void set_hit_t(float a) { hit_t = a; }
    void set_hit_normal(Vec3f n) { hit_normal = n; }
    void set_hit_shape(unsigned a) { hit_shape = a; }
    void set_hit_face(unsigned a) { hit_face = a; }

    Vec3f get_origin() const { return ray_origin; }
    Vec3f get_direction() const { return ray_direction; }
    Vec3f point_at_parameter(float t) const { return ray_origin + ray_direction * t; }

    void loop_triangles(triangle_mesh model)
    {
        unsigned idx0; unsigned idx1; unsigned idx2;
        Vec3f V0; Vec3f V1; Vec3f V2;

        //for each shape
        unsigned shape_number = model.shapes.size();
        for (unsigned i = 0; i < shape_number; i++) {
            //for each face
            unsigned face_number = model.shapes[i].mesh.num_face_vertices.size();
            //cout << "shape: " << i << endl;
            //cout << "number faces: " << face_number << endl;
            for (unsigned f = 0; f < face_number; f++) {
                //cout << "face: " << f << endl;
                //if (model.shapes[i].mesh.num_face_vertices.size() != 3) { cout << "error: shapes are not triangles !" << endl; }

                // 3 verticies
                idx0 = model.shapes[i].mesh.indices[3 * f].vertex_index;
                idx1 = model.shapes[i].mesh.indices[3 * f + 1].vertex_index;
                idx2 = model.shapes[i].mesh.indices[3 * f + 2].vertex_index;
                V0 = Vec3f(model.attrib.vertices[3*idx0], model.attrib.vertices[3 * idx0 + 1], model.attrib.vertices[3 * idx0 + 2]);
                V1 = Vec3f(model.attrib.vertices[3*idx1], model.attrib.vertices[3 * idx1 + 1], model.attrib.vertices[3 * idx1 + 2]);
                V2 = Vec3f(model.attrib.vertices[3*idx2], model.attrib.vertices[3 * idx2 + 1], model.attrib.vertices[3 * idx2 + 2]);
                //cout << idx0 << idx1 << idx2 << endl;
                //V0.print();
                //V1.print();
                //V2.print();
                //cout << endl;
                //hit_parameters = calc_hit(hit_parameters, V0, V1, V2, r);

                //cout << "ray_direction: ";  r.ray_direction.print();
                Vec3f E1 = V1 - V0;
                Vec3f E2 = V2 - V0;
                //E1.print();
                //E2.print();
                Vec3f P = ray_direction.crossProduct(E2);
                float a = P.dotProduct(E1);
                if (a > -epsillon && a < epsillon) { continue; }//if allmost parallel (determinant ~ 0)
                float c = 1 / a;
                Vec3f T = ray_origin - V0;
                float u = c * P.dotProduct(T);
                //cout << "u: " << u << endl;
                if (u < 0 || u > 1) { continue; }//outside of intervall
                Vec3f Q = T.crossProduct(E1);
                float v = c * Q.dotProduct(ray_direction);
                //cout << "v: " << v << endl;
                if (v < 0 || v > 1) { continue; }//outside of intervall
                float t = c * E2.dotProduct(Q);
                //cout << "t: " << t << endl;
                if (t > t_max) { continue; }//if to far away
                if ((u + v) <= 1)//if hit inside triangle
                {
                    //cout << "hit" << endl;
                    if (t < hit_t)//if hit has smallest distance
                    {
                        hit_t = t;
                        //cout << "set_hit: " << hit_t << endl;
                        //r.hit_point = r.point_at_parameter(t);
                        hit_normal = E1.crossProduct(E2).normalize();
                        //hit_normal.print();
                        //hit_parameters.model = &model;
                        hit_shape = i;
                        hit_face = f;
                        //hit_parameters.index_verticies[0] = idx0; hit_parameters.index_verticies[1] = idx1; hit_parameters.index_verticies[2] = idx2;
                        //tidx0 = model.shapes[i].mesh.indices[3 * f].texcoord_index;
                        //tidx1 = model.shapes[i].mesh.indices[3 * f + 1].texcoord_index;
                        //tidx2 = model.shapes[i].mesh.indices[3 * f + 2].texcoord_index;
                        //hit_parameters.index_texture[0] = tidx0; hit_parameters.index_texture[1] = tidx1; hit_parameters.index_texture[2] = tidx2;
                        //hit_parameters.material_id = model.shapes[i].mesh.material_ids[f];
                    }
                }


            }
        }
    }
};

void generate_rays_fisheye(Vec3f origin, float fov_horizontal[], float fov_vertical[], ray rays[height][width])
{
    float angle_vertical = fov_vertical[0];
    float angle_step_vertical = (fov_vertical[2] - fov_vertical[0]) / (height - 1);
    float angle_horizontal = fov_horizontal[0];
    float angle_step_horizontal = (fov_horizontal[0] - fov_horizontal[2]) / (width - 1);
    //cout << "step horizontal: " << angle_step_horizontal << " step vertical: " << angle_step_vertical << endl;
    float sin_vertical;
    float cos_vertical;

    for (unsigned row = 0; row < height; row++)
    {
        float angle_horizontal = fov_horizontal[0];
        sin_vertical = sin(angle_vertical * deg2rad);
        cos_vertical = cos(angle_vertical * deg2rad);
        for (unsigned column = 0; column < width; column++)
        {
            rays[row][column].ray_origin = origin;
            //Vec3f dir = Vec3f(sin(angle_horizontal * deg2rad) * cos(angle_vertical * deg2rad), sin(angle_horizontal * deg2rad) * sin(angle_vertical * deg2rad), cos(angle_horizontal * deg2rad));
            Vec3f dir = Vec3f(sin_vertical * cos(angle_horizontal * deg2rad), sin_vertical * sin(angle_horizontal * deg2rad), cos_vertical);
            rays[row][column].ray_direction = dir;
            //dir.print();
            //cout << " angle horizontal: " << angle_horizontal << " angle vertical: " << angle_vertical << endl;
            angle_horizontal -= angle_step_horizontal;
        }
        angle_vertical += angle_step_vertical;
    }

}

void generate_rays_parallel(Vec3f origin, float phi, float theta, float range_horizontal[], float range_vertical[], ray rays[height][width])
{
    Vec3f direction = Vec3f(sin(theta * deg2rad) * cos(phi * deg2rad), sin(theta * deg2rad) * sin(phi * deg2rad), cos(theta * deg2rad));
    Vec3f vec1 = Vec3f(cos(theta *deg2rad)* cos(phi *deg2rad), cos(theta *deg2rad) *sin(phi *deg2rad), -sin(theta *deg2rad)); //(cos(theta)*cos(phi),cos(theta)*sin(phi),-sin(theta))
    Vec3f vec2 = Vec3f(-sin(phi *deg2rad), cos(phi *deg2rad), 0);//(-sin(phi),cos(phi),0)
    cout << "direction: ";
    direction.print();
    cout << "origin: ";
    origin.print();
    cout << "vec1: ";
    vec1.print();
    cout << "vec2: ";
    vec2.print();
    float step_vec2 = (range_vertical[0] - range_vertical[2]) / (height - 1);
    float step_vec1 = (range_horizontal[0] - range_horizontal[2]) / (width - 1);
    Vec3f edge = origin + vec2 *range_horizontal[0] - vec1 * range_vertical[0];

    for (unsigned row = 0; row < height; row++)
    {
        float pixel_horizontal = range_horizontal[0];
        for (unsigned column = 0; column < width; column++)
        {
            rays[row][column].ray_origin = edge - vec2 * step_vec2*row + vec1* step_vec1* column;
            rays[row][column].ray_direction = direction;
            //rays[row][column].ray_origin.print();
        }
    }

}

void generate_rays_lensarray()
{

}