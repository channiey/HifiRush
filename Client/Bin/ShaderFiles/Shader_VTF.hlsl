
/* 상수테이블. */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightPos = vector(0.f, 0.f, 0.f, 1.f);
float g_fLightRange = 0.f;
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

//#define MAX_MODEL_TRANSFORMS 600
//#define MAX_MODEL_KEYFRAMES 500 

struct KeyframeDesc
{
    int animIndex;
    uint currFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

KeyframeDesc g_Keyframes;

Texture2DArray g_TransformMap;
//Texture2D g_TransformMap;

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

    int animIndex =  g_Keyframes.animIndex;
    int currFrame =  g_Keyframes.currFrame;
    int nextFrame =  g_Keyframes.nextFrame;
    float ratio = g_Keyframes.ratio;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;

    /* 영향 주는 뼈 갯수 4개만큼 반복한다. */
    for (int i = 0; i < 4; i++)
    {
        /* int 4는 텍스처 배열에서 */
        /* x 좌표(해당 뼈 트랜스폼 행), y좌표(현재 프레임), 배열 인덱스(애님인덱스), 밉맵레벨(아직 사용X)을 나타낸다. */
        c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame, animIndex, 0));
        c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame, animIndex, 0));
        c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame, animIndex, 0));
        c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame, animIndex, 0));
        curr = matrix(c0, c1, c2, c3); /* indices[i]의 현재 프레임 뼈 행렬 */

        n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame, animIndex, 0));
        n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame, animIndex, 0));
        n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame, animIndex, 0));
        n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame, animIndex, 0));
        next = matrix(n0, n1, n2, n3);
        
        //c0 = g_TransformMap.Load(int3(indices[i] * 4 + 0, currFrame, 0));
        //c1 = g_TransformMap.Load(int3(indices[i] * 4 + 1, currFrame, 0));
        //c2 = g_TransformMap.Load(int3(indices[i] * 4 + 2, currFrame, 0));
        //c3 = g_TransformMap.Load(int3(indices[i] * 4 + 3, currFrame, 0));
        //curr = matrix(c0, c1, c2, c3);
        
        //n0 = g_TransformMap.Load(int3(indices[i] * 4 + 0, nextFrame, 0));
        //n1 = g_TransformMap.Load(int3(indices[i] * 4 + 1, nextFrame, 0));
        //n2 = g_TransformMap.Load(int3(indices[i] * 4 + 2, nextFrame, 0));
        //n3 = g_TransformMap.Load(int3(indices[i] * 4 + 3, nextFrame, 0));
        //next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio);

        transform += mul(weights[i], result);
        //transform += result;
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




