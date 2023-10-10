matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;

vector			g_vCamPosition;

vector			g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

/* ���¸��󿡼� Diffuse ���� ����Ų��(���¸��� ����ü ���X). */
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
    float4		vWorldPos : TEXCOORD1; /* �ø�ƽ�� WorldPos�� �������� �����Ƿ� UV�� ����ũ */
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix); /* ���� �����̽������� �븻 */
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix); /* ī�޶��� ���� ������ */

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

/* �ȼ� ���̴��� �� ������ �ȼ� ������ŭ ȣ��ȴ�. */
/* ���� �ȼ� ���̴� �ȿ��� �ݺ����̳� ���ǹ��� ����ϴ� ���� �����ϵ��� �Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/* https://novemberfirst.tistory.com/112 */
	/* ���� �� ������ ��, Light�� Material�� ���� ���� �����̴�. */
	
	/* Diffuse�� ���� �޾��� �� �ݻ�Ǵ� ������ ������ �ִ�(����� ���� ���� �ؽ�ó). */
	/* �ؽ�ó ���� */ 
	/* hlsl������ ���Ϳ� ����� ���� �� ������ �����ϴ�. �̶� ����� ������ ũ�⿡ �°� ���ȴ�. */
    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
	/* ����ũ�� Ȯ������ �ʵ��� * 30.f�� ������� �ʴ´�. */
    vector vMtrlDiffuse = vDestDiffuse * vMaskColor + vSourDiffuse * (1.f - vMaskColor);

	/* ����Ʈ ���� �������� ��� ���� */
	/* �ڻ��� �׷����� ����ϸ�, ������ ����� ������ ������ max �Լ��� ���� 0���� �����Ѵ�. */
	/* ������ ���� "A������ ���� * B������ ���� * cos(�� ���Ͱ� �̷�� ��)"�̴�. */
	/* �̶� �� ������ ���̸� ����ȭ�Ѵٸ� ���� cos(theta)���� ���� �� �ִ�. */
	
    vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
	
	/* Ambient ȯ�汤 �ο� */
    vShade += g_vLightAmbient * g_vMtrlAmbient;

	/* Speculat ���̶���Ʈ(���ݻ籤) �ο�*/
	/* ������ ���� "A������ ���� * B������ ���� * cos(�� ���Ͱ� �̷�� ��)"�̴�. */
	/* ���� B������ ���̰� 1(��������)�̶�� B���Ϳ� ������ A������ ���� * cos(theta)�� ���´�. */	
	/* �׷��Ƿ� �Ի� ���� * -1�� B���͸� ������ ���� 2�� ���� ���� �Ի纤�Ϳ� �����ָ� �ݻ纤�Ͱ� �������. */
    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    vector vLook = In.vWorldPos - g_vCamPosition;

	/* Specular�� ���� ��(0~1 ���� �Ǽ�)�� ���������� �����ν� cos(0)���� ���ݸ� ������� ���� Ȯ ������ �� �ְԲ� ó���Ѵ�. (�׷��� ����)*/
    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

	/* saturate() : 0���� ������ 0, 1���� ũ�� 1�� �����Ѵ�. */
	/* (��ǻ�� * ���) + (����ŧ��) */
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




