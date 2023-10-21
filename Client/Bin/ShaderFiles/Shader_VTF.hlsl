
/* ������̺�. */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightPos = vector(0.f, 0.f, 0.f, 1.f);
float  g_fLightRange = 0.f;
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

texture2D g_DiffuseTexture;

sampler LinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

sampler PointSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = wrap;
    AddressV = wrap;
};

struct KeyframeDesc
{
    // 4����Ʈ 6��, �� 24����Ʈ
    int iAnimIndex;
    uint iCurFrame;
    uint iNextFrame;
    float fRatio;
    float fFrameAcc;
    float fAnimAcc;
    float fSpeed;
    
    int iPadding;
};

struct TweenFrameDesc
{
    KeyframeDesc cur;
    KeyframeDesc next;
    
    float fTweenDuration;
    float fTweenRatio;
    float fTweenAcc;
    float fPadding;
};

TweenFrameDesc  g_TweenFrames;
Texture2DArray  g_TransformMap;

struct VS_IN
{
    float3  vPosition   : POSITION;
    float3  vNormal     : NORMAL;
    float2  vTexcoord   : TEXCOORD0;
    float3  vTangent    : TANGENT;
    uint4   vBlendIndices : BLENDINDEX;
    float4  vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition    : SV_POSITION;
    float4 vNormal      : NORMAL;
    float2 vTexcoord    : TEXCOORD0;
    float4 vWorldPos    : TEXCOORD1;
};

matrix GetAnimationMatrix(VS_IN input)
{
    float indices[4] = { input.vBlendIndices.x, input.vBlendIndices.y, input.vBlendIndices.z, input.vBlendIndices.w };
    float weights[4] = { input.vBlendWeights.x, input.vBlendWeights.y, input.vBlendWeights.z, input.vBlendWeights.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];
	
	/* cur */
    animIndex[0] = g_TweenFrames.cur.iAnimIndex;
    currFrame[0] = g_TweenFrames.cur.iCurFrame;
    nextFrame[0] = g_TweenFrames.cur.iNextFrame;
    ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
    animIndex[1] = g_TweenFrames.next.iAnimIndex;
    currFrame[1] = g_TweenFrames.next.iCurFrame;
    nextFrame[1] = g_TweenFrames.next.iNextFrame;
    ratio[1] = g_TweenFrames.next.fRatio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
		/* cur */
        c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
        c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
        c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
        c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
        curr = matrix(c0, c1, c2, c3);

        n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio[0]);

		/* if next */
        if (animIndex[1] >= 0)
        {
            c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);

            n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);

            matrix nextResult = lerp(curr, next, ratio[1]);

            result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
        }
		
        transform += mul(weights[i], result);
    }

    return transform;
}


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix m = GetAnimationMatrix(In);    
    vector vPosition = mul(vector(In.vPosition, 1.f), m);

    matrix matWV    = mul(g_WorldMatrix, g_ViewMatrix);
    matrix matWVP   = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;


    vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f) +
		g_vLightAmbient * g_vMtrlAmbient;

    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    vector vLook = In.vWorldPos - g_vCamPosition;

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

    Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    return Out;
}

technique11 DefaultTechnique
{
    pass Mesh
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}




