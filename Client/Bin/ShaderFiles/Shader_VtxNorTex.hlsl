matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;

vector			g_vCamPosition;

vector			g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

/* 매태리얼에서 Diffuse 값을 가리킨다(매태리얼 구조체 사용X). */
texture2D		g_DiffuseTexture[2];
texture2D		g_MaskTexture;

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
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
    float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos : TEXCOORD1; /* 시멘틱에 WorldPos는 존재하지 않으므로 UV로 페이크 */
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix); /* 월드 스페이스에서의 노말 */
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix); /* 카메라의 월드 포지션 */

	return Out;	
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
    float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* 픽셀 셰이더는 매 프레임 픽셀 개수만큼 호출된다. */
/* 따라서 픽셀 셰이더 안에서 반복문이나 조건문을 사용하는 것을 지양하도록 한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/* https://novemberfirst.tistory.com/112 */
	/* 최종 색 결정할 때, Light와 Material은 서로 곱셈 연산이다. */
	
	/* Diffuse는 빛을 받았을 때 반사되는 색상만을 가지고 있다(명암이 없는 순수 텍스처). */
	/* 텍스처 블렌딩 */ 
	/* hlsl에서는 벡터와 상수의 곱셈 및 덧셈이 가능하다. 이때 상수는 벡터의 크기에 맞게 계산된다. */
    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
	/* 마스크는 확대하지 않도록 * 30.f를 사용하지 않는다. */
    vector vMtrlDiffuse = vDestDiffuse * vMaskColor + vSourDiffuse * (1.f - vMaskColor);

	/* 램버트 조명 공식으로 명암 연산 */
	/* 코사인 그래프를 사용하며, 내적의 결과로 음수가 나오면 max 함수를 통해 0으로 세팅한다. */
	/* 내적은 원래 "A벡터의 길이 * B벡터의 길이 * cos(두 벡터가 이루는 각)"이다. */
	/* 이때 두 벡터의 길이를 정규화한다면 순수 cos(theta)만을 얻을 수 있다. */
	
    vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
	
	/* Ambient 환경광 부여 */
    vShade += g_vLightAmbient * g_vMtrlAmbient;

	/* Speculat 하이라이트(정반사광) 부여*/
	/* 내적은 원래 "A벡터의 길이 * B벡터의 길이 * cos(두 벡터가 이루는 각)"이다. */
	/* 따라서 B벡터의 길이가 1(단위벡터)이라면 B벡터에 투영된 A벡터의 길이 * cos(theta)가 나온다. */	
	/* 그러므로 입사 벡터 * -1과 B벡터를 내적한 값에 2를 곱해 원래 입사벡터에 더해주면 반사벡터가 얻어진다. */
    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    vector vLook = In.vWorldPos - g_vCamPosition;

	/* Specular은 최종 값(0~1 사이 실수)에 제곱연산을 함으로써 cos(0)에서 조금만 벗어나더라도 값이 확 떨어질 수 있게끔 처리한다. (그래프 생각)*/
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

	/* saturate() : 0보다 작으면 0, 1보다 크면 1로 세팅한다. */
	/* (디퓨즈 * 명암) + (스페큘러) */
    Out.vColor = ((g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade)) + ((g_vLightSpecular * g_vMtrlSpecular) * fSpecular);

	return Out;
}

technique11 DefaultTechnique
{
	pass Terrain
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}




