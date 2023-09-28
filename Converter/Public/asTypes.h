#pragma once



enum class MODEL_TYPE { STATIC, ANIM, TYPEEND };

struct VTXSTATIC
{
	Vec3		vPosition;
	Vec3		vNormal;
	Vec2		vTexture;
	Vec3		vTangent;
};

struct VTXANIM
{
	Vec3		vPosition;
	Vec3		vNormal;
	Vec2		vTexture;
	Vec3		vTangent;

	XMUINT4		vBlendIndex;
	Vec4		vBlendWeight;
};

struct asMesh
{
	string				name;

	vector<VTXSTATIC>	verticesStatic;
	vector<VTXANIM>		verticesAnim;
	vector<int>			indices;

	uint 				materialIndex;

};
