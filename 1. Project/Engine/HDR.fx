#ifndef _COMPUTE
#define _COMPUTE

#include "register.fx"

#define LUM_FACTOR  float4(0.2126, 0.7152, 0.0722, 1)




StructuredBuffer<float> AverageValues1D : register(t0);
RWStructuredBuffer<float> AverageLumFinal : register(u1);

groupshared float SharedPositions[1024];

//cbuffer Adapt_Bloom_CB : register(b3)
//{
//    float g_fAdaptation;
//    float3 vEmpty;
//}
//
//cbuffer BloomThresholdCB : register(b4)
//{
//    float g_fBloomThreshold;
//    float3 g_vEmpty;
//}

// �� �����忡 ���� 4x4 �ٿ� �������� �����Ѵ�
float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.f;

    if (CurPixel.y < g_vRenderResolution.y)
    {
        int3 iFullResPos = int3(CurPixel * 4, 0);
        float4 vDownScaled = float4(0.f, 0.f, 0.f, 0.f);

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                vDownScaled += HDR.Load(iFullResPos, int2(j, i));
            }
        }
        vDownScaled /= 16;
        
        // �ȼ��� �ֵ� �� ���
        avgLum = dot(vDownScaled, LUM_FACTOR);

        // ���� �޸𸮿� ��� ���
        SharedPositions[groupThreadId] = avgLum;
    }

    // ����ȭ �� ���� �ܰ��
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

// ������ ���� ���� 4���� ������ �ٿ�����Ѵ�
float DownScale1024to4(uint dispachThreadId, uint groupThreadId,
    float avgLum)
{
    // �ٿ���� �ڵ带 Ȯ��
    for (uint iGroupSize = 4, iStep1 = 1, iStep2 = 2, iStep3 = 3;
        iGroupSize < 1024;
        iGroupSize *= 4, iStep1 *= 4, iStep2 *= 4, iStep3 *= 4)
    {
        if (groupThreadId % iGroupSize == 0)
        {
            float fStepAvgLum = avgLum;

            fStepAvgLum += dispachThreadId + iStep1 < (1600 * 900) ?
                SharedPositions[groupThreadId + iStep1] : avgLum;

            fStepAvgLum += dispachThreadId + iStep2 < (1600 * 900) ?
                SharedPositions[groupThreadId + iStep2] : avgLum;

            fStepAvgLum += dispachThreadId + iStep3 < (1600 * 900) ?
              SharedPositions[groupThreadId + iStep3] : avgLum;

            // ��� �� ����
            avgLum = fStepAvgLum;
            SharedPositions[groupThreadId] = fStepAvgLum;
        }
        // ����ȭ �� ��������
        GroupMemoryBarrierWithGroupSync();
    }
    return avgLum;
}

// 4���� ���� �ϳ��� ��հ����� �ٿ������ �� �����Ѵ�
void DownScale4to1(uint dispatchThreadId, uint groupThreadId,
    uint groupId, float avgLum)
{
    if (groupThreadId == 0)
    {
        //  ������ �׷쿡 ���� ��� �ֵ� �� ���
        float fFinalAvgLum = avgLum;

        fFinalAvgLum += dispatchThreadId + 256 < (1600 * 900) ?
            SharedPositions[groupThreadId + 256] : avgLum;

        fFinalAvgLum += dispatchThreadId + 512 < (1600 * 900) ?
            SharedPositions[groupThreadId + 512] : avgLum;

        fFinalAvgLum += dispatchThreadId + 768 < (1600 * 900) ?
            SharedPositions[groupThreadId + 768] : avgLum;

        fFinalAvgLum /= 1024.f;

        // ���� ���� ID UAV�� ��� �� ���� ��������
        AverageLumFinal[groupId] = fFinalAvgLum;
        //AverageLumFinal[0] = fFinalAvgLum;

    }
}

// �̷��� ���� ���� ���̴� ��Ʈ�� ����Ʈ�� ���Եȴ�
[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 groupId : SV_GroupID,
    uint3 dispatchThreadId : SV_DispatchThreadID,
    uint3 groupThreadId : SV_GroupThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadId.x % g_vRenderResolution.x,
    dispatchThreadId.x / g_vRenderResolution.x);

    // 16 �ȼ� �׷��� �ϳ��� �ȼ��� �ٿ� ���� �޸𸮿� ����
    float favgLum = DownScale4x4(vCurPixel, groupThreadId.x);

    // 1024���� 4�� �ٿ����
    favgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x,
         favgLum);

    // 4���� 1�� �ٿ����
    DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x,
        favgLum);
    

}

// ù ��° ��ǻƮ ���̴��� ������ �Ϸ�Ǹ� ������ ��� ���۸� ����� �ι�° ��ǻƮ ���̴��� �����Ѵ�
// �߰� �� �ֵ� SRV�� ��� �ֵ� UAV ���� ������ ����Ѵ�
#define MAX_GROUPS 64

// ���� �޸� �׷쿡 �߰� �� ����
groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID,
        uint3 groupThreadId : SV_GroupThreadID,
        uint3 dispatchThreadId : SV_DispatchThreadID)
{

    // ���� �޸𸮿� ID�� ����
    float favgLum = 0.f;
    float domain = 350.f;
    if (dispatchThreadId.x < domain)
    {
        favgLum = AverageLumFinal[dispatchThreadId.x];
    }

    SharedAvgFinal[dispatchThreadId.x] = favgLum;

    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 64���� 16���� �ٿ� ������
    if (dispatchThreadId.x % 4 == 0)
    {
        // �ֵ� �� �ջ�
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 1 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 1] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 2 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 2] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 3 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 3] : favgLum;

        // ��� �� ����
        favgLum = fstepAvgLum;

        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 16���� 4�� �ٿ����
    if (dispatchThreadId.x % 16 == 0)
    {
        // �ֵ� �� �ջ�
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 4 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 4] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 8 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 8] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 12 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 12] : favgLum;

        // ��� �� ����
        favgLum = fstepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 4���� 1�� �ٿ����
    if (dispatchThreadId.x == 0)
    {
        // �ֵ� �� �ջ�
        float fFinalLumValue = favgLum;

        fFinalLumValue += dispatchThreadId.x + 16 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 16] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 32 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 32] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 48 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 48] : favgLum;

        fFinalLumValue /= 64.f;
        
        AverageLumFinal[0] = max(fFinalLumValue, 0.0001);
        //AverageLumFinal[0] = 10.f;

    }
    //AverageLumFinal[dispatchThreadId.x] = SharedAvgFinal[dispatchThreadId.x];
    //AverageLumFinal[dispatchThreadId.x] = favgLum;
}

#endif