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

void generate_rays_fisheye(Vec3f origin, Vec3f left_bottom, Vec3f left_top, Vec3f right_bottom, ray rays[height][width])
{
	for (unsigned row = 0; row < height; row++)
	{
		for (unsigned column = 0; column < width; column++)
		{
			rays[row][column].ray_origin = origin;
			rays[row][column].ray_direction = (left_top - (left_top - left_bottom) * ((float)row / (height-1)) + (right_bottom - left_bottom) * ((float)column / (width-1))) - origin;
			rays[row][column].ray_direction.normalize();
		}
	}
}

void generate_rays_parallel(Vec3f direction, Vec3f left_bottom, Vec3f left_top, Vec3f right_bottom, ray rays[height][width])
{
	direction = direction.normalize();
	for (unsigned row = 0; row < height; row++)
	{
		for (unsigned column = 0; column < width; column++)
		{
			rays[row][column].ray_origin = left_top - (left_top - left_bottom) * ((float)row / (height - 1)) + (right_bottom - left_bottom) * ((float)column / (width - 1));
			rays[row][column].ray_direction = direction;
		}
	}
}

void generate_rays_lensarray()
{

}