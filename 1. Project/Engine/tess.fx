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

// �׳� �ٷ� �ѱ� ������ǥ��
VS_OUT VS_Tess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    return output;
}

// Hull Shader
// hull shader ����ü
struct PatchTess
{
    // �ﰢ�������� ������ ������ 3��
    float EdgeFactor[3] : SV_TessFactor;
    // �ﰢ�� ���� ������ �ܰ�
    float InsideFactor : SV_InsideTessFactor;
};


// ��ġ ��� �Լ�
// hullshader���� ȣ��Ǵ� �Լ�
// ��� �������� �����ش�
// ���ڷδ� ������ ���� ������ ���̵�-> ���° ��������
PatchTess PatchConstFunc(InputPatch<VS_OUT, 3> _patch, uint _patchId : SV_PrimitiveID)
{
    PatchTess factor = (PatchTess) 0.f;

    factor.EdgeFactor[0] = g_int_0;
    factor.EdgeFactor[1] = g_int_1;
    factor.EdgeFactor[2] = g_int_2;
    factor.InsideFactor = g_int_3;
    
    return factor;
}

// ����ü
struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


[domain("tri")] // ž������ ������ ��� ���ܸԾ�����
[outputcontrolpoints(3)] // � ��������
[partitioning("integer")] //partitioning("fractional_odd")] ���� �ɼ� �������� �Ǽ����� 
//integer ���� 
//fractional_odd�Ǽ�
[outputtopology("triangle_cw")] // ������ ���� 3��
[patchconstantfunc("PatchConstFunc")] // hullshader���� ȣ��� �Լ��̸� ���
[maxtessfactor(32)] // �ִ� ������ ����
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

// ������ ����
[domain("tri")]
DS_OUT DS_Tess(PatchTess _tessFactor
                , const OutputPatch<HS_OUT, 3> _patch
                , float3 _Ratio : SV_DomainLocation)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    // ���ҵ� �������� ������ ������ ������ ��������� ������ ���ϰ� ������ ���̴�.
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