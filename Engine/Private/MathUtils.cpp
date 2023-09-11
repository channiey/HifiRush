#include "MathUtils.h"

float MathUtils::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}

Vec2 MathUtils::RandomVec2(float r1, float r2)
{
	Vec2 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);

	return result;
}

Vec3 MathUtils::RandomVec3(float r1, float r2)
{
	Vec3 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);
	result.z = Random(r1, r2);

	return result;
}