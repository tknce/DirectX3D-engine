#ifndef _TESS
#define _TESS

#include "register.fx"


// ===============
// TessShader
// Domain : DOMAIN_OPAQUE
// ===============

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

// 그냥 바로 넘김 로컬좌표계
VS_OUT VS_Tess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    return output;
}

// Hull Shader
// hull shader 구조체
struct PatchTess
{
    // 삼각형단위로 들어오기 때문에 3개
    float EdgeFactor[3] : SV_TessFactor;
    // 삼각형 안쪽 나뉘는 단계
    float InsideFactor : SV_InsideTessFactor;
};


// 패치 상수 함수
// hullshader에서 호출되는 함수
// 몇개로 나누는지 정해준다
// 인자로는 정점의 갯수 정점의 아이디값-> 몇번째 정점인지
PatchTess PatchConstFunc(InputPatch<VS_OUT, 3> _patch, uint _patchId : SV_PrimitiveID)
{
    PatchTess factor = (PatchTess) 0.f;

    factor.EdgeFactor[0] = g_int_0;
    factor.EdgeFactor[1] = g_int_1;
    factor.EdgeFactor[2] = g_int_2;
    factor.InsideFactor = g_int_3;
    
    return factor;
}

// 구조체
struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


[domain("tri")] // 탑폴로지 정점이 어떻게 생겨먹었는지
[outputcontrolpoints(3)] // 몇개 들어오는지
[partitioning("integer")] //partitioning("fractional_odd")] 분할 옵션 정수인지 실수인지 
//integer 정수 
//fractional_odd실수
[outputtopology("triangle_cw")] // 정점의 갯수 3개
[patchconstantfunc("PatchConstFunc")] // hullshader에서 호출된 함수이름 명시
[maxtessfactor(32)] // 최대 나누는 갯수
HS_OUT HS_Tess(InputPatch<VS_OUT, 3> _patch
                , uint _patchId : SV_PrimitiveID
                , uint _vtxId : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _patch[_vtxId].vPos;
    output.vUV = _patch[_vtxId].vUV;
        
    return output;
}



// Domain Shader
struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// 정점의 형태
[domain("tri")]
DS_OUT DS_Tess(PatchTess _tessFactor
                , const OutputPatch<HS_OUT, 3> _patch
                , float3 _Ratio : SV_DomainLocation)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    // 분할된 정점들이 비율로 들어오기 때문에 로컬포즈는 비율을 곱하고 더해준 값이다.
    float3 vLocalPos = (_patch[0].vPos * _Ratio[0]) + (_patch[1].vPos * _Ratio[1]) + (_patch[2].vPos * _Ratio[2]);
    float2 vUV = (_patch[0].vUV * _Ratio[0]) + (_patch[1].vUV * _Ratio[1]) + (_patch[2].vUV * _Ratio[2]);
        
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    return output;
};


float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    
    return vColor;
}













#endif