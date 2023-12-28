#ifndef _RAZERPARTICLE
#define _RAZERPARTICLE

#include "register.fx"
#include "func.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleShare> ParticleShare : register(u1);

#define PARTICLE_MAX    g_int_0
#define Particle        ParticleBuffer[_id.x]
#define ShareData       ParticleShare[0]

#define SpawnRange      g_float_0
#define IsWorldSpawn    g_int_1
#define ObjectWorldPos  g_vec4_0
#define MinSpeed        g_vec2_0.x
#define MaxSpeed        g_vec2_0.y
#define MinLife         g_vec2_1.x
#define MaxLife         g_vec2_1.y



[numthreads(128, 1, 1)]
void CS_RazerParticle(uint3 _id : SV_DispatchThreadID)
{
    if ((uint) PARTICLE_MAX <= _id.x)
        return;
            
    if (0 == Particle.iActive)
    {
        while (0 < ShareData.iAliveCount)
        {
            int iOriginValue = ShareData.iAliveCount;
            int iExchange = iOriginValue - 1;
        
            //InterlockedExchange(ShareData.iAliveCount, iExchange, iExchange);
            InterlockedCompareExchange(ShareData.iAliveCount, iOriginValue, iExchange, iExchange);
            
            if (iOriginValue == iExchange)
            {
                Particle.iActive = 1;
                break;
            }
        }
        
        if (Particle.iActive)
        {
            // ���������� ��ġ�� ������ �����Ѵ�.
            // ���ø��� �õ��� UV �� ����Ѵ�.
            float4 vRandom = (float4) 0.f;
            
            float2 vUV = float2((float) _id.x / PARTICLE_MAX, 0.5f);
            vUV.x += g_fAccTime;
            vUV.y += sin((vUV.x + g_fAccTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float4(GaussianSample(vUV + float2(0.f, 0.f)).x, GaussianSample(vUV + float2(0.1f, 0.f)).x, GaussianSample(vUV + float2(0.2f, 0.f)).x, GaussianSample(vUV + float2(0.3f, 0.f)).x);
            
            
            // �簢�������� ����
            Particle.vRelativePos.x = 1.f;
            //Particle.vRelativePos.y = vRandom.y * SpawnRange - SpawnRange / 2.f;
            Particle.vRelativePos.y = 50.f;
            // ���� ������ ����
            //float fTheta = vRandom.x * 3.141592f * 2.f;
            //Particle.vRelativePos.xy = float2(cos(fTheta), sin(fTheta)) * vRandom.y * SpawnRange;
            //Particle.vRelativePos.z = 0.f;
            
            // ��ƼŬ ����
            Particle.vDir.xy = normalize(float2(Particle.vRelativePos.xy));
            
            if (IsWorldSpawn)
            {
                Particle.vRelativePos.xy += ObjectWorldPos.xy;
            }
            
            // ��ƼŬ �ӷ�
            Particle.fSpeed = vRandom.z * (MaxSpeed - MinSpeed) + MinSpeed;
            
            // ��ƼŬ Life
            Particle.fCurTime = 0.f;
            Particle.fMaxTime = vRandom.w * (MaxLife - MinLife) + MinLife;
        }
    }
    
    // Ȱ��ȭ �� ��ƼŬ�� ������Ʈ
    else
    {
        Particle.fCurTime += g_fDT;
        if (Particle.fMaxTime < Particle.fCurTime)
        {
            Particle.iActive = 0;
        }
        else
        {
            Particle.vRelativePos.y -=  ParticleBuffer[_id.x].fSpeed * g_fDT;
        }
    }
}

#endif