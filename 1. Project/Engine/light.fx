#ifndef _LIGHT
#define _LIGHT

#include "register.fx"
#include "struct.fx"
#include "func.fx"


// =================
// Directional Light
// MRT  : MRT_TYPE::LIGHT
// mesh : RectMesh

// g_int_0 : Light Index
// g_tex_0 : Position Target Tex
// g_tex_1 : Normal Target Tex
// g_tex_2 : Data Target Tex
// =================

#define DepthMap        g_tex_3
#define LightVP         g_mat_0


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_DirLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    
    return output;
}
struct PS_OUT
{
    float4 vDiffuse : SV_Target;
    float4 vSpecular : SV_Target1;
};

SamplerComparisonState g_ShadowSampler
{
    // sampler state
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    // sampler comparison state
    ComparisonFunc = LESS;
};

PS_OUT PS_DirLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    // ����Ÿ��, ������ ���� 0�� - ������ ���İ��� 1
    float4 vViewPos = g_tex_0.Sample(g_sam_0, vUV);
    
    if (0.f == vViewPos.a)
        discard;
    
    // ����Ÿ��, �븻����
    float4 vViewNormal = g_tex_1.Sample(g_sam_0, vUV);
    
    tLightColor lightcolor = (tLightColor) 0.f;
    CalcLight3D(vViewPos.xyz, vViewNormal.xyz, g_int_0, lightcolor);
    
     // �׸��� ����
    // ViewPos -> WorldPos
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), g_matViewInv).xyz;

    // WorldPos -> Light ����
    float4 vLightProj = mul(float4(vWorldPos, 1.f), LightVP);

    // w �� ������ ���� xy ������ǥ�� ����
    vLightProj.xyz /= vLightProj.w;

    // ���ø��� �ϱ� ���ؼ� ������ǥ�踦 UV ��ǥ��� ��ȯ
    float fShadowPow = 0.f;
    float2 vDepthMapUV = float2((vLightProj.x / 2.f) + 0.5f, -(vLightProj.y / 2.f) + 0.5f);
    /*if (DepthMap.SampleCmp(g_ShadowSampler, vDepthMapUV, vLightProj.z - 0.001f))
    {
        fShadowPow = DepthMap.SampleCmp(g_ShadowSampler, vDepthMapUV, vLightProj.z - 0.001f);        
    }*/

    float fDepth = DepthMap.Sample(g_sam_0, vDepthMapUV).r;

    
    // ������ ��ϵ� ���̺���, ��ü�� ���̰� �� �� ��, �׸��� ����
    if (0.f != fDepth
        && 0.f <= vDepthMapUV.x && vDepthMapUV.x <= 1.f
        && 0.f <= vDepthMapUV.y && vDepthMapUV.y <= 1.f
        && vLightProj.z >= fDepth + 0.0001f)
    {
        fShadowPow = 0.6f;
    }

    output.vDiffuse = lightcolor.vDiff * saturate(1.f - fShadowPow) + lightcolor.vAmb;


    
    // ����Ÿ��, �������������� �ݻ籤 �� �Ѱ���
    float SpecCoef = g_tex_2.Sample(g_sam_1, vUV).x;
    
    // �̹� ���Ǿ� ���ϵƱ⶧���� diffuse(Ȯ�걤)�� ambient(�ֺ���)
    // Ȯ�걤�� �ֺ��� ���ϰ� �� ���� �ݻ籤�� ��
    //output.vDiffuse = lightcolor.vDiff + lightcolor.vAmb;
    float4 vSpec = decode(SpecCoef);

    output.vSpecular.xyz = lightcolor.vSpec.xyz * vSpec.xyz;
    
    // ������������ ���⿡ ����Ʈ�� �ִٰ� ǥ��
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;

}

// =================
// Point Light
// MRT  : MRT_TYPE::LIGHT
// mesh : SphereMesh

// g_int_0 : Light Index
// g_tex_0 : Position Target Tex
// g_tex_1 : Normal Target Tex
// g_tex_2 : Data Target Tex
// g_int_0 : Light Type
// =================
VS_OUT VS_PointLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // ��ġ ���� ����� Ȯ��
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}

PS_OUT PS_PointLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // ���� ��ġ�� �ȼ����� ��������
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    float4 vViewPos = g_tex_0.Sample(g_sam_0, vUV);
    
    // �������� ���� == ���� ���� ��ü�� ����
    if (0.f == vViewPos.a)
    {
        discard;
    }
    
    // ���� ������ ���� PositionTarget �� ��ġ���� ������ ���÷� �̵���Ų��.
    // ������ ���÷� �ҷ��� �俪����� ���ϰ� �ٽ� ���� ������� ���Ѵ�.
    // ���ÿ������� �����޽�(Sphere) ���ο��ٸ�, ������ �������� �ȿ� �ִ� ��ǥ���ٴ� ��
    float4 vLocalPos = mul(mul(vViewPos, g_matViewInv), g_matWorldInv);
    
    // pointlight
    if (g_int_0 == 0)
        if (length(vLocalPos.xyz) > 0.5f)
        {
            discard;
        }
    
    // spotlight
    if (g_int_0 == 1)
        if (degrees(atan2(length(vLocalPos.xy), vLocalPos.z)) > 45)
        {
            discard;
        }

        float4 vViewNormal = g_tex_1.Sample(g_sam_0, vUV);
        
        tLightColor lightcolor = (tLightColor) 0.f;
        CalcLight3D(vViewPos.xyz, vViewNormal.xyz, g_int_0, lightcolor);
   
        float SpecCoef = g_tex_2.Sample(g_sam_0, vUV).x;
    
        output.vDiffuse = lightcolor.vDiff + lightcolor.vAmb;
        output.vSpecular = lightcolor.vSpec * SpecCoef;
    
        output.vDiffuse.a = 1.f;
        output.vSpecular.a = 1.f;

    return output;
}


// ===============
// DepthMap Shader
// MRT : ShadowMap MRT
// RS : CULL_BACK
// BS : Default
// DS : Less
// ===============
struct VS_DEPTH_IN
{
    float3 vPos : POSITION;
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_DEPTH_OUT
{
    float4 vPosition : SV_Position;
    float4 vProjPos : POSITION;
};

VS_DEPTH_OUT VS_DepthMap(VS_DEPTH_IN _in)
{
    VS_DEPTH_OUT output = (VS_DEPTH_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vWeights, _in.vIndices, 0);
    }

    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vProjPos = output.vPosition;

    return output;
}

float PS_DepthMap(VS_DEPTH_OUT _in) : SV_Target
{
    float fOut = 0.f;
    fOut = _in.vProjPos.z / _in.vProjPos.w;
    return fOut;
}

#endif