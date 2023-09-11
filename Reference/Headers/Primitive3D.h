#pragma once
#include "SimpleMath.h"
// *************
// Point3D
// *************
using Point3D = Vec3;

// *************
// Line3D
// *************

struct Line3D
{
	Point3D start = Point3D(0.f);
	Point3D end = Point3D(0.f);

	float Length() { return Vec3::Distance(start, end); }
	float LengthSq() { return Vec3::DistanceSquared(start, end); }
};

// *************
// Ray3D
// *************

struct Ray3D
{
	Point3D origin = Point3D(0.f);
	Vec3 direction = Vec3(0.f);

	void NormalizeDirection() { direction.Normalize(); }

	static Ray3D FromPoints(const Point3D& from, const Point3D& to) { return Ray3D{from, to - from}; }
};

// *************
// Sphere3D
// *************

struct Sphere3D
{
	Point3D position;
	float radius;
};

// *************
// AABB
// *************

struct AABB3D
{
	Point3D position = Vec3(0.f);
	Vec3 size = Vec3(1.f, 1.f, 1.f);

	static Vec3 GetMin(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	static Vec3 GetMax(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	// 생성자 없어서 임시 주석 처리
	/*static AABB3D FromMinMax(const Vec3& min, const Vec3& max)
	{
		return AABB3D((min + max) /2, (max - min) / 2);
	}*/
};

// *****************
// OBB
// *****************

struct OBB3D
{
	Point3D position;
	Vec3 size;
	Matrix orientation;
	// Vec4 quaternion;
	// Vec3 rotation;
};

// *****************
// Plane3D
// *****************

// 삼각형 (정점3개)
// 노멀 + 정점 1개
// 노멀 + 거리
struct Plane3D
{
	Vec3 normal;
	float distance;
};

// *****************
// Triangle3D
// *****************

struct Triangle3D
{
	union
	{
		struct  
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};
		Point3D points[3];
		float values[9];
	};
};

struct Interval3D
{
	float min;
	float max;
};