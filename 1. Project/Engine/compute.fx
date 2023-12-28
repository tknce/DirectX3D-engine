#ifndef _COMPUTE
#define _COMPUTE

#include "register.fx"

RWTexture2D<float4> tex : register(u0);

#define Color       g_vec4_0
#define Resolution  g_vec2_0

// SV_GroupID           : �����尡 ���� �׷��� ��ǥ
// SV_GoupThreadID      : �׷� ������, �������� ��ǥ
// SV_GoupIndex         : �׷� ������, �������� �ε��� (1����)
// SV_DispatchThreadID  : ��ü ������(��� �׷� ����) ��������, ȣ��� �������� ��ǥ

[numthreads(32, 32, 1)] // �׷� �� ������ ���� (�ִ� 1024 ���� ���� ����)
void CS_Paint(uint3 _id : SV_DispatchThreadID)
{    
    if ((uint) Resolution.x <= _id.x || (uint) Resolution.y <= _id.y)
    {
        return;
    }
    
    tex[_id.xy] = Color;
}

#endif
