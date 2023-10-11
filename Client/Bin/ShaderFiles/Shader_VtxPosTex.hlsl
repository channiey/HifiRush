/* ������̺�. */
/* ���� �ٸ� ���̴� �����̴��� ��� ���̺��� �������� �̸��� ���ٸ� */
/* �ϳ��� ���̴� ���Ͽ� ������ ���������� �ٸ� ���̴� ������ ���� �̸� ������������ ���õȴ� -> �̸��� ���ٸ� ���� �޸� ���� ��� */
/* ���� �ѹ� ������ ���������� �������� �ʴ���, ��� �����ȴ�. */

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D	g_Texture;
texture2D	g_Textures[2];

sampler LinearSampler	= sampler_state { 
	Filter = MIN_MAG_MIP_LINEAR; 
};

sampler PointSampler	= sampler_state {
	Filter = MIN_MAG_MIP_POINT; 
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;	
}

/* w������ ���� ����(������ ������ȯ) */
/* ����Ʈ�����̽�(��������ǥ)�� ��ġ�� ��ȯ�Ѵ�. */
/* �����Ͷ����� : ������ �ѷ����� �ȼ��� ������ �����Ѵ�. */
/* �ȼ������� ���������� ����Ѵ�. */
/* ������ ����� �ȼ��� ������ �����ϰ�, �ȼ� ���̴��� �ȼ��� ������ �����Ѵ�. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

/* �޾ƿ� �ȼ��� ������ �������� �Ͽ� ȭ�鿡 �׷��� �ȼ��� �������� ���� �����Ѵ�. */
struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* ���޹��� �ȼ��� ������ �̿��Ͽ� �ȼ��� �������� ���� ��������. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	// Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
	/*g_Texture�������� PointSampler�������  In.vTexcoord��ġ�� �ش��ϴ� ���� ���ø�(������)�ؿ�. */
	// Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
	//Out.vColor = float4(1.f, 0.f, 0.f, 1.f);

    vector vSourColor = g_Textures[0].Sample(LinearSampler, In.vTexcoord);
    vector vDestColor = g_Textures[1].Sample(LinearSampler, In.vTexcoord);
	
    Out.vColor = vSourColor + vDestColor;

	return Out;
}

technique11 DefaultTechnique
{
	pass UI
	{
		/* ������� �ʴ´ٸ� �ݵ�� NULL�� ä����� �Ѵ�. */
		/* ���� ���̴��� ���ؼ� ���� � �������� �����ϰ� � �Լ��� ȣ���Ͽ� �ش� ���̴��� �����Ǵ����� �����Ѵ�. */
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}
}



