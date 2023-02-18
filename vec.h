#pragma once

class Vec2f
{
public:
    Vec2f() : x(0), y(0) {}
    Vec2f(float xx) : x(xx), y(xx) {}
    Vec2f(float xx, float yy) : x(xx), y(yy) {}
    Vec2f operator * (const float& r) const { return Vec2f(x * r, y * r); }
    Vec2f operator / (const float& r) const { return Vec2f(x / r, y / r); }
    Vec2f operator + (const Vec2f& v) const { return Vec2f(x + v.x, y + v.y); }
    Vec2f operator - (const Vec2f& v) const { return Vec2f(x - v.x, y - v.y); }
    float x, y;
};

class Vec3f {
public:
    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float xx) : x(xx), y(xx), z(xx) {}
    Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vec3f operator * (const float& r) const { return Vec3f(x * r, y * r, z * r); }
    Vec3f operator * (const Vec3f& v) const { return Vec3f(x * v.x, y * v.y, z * v.z); }
    Vec3f operator + (const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
    Vec3f operator - (const Vec3f& v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
    Vec3f operator - () const { return Vec3f(-x, -y, -z); }
    void operator = (const Vec3f& v) { x = v.x, y = v.y, z = v.z; }
    Vec3f operator += (const Vec3f& v) { x += v.x, y += v.y, z += v.z; return *this; }
    void output(ofstream* myfile) const { *myfile << x << " " << y << " " << z << "\n"; };
    void output_uint8(ofstream* myfile) const { *myfile << round(x) << " " << round(y) << " " << round(z) << "\n"; };
    void print() const { cout << x << " " << y << " " << z << "\n"; };

    Vec3f normalize()
    {
        float mag2 = x * x + y * y + z * z;
        float invMag = 1 / sqrtf(mag2);
        return Vec3f(x * invMag, y * invMag, z * invMag);
    }

    float dotProduct(const Vec3f& b)
    {
        return x * b.x + y * b.y + z * b.z;
    }

    Vec3f crossProduct(const Vec3f& b)
    {
        return Vec3f(
            y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x
        );
    }

    float square()
    {
        return x * x + y * y + z * z;
    }

    Vec3f length(const Vec3f& a)
    {
        return sqrt(x * x + y * y + z * z);
    }

float x, y, z;
};
