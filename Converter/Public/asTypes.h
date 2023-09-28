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

	uint 				materialIndex; /* ���� ���� �ִ� ���¸��� �� �� �޽ø� �׸� ���¸����� �ε��� */
	
	vector<int>			bones; /* ���� �޽ÿ� ������ �ִ� �� �ε��� */
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

	Matrix		transform;  /* Relative(�θ� ����) - ����ٰ� �θ��� m_CombinedTransformation�� ���ϸ� ��Ʈ �������� ��ȯ */
	Matrix		offsetTransform;  /* Global(��Ʈ ���� = m_CombinedTransformation) �� ����� */
	int32		index = -1;
	int32		parent = -1;

	uint		depth;  /* Ʈ�������� ���̰�, �� ���� �ڽ��̳� */
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

	float	fDuration;		/* ��ü ��� �ð� */
	float	fTickPerSecond;	/* �ʴ� ��� �ӵ� */

	vector<shared_ptr<asChannel>>	channels;
};
