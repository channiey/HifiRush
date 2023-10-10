/* 상수테이블. */
/* 서로 다른 셰이더 파일이더라도 상수 테이블의 전역변수 이름이 같다면 */
/* 하나의 셰이더 파일에 세팅한 전역변수는 다른 셰이더 파일의 같은 이름 전역변수에도 세팅된다 -> 이름이 같다면 같은 메모리 공간 사용 */
/* 또한 한번 세팅한 전역변수를 갱신하지 않는한, 계속 유지된다. */

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

/* w나누기 연산 진행(진정한 투영변환) */
/* 뷰포트스페이스(윈도우좌표)로 위치를 변환한다. */
/* 래스터라이즈 : 정점에 둘러쌓인 픽셀의 정보를 생성한다. */
/* 픽셀정보는 정점정보에 기반한다. */
/* 레스터 라이즈가 픽셀의 정보를 생성하고, 픽셀 셰이더가 픽셀의 색상을 결정한다. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

/* 받아온 픽셀의 정보를 바탕으로 하여 화면에 그려질 픽셀의 최종적인 색을 결정한다. */
struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* 전달받은 픽셀의 정보를 이용하여 픽셀의 최종적인 색을 결정하자. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	// Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
	/*g_Texture에서부터 PointSampler방식으로  In.vTexcoord위치에 해당하는 색을 샘플링(가져와)해와. */
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
		/* 사용하지 않는다면 반드시 NULL을 채워줘야 한다. */
		/* 여러 셰이더에 대해서 각각 어떤 버젼으로 빌드하고 어떤 함수를 호출하여 해당 셰이더가 구동되는지를 설정한다. */
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}
}




