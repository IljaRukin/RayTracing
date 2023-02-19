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

/*
class Mat3
{
public:
	float mat3[3][3];
	Mat3() : mat3() {}
	{
		mat3[0][0] = 0, mat3[1][0] = 0, mat3[2][0] = 0,
			mat3[0][1] = 0, mat3[1][1] = 0, mat3[2][1] = 0,
			mat3[0][2] = 0, mat3[1][2] = 0, mat3[2][2] = 0;
	};
	Mat3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		mat3[0][0] = m00, mat3[1][0] = m01, mat3[2][0] = m02,
			mat3[0][1] = m10, mat3[1][1] = m11, mat3[2][1] = m12,
			mat3[0][2] = m20, mat3[1][2] = m21, mat3[2][2] = m22;
	};
	Mat3 operator * (const float& r) const {
		return Mat3(
			mat3[0][0] * r, mat3[1][0] * r, mat3[2][0] * r,
			mat3[0][1] * r, mat3[1][1] * r, mat3[2][1] * r,
			mat3[0][2] * r, mat3[1][2] * r, mat3[2][2] * r);
	}
	Mat3 operator * (const Mat3& arr) const {
		return Mat3(
			mat3[0][0] * arr.mat3[0][0], mat3[1][0] * arr.mat3[1][0], mat3[2][0] * arr.mat3[2][0],
			mat3[0][1] * arr.mat3[0][1], mat3[1][1] * arr.mat3[1][1], mat3[2][1] * arr.mat3[2][1],
			mat3[0][2] * arr.mat3[0][2], mat3[1][2] * arr.mat3[1][2], mat3[2][2] * arr.mat3[2][2]);
	}
	Vec3f matProduct(const Vec3f& vec)
	{
		return Vec3f(
			mat3[0][0] * vec.x + mat3[1][0] * vec.y + mat3[2][0] * vec.z,
			mat3[0][1] * vec.x + mat3[1][1] * vec.y + mat3[2][1] * vec.z,
			mat3[0][2] * vec.x + mat3[1][2] * vec.y + mat3[2][2] * vec.z
		);
	}
};
*/

class Mat3
{
public:
	Mat3() : a00(0), a01(0), a02(0), a10(0), a11(0), a12(0), a20(0), a21(0), a22(0) {}
	Mat3(float r) :	a00(r), a10(r), a20(r), a01(r), a11(r), a21(r), a02(r), a12(r), a22(r) {}
	Mat3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) :
		a00(m00), a10(m10), a20(m20), a01(m01), a11(m11), a21(m21), a02(m02), a12(m12), a22(m22) {}
	void print() const { cout << a00 << " " << a01 << " " << a02 << " " << a10 << " " << a11 << " " << a12 << " " << a20 << " " << a21 << " " << a22 << " " << "\n"; };
	Mat3 operator * (const float& r) const {
		return Mat3(
			a00 * r, a10 * r, a20 * r,
			a01 * r, a11 * r, a21 * r,
			a02 * r, a12 * r, a22 * r);
	}
	Mat3 operator * (const Mat3& arr) const {
		return Mat3(
			a00 * arr.a00, a10 * arr.a10, a20 * arr.a20,
			a01 * arr.a01, a11 * arr.a11, a21 * arr.a21,
			a02 * arr.a02, a12 * arr.a12, a22 * arr.a22);
	}
	Vec3f matProduct(const Vec3f& vec)
	{
		return Vec3f(
			a00 * vec.x + a10 * vec.y + a20 * vec.z,
			a01 * vec.x + a11 * vec.y + a21 * vec.z,
			a02 * vec.x + a12 * vec.y + a22 * vec.z
		);
	}
	float a00, a01, a02, a10, a11, a12, a20, a21, a22;
};

Mat3 rotXmat(const float& theta)
{
	return Mat3(1, 0, 0, 0, cos(theta), -sin(theta), 0, sin(theta), cos(theta));
}

Mat3 rotYmat(const float& theta)
{
	return Mat3(cos(theta), 0, sin(theta), 0, 1, 0, -sin((theta)), 0, cos(theta));
}

Mat3 rotZmat(const float& theta)
{
	return Mat3(cos(theta), -sin(theta), 0, sin(theta), cos(theta), 0, 0, 0, 1);
}
