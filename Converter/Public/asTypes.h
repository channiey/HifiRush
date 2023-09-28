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
	
	bool				isAinm;

	vector<VTXSTATIC>	verticesStatic;
	vector<VTXANIM>		verticesAnim;
	vector<int>			indices;

	uint 				materialIndex; /* 모델이 갖고 있는 매태리얼 중 이 메시를 그릴 매태리얼의 인덱스 */
	
	vector<int>			bones; /* 현재 메시에 영향을 주는 뼈 인덱스 */
};

struct asMaterial
{
	string diffuseFilePath;
	string specularFilePath;
	string normalFilePath;
};

struct asBone
{
	string		name;

	Matrix		transform;  /* Relative(부모 기준) - 여기다가 부모의 m_CombinedTransformation을 곱하면 루트 기준으로 변환 */
	Matrix		offsetTransform;  /* Global(루트 기준 = m_CombinedTransformation) 의 역행렬 */
	int32		index = -1;
	int32		parent = -1;

	uint		depth;  /* 트리에서의 깊이값, 즉 몇차 자식이냐 */
};

struct asKeyFrame
{
	float	fTime;
	Vec3	vScale;
	Vec4	vRotation;
	Vec3	vPosition;
};

struct asChannel
{
	string name; /* Bone Name */

	vector<shared_ptr<asKeyFrame>> keyframes;
};

struct asAnimation
{
	string name;

	float	fDuration;		/* 전체 재생 시간 */
	float	fTickPerSecond;	/* 초당 재생 속도 */

	vector<shared_ptr<asChannel>>	channels;
};
