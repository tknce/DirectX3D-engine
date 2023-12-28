#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "register.fx"
#include "func.fx"

// =========================
// TessShader
// Domain : DOMAIN_DEFERRED
// =========================


#define                         FACE_X                          g_int_0
#define                         FACE_Z                          g_int_1  

#define                         HeightMap                       g_tex_0     // ���̸�
#define                         HeightMapResolution             g_vec2_0    // ���̸� �ػ�

StructuredBuffer<float4> WEIGHT_MAP : register(t17); // ����ġ ����
#define                         WeightMapResolution             g_vec2_1    // ����ġ ���� �ػ�

#define                         TileTexArr                      g_texarr_0  // Tile �迭 �ý���
#define                         TileCount                       g_float_1   // �迭 ����
#define                         SpecPow                         g_float_0   // �ݻ籤 ����

#define                         BrushMap                        g_tex_1     // �귯��
#define                         BrushCenterUV                   g_vec2_2    // �귯���� �׷��� �� �߽� uv
#define                         BrushResolution                 g_vec2_3    // �귯�� �ػ�
#define                         BrushVaild                      g_int_2     // ��ȿ��üũ


struct VS_IN
{
    float3 vPos : POSITION;
    float3 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION1;
};


VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    
    return output;
}

// Hull Shader
struct PatchTess
{
    float EdgeFactor[3] : SV_TessFactor;
    float InsideFactor : SV_InsideTessFactor;
};


// ��ġ ��� �Լ�
PatchTess PatchConstFunc(InputPatch<VS_OUT, 3> _patch, uint _patchId : SV_PrimitiveID)
{
    PatchTess factor = (PatchTess) 0.f;
        
    float3 vUpDown = (_patch[1].vWorldPos + _patch[2].vWorldPos) / 2.f;
    float3 vLeftRight = (_patch[0].vWorldPos + _patch[2].vWorldPos) / 2.f;
    float3 vSlide = (_patch[0].vWorldPos + _patch[1].vWorldPos) / 2.f;
    float3 vMid = (_patch[0].vWorldPos + _patch[1].vWorldPos + _patch[2].vWorldPos) / 3.f;
    
    factor.EdgeFactor[0] = pow(2, (int) GetTessFactor(vUpDown, 1, 4, 500.f, 2000.f));
    factor.EdgeFactor[1] = pow(2, (int) GetTessFactor(vLeftRight, 1, 4, 500.f, 2000.f));
    factor.EdgeFactor[2] = pow(2, (int) GetTessFactor(vSlide, 1, 4, 500.f, 2000.f));
    factor.InsideFactor = pow(2, (int) GetTessFactor(vMid, 1, 4, 500.f, 2000.f));
    
    return factor;
}


struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


[domain("tri")]
[outputcontrolpoints(3)]
[partitioning("integer")] //partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[patchconstantfunc("PatchConstFunc")]
[maxtessfactor(32)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _patch
                , uint _patchId : SV_PrimitiveID
                , uint _vtxId : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _patch[_vtxId].vLocalPos;
    output.vUV = _patch[_vtxId].vUV;
        
    return output;
}



// Domain Shader
struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float2 vFullUV : TEXCOORD1;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    float3 vViewNormal : NORMAL;
};

[domain("tri")]
DS_OUT DS_LandScape(PatchTess _tessFactor
                , const OutputPatch<HS_OUT, 3> _patch
                , float3 _Ratio : SV_DomainLocation)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    // �ڽ��� ��ġ�� ������ �����µ� ������ġ�� ������ ���ؼ� ����
    float3 vLocalPos = (_patch[0].vPos * _Ratio[0]) + (_patch[1].vPos * _Ratio[1]) + (_patch[2].vPos * _Ratio[2]);
    // ��� SV_DomainLocation�� uv�ε� �츮�� landscape�� �������־ uv�� ���� ���ؾ��Ѵ�.
    float2 vUV = (_patch[0].vUV * _Ratio[0]) + (_patch[1].vUV * _Ratio[1]) + (_patch[2].vUV * _Ratio[2]);
            
    float2 FullUV = vUV / float2(FACE_X, FACE_Z);
    
  // ���̸� �ȼ��� UV ����
    float2 vLandscapeUVStep = float2(1.f / HeightMapResolution.x, 1.f / HeightMapResolution.y);

    // ���� �ֺ�(��, �Ʒ�, ��, ��) ���̸� ���ø� ��ġ
    float2 vLandScapeUpUV = float2(FullUV.x, FullUV.y - vLandscapeUVStep.y);
    float2 vLandScapeDownUV = float2(FullUV.x, FullUV.y + vLandscapeUVStep.y);
    float2 vLandScapeLeftUV = float2(FullUV.x - vLandscapeUVStep.x, FullUV.y);
    float2 vLandScapeRightUV = float2(FullUV.x + vLandscapeUVStep.x, FullUV.y);

    // �ȼ� �� �������� ��ȭ��
    float2 vLandscapeLocalposStep = float2(FACE_X / HeightMapResolution.x, FACE_Z / HeightMapResolution.y);

    // �ֺ� �ȼ��� �����ϴ� �ֺ� ��������
    float3 vLocalUpPos = float3(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vLandScapeUpUV, 0).x, vLocalPos.z + vLandscapeLocalposStep.y);
    float3 vLocalDownPos = float3(vLocalPos.x, HeightMap.SampleLevel(g_sam_0, vLandScapeDownUV, 0).x, vLocalPos.z - vLandscapeLocalposStep.y);
    float3 vLocalLeftPos = float3(vLocalPos.x - vLandscapeLocalposStep.x, HeightMap.SampleLevel(g_sam_0, vLandScapeLeftUV, 0).x, vLocalPos.z);
    float3 vLocalRightPos = float3(vLocalPos.x + vLandscapeLocalposStep.x, HeightMap.SampleLevel(g_sam_0, vLandScapeRightUV, 0).x, vLocalPos.z);

    vLocalUpPos = mul(float4(vLocalUpPos, 1.f), g_matWV).xyz;
    vLocalDownPos = mul(float4(vLocalDownPos, 1.f), g_matWV).xyz;
    vLocalLeftPos = mul(float4(vLocalLeftPos, 1.f), g_matWV).xyz;
    vLocalRightPos = mul(float4(vLocalRightPos, 1.f), g_matWV).xyz;

    // Tangent, Binormal, Normal �� ���        
    output.vViewTangent = normalize(vLocalRightPos - vLocalLeftPos);
    output.vViewBinormal = normalize(vLocalDownPos - vLocalUpPos);
    output.vViewNormal = normalize(cross(output.vViewTangent, output.vViewBinormal).xyz);

    /*
    
    float3 vTangent = float3(1.f, 0.f, 0.f);
    float3 vNormal = float3(0.f, 1.f, 0.f);
    float3 vBinormal = float3(0.f, 0.f, -1.f);
    
    output.vViewTangent = normalize(mul(float4(vTangent, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(vBinormal, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matWV));
    */


    // ������ǥ ����
    vLocalPos.y = HeightMap.SampleLevel(g_sam_0, FullUV, 0).x;
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    output.vFullUV = FullUV;
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV);
    return output;
};




struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vData : SV_Target3;
};

PS_OUT PS_LandScape(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vColor = float4(1.f, 0.f, 1.f, 1.f);


    float3 vViewNormal = _in.vViewNormal;
        
    // Ÿ�� �迭�ؽ��İ� ������
    if (g_btexarr_0)
    {
        float2 derivX = ddx(_in.vUV); // �����ȼ��� x�� ��̺а��� ���Ѵ�
        float2 derivY = ddy(_in.vUV); // �����ȼ��� y�� ��̺а��� ���Ѵ�
        
        // Ÿ�� ����
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int) WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;

        int iMaxWeightIdx = -1;
        float fMaxWeight = 0.f;
       
        // Ÿ�� �迭
        for (int i = 0; i < 4; ++i)
        {
            vColor += TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            // vColor += TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, i), 0.f) * vWeight[i];

            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
        output.vColor = float4(vColor.rgb, 1.f);

        // Ÿ�� �븻
        if (-1 != iMaxWeightIdx)
        {
            float3 vTangentSpaceNormal = TileTexArr.SampleGrad(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), derivX, derivY).xyz;
            //float3 vTangentSpaceNormal = TileTexArr.SampleLevel(g_sam_0, float3(_in.vUV, iMaxWeightIdx + TileCount), 0.f).xyz;
            vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;

            float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
            vViewNormal = normalize(mul(vTangentSpaceNormal, matTBN));
        }
    }
    if (BrushVaild == 1)
    {
        float2 vScale = BrushResolution / HeightMapResolution;
        if (_in.vFullUV.x < BrushCenterUV.x - (vScale.x / 2) || BrushCenterUV.x + (vScale.x / 2) < _in.vFullUV.x
        || _in.vFullUV.y < BrushCenterUV.y - (vScale.y / 2) || BrushCenterUV.y + (vScale.y / 2) < _in.vFullUV.y)
        {
            //BrushMap.Sample(g_sam_0,);
            
        }
        else
        {
            // �߽� ��ġ�� ����
            // �ʺ� ���̿� �߽� uv ����
            int2 vCenterPos = HeightMapResolution * BrushCenterUV;
            int2 vScale = HeightMapResolution * float2(0.3f,0.3f);
            int2 Pos = _in.vFullUV * HeightMapResolution;
            // brush texture ���� ���ø� �� UV ���
            int2 vLTPos = vCenterPos - (vScale / 2);
            float2 vUV = float2(Pos - vLTPos) / float2(vScale);
            
            float4 brusha = BrushMap.Sample(g_sam_0, vUV);
            if (brusha.a > 0)
            {
                output.vColor.g += 0.5f;
            }           
        }
        
        

        
        
        //output.vColor;

    }
    
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vNormal = float4(vViewNormal, 1.f);
    output.vData.x = SpecPow;
            
        // �귯�� ��ȿ�� üũ

    
    
    return output;
}









#endif