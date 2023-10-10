
/* 상수테이블. */
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

/* 내 현재 모델의 전체 뼈(x), 이 메시가 사용하는 뼈(o) */
matrix			g_BoneMatrices[256];

vector			g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightPos = vector(0.f, 0.f, 0.f, 1.f);
float			g_fLightRange = 0.f; 
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vCamPosition;

texture2D		g_DiffuseTexture;

sampler LinearSampler = sampler_state {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state {
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float3		vTangent : TANGENT;

	/* 이 메시에 영향을 주는 뼈들 중 몇번째 뼈?!  */
	uint4		vBlendIndices : BLENDINDEX;
	float4		vBlendWeights : BLENDWEIGHT;

};

struct VS_OUT
{
	/* float4 : w값을 반드시 남겨야하는 이유는 w에 뷰스페이스 상의 깊이(z)를 보관하기 위해서다. */
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};


/* 버텍스에 대한 변환작업을 거친다.  */
/* 변환작업 : 정점의 위치에 월드, 뷰, 투영행렬을 곱한다. and 필요한 변환에 대해서 자유롭게 추가해도 상관없다 .*/
/* 버텍스의 구성 정보를 변경한다. */
VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y + 
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z + 
		g_BoneMatrices[In.vBlendIndices.w] * In.vBlendWeights.w;

	vector			vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);


	/* mul : 모든(곱하기가 가능한) 행렬의 곱하기를 수행한다. */
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexcoord = In.vTexcoord;
	Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;	
}

/* w나누기 연산. 진정한 투영변환. */
/* 뷰포트스페이스(윈도우좌표)로 위치를 변환한다. */
/* 래스터라이즈 : 정점에 둘러쌓인 픽셀의 정보를 생성한다. */
/* 픽셀정보는 정점정보에 기반한다. */

struct PS_IN
{
	/* Viewport */
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

/* 받아온 픽셀의 정보를 바탕으로 하여 화면에 그려질 픽셀의 최종적인 색을 결정하낟. */
struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* 전달받은 픽셀의 정보를 이용하여 픽셀의 최종적인 색을 결정하자. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

	if (vMtrlDiffuse.a < 0.3f)
		discard;


	vector	vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f) +
		g_vLightAmbient * g_vMtrlAmbient;		

	vector	vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	vector	vLook = In.vWorldPos - g_vCamPosition;

	float	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	return Out;
}

technique11 DefaultTechnique
{
	/* */
	pass Mesh
	{
		/* 여러 셰이더에 대해서 각각 어떤 버젼으로 빌드하고 어떤 함수를 호출하여 해당 셰이더가 구동되는지를 설정한다. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}




