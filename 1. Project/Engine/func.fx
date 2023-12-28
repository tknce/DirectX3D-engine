#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "register.fx"

#define light g_Light2DBuffer[iLightIdx]

void CalcLight2D(inout tLightColor _LightColor, float3 _vWorldPos, int iLightIdx)
{   
    // Directional Light
    if (0 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
    }
    
    // Point Light
    else if (1 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);
            
        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
            float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;
                
            _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;
        }
    }   
    
    // Spot Light
    else
    {
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);
                
        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
            
            // dot(normalize.(dir) dir.x cos,dir.y sin)
            // dot(normalize.(dir) dir.x -cos,dir.y -sin)
            
            
            
            float3 nomaldir = normalize(g_Light2DBuffer[iLightIdx].vWorldDir.xyz);
            float playerradius = dot(_vWorldPos.xyz, nomaldir);
           
            float2 a = float2(playerradius * cos(light.fAngle), playerradius * sin(light.fAngle));
            float2 b = float2(playerradius * cos(-light.fAngle), playerradius * sin(-light.fAngle));
            
            float3 LightDotline = float3(light.vWorldDir.x * cos(light.fAngle), light.vWorldDir.x * sin(light.fAngle), 0.f);
            
            float LightDotUp = dot(normalize(light.vWorldDir.xyz), LightDotline.xyz);
            LightDotline = float3(light.vWorldDir.x * cos(-light.fAngle), light.vWorldDir.x * sin(-light.fAngle), 0.f);
            float LightDotDown = dot(normalize(light.vWorldDir.xyz), LightDotline.xyz);
            
            
            if (a.x < _vWorldPos.x && a.y > _vWorldPos.y && b.x < _vWorldPos.x && b.y < _vWorldPos.y)
            {
                float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;
              
                _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
            }
        }
    }
}
void CalcLight3D(float3 _vViewPos, float3 _vViewNormal, int _LightIdx, inout tLightColor _lightcolor)
{
    tLightInfo lightinfo = g_Light3DBuffer[_LightIdx];
       
    float3 ViewLightDir = (float3) 0.f;
    float fDiffPow = (float) 0.f;
    float fSpecPow = (float) 0.f;
    
    // Directional Light
    if (lightinfo.iLightType == 0)
    {
        ViewLightDir = normalize(mul(float4(lightinfo.vWorldDir.xyz, 0.f), g_matView));
    
        // View 스페이스 상에서 표면의 빛의 세기를 구함
        fDiffPow = saturate(dot(-ViewLightDir, _vViewNormal));
    
        // 반사광 세기를 구함
        // 표면의 빛의 반사벡터       
        float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _vViewNormal) * _vViewNormal);
        
        // 시점에서 표면을 향하는 벡터
        float3 vEye = normalize(_vViewPos);
        
        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
        fSpecPow = saturate(dot(-vEye, vViewReflect));
        fSpecPow = pow(fSpecPow, 20);
    }
    
    // Point Light
    else if (lightinfo.iLightType == 1)
    {
        // ViewSpace 상에서 광원의 위치를 알아낸다.
        float3 vLightViewPos = mul(float4(lightinfo.vWorldPos.xyz, 1.f), g_matView);
        
        // 광원의 위치에서 표면을 향하는 벡터를 구한다.
        ViewLightDir = _vViewPos - vLightViewPos;
        
        // 광원에서 표면까지의 거리를 구한다.
        float fDist = length(ViewLightDir);
        
        // 광원에서 표면을 향하는 단위벡터를 구한다.        
        ViewLightDir = normalize(ViewLightDir);
        
        // 반경대비 거리에 따른 빛의 세기 비율
        //float fRatio = saturate(1.f - (fDist / lightinfo.fRadius));        
        float fRatio = cos(saturate(fDist / lightinfo.fRadius) * 3.1415926535f * 0.5f);
                        
        // View 스페이스 상에서 표면의 빛의 세기를 구함        
        fDiffPow = saturate(dot(-ViewLightDir, _vViewNormal))* fRatio;

        // 반사광 세기를 구함
        // 표면의 빛의 반사벡터       
        float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _vViewNormal) * _vViewNormal);
        
        // 시점에서 표면을 향하는 벡터
        float3 vEye = normalize(_vViewPos);
        
        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
        fSpecPow = saturate(dot(-vEye, vViewReflect));
        fSpecPow = pow(fSpecPow, 20);
    }
    
    // SpotLight
    else
    {
        // ViewSpace 상에서 광원의 위치를 알아낸다.
        float3 vLightViewPos = mul(float4(lightinfo.vWorldPos.xyz, 1.f), g_matView);
        
        

        //float3 vLightViewDir = _vViewPos - vLightViewPos;
        
        float3 vLightViewDir = normalize(mul(float4(lightinfo.vWorldDir.xyz, 0.f), g_matView));

        float3 Dir = normalize(_vViewPos - vLightViewPos);
        
        // 픽셀의 위치와 내적해서 각도를 구한다음 내가 설정한 각도와 일치하는지 판단한다
        float3 vViwePos = normalize(_vViewPos);
        float angle = dot(vLightViewDir, Dir);
        
        // 광원의 위치에서 표면을 향하는 벡터를 구한다.
       vLightViewDir = _vViewPos - vLightViewPos;
            
        
        // 광원에서 표면까지의 거리를 구한다.
            float fDist = length(vLightViewDir);
        
        // 광원에서 표면을 향하는 단위벡터를 구한다.
            ViewLightDir = normalize(vLightViewDir);
        
            
        if (angle > lightinfo.fAngle)
        {
        // 반경대비 거리에 따른 빛의 세기 비율
        //float fRatio = saturate(1.f - (fDist / lightinfo.fRadius));        
            float fRatio = cos(saturate(fDist / lightinfo.fRadius) * 3.1415926535f * 0.5f);
                        
        // View 스페이스 상에서 표면의 빛의 세기를 구함        
            fDiffPow = saturate(dot(-vLightViewDir, _vViewNormal)) * fRatio;

        // 반사광 세기를 구함
        // 표면의 빛의 반사벡터       
            float3 vViewReflect = normalize(vLightViewDir + 2.f * dot(-vLightViewDir, _vViewNormal) * _vViewNormal);
        
        // 시점에서 표면을 향하는 벡터
            float3 vEye = normalize(_vViewPos);
        
        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
            fSpecPow = saturate(dot(-vEye, vViewReflect));
            fSpecPow = pow(fSpecPow, 20);
        }
  
    }
    
    // 최종 난반사광
    _lightcolor.vDiff += lightinfo.color.vDiff * fDiffPow;
    
    // 최종 반사광 
    _lightcolor.vSpec += lightinfo.color.vSpec * fSpecPow;

    // 최종 환경광
    _lightcolor.vAmb += lightinfo.color.vAmb;
}

static float GaussianFilter[5][5] =
{
    0.003f  , 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f , 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f , 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f , 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f  , 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

float4 GaussianSample(float2 _vUV)
{
    float4 vOutColor = (float4) 0.f;
    
    if (1.f < _vUV.x)
        _vUV.x = frac(_vUV.x);    
    else if (_vUV.x < 0.f)
        _vUV.x = 1.f + frac(_vUV.x);
    
    if (1.f < _vUV.y)
        _vUV.y = frac(_vUV.y);
    else if (_vUV.y < 0.f)
        _vUV.x = 1.f + frac(_vUV.x);
    
    int2 iUV = _vUV * g_vNoiseResolution;
    iUV -= int2(2, 2);
    
    for (int j = 0; j < 5; ++j)
    {
        for (int i = 0; i < 5; ++i)
        {
            int2 idx = int2(iUV.y + i, iUV.x + j);
            vOutColor += g_Noise[idx] * GaussianFilter[j][i];
        }
    }
    
    return vOutColor;
}


// 0 ~ 1 사이 값을 반환
float Random(int key)
{
    float fValue = 0.f;
    
    return fValue;
}

// flaot4를 float로 저장
float encode(float4 _value)
{
    uint rgba = (uint(_value.x * 255.f) << 24) + (uint(_value.y * 255.f) << 16) + (uint(_value.z * 255.f) << 8) + uint(_value.w * 255.f);
    
    // uint형태인데 비트는 float그대로
    return asfloat(rgba);
}

float4 decode(float _value)
{
    uint rgba = asint(_value);
    float r = float((rgba & 0xff000000) >> 24) / 255.f;
    float g = float((rgba & 0x00ff0000) >> 16) / 255.f;
    float b = float((rgba & 0x0000ff00) >> 8) / 255.f;
    float a = float((rgba & 0x000000ff) >> 0) / 255.f;

    return float4(r, g, b, a);
}
float GetTessFactor(float3 _vPos, int _iMinLevel, int _iMaxLevel, float _MinDistance, float _MaxDistance)
{
    float fDistance = length(_vPos);
    
    if (_MaxDistance < fDistance)
        return _iMinLevel;
    if (fDistance < _MinDistance)
        return _iMaxLevel;
    
    float fLevel = _iMaxLevel - (_iMaxLevel - _iMinLevel) * ((fDistance - _MinDistance) / (_MaxDistance - _MinDistance));
    
    return fLevel;
}


int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);

    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    _fResult = t;

    float3 p = _vStart + t * _vDir;

    _vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }

    return 1;
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

matrix GetBoneOriginMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrOriginBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}



void Skinning(inout float3 _vPos, inout float4 _vWeight, inout float4 _vIndices, int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
}

static const float Weight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
    1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

static const float3x3 sobel_x =
{
    { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 }
};

static const float3x3 sobel_y =
{
    { -1, -2, -1 },
    {  0,  0,  0 },
    {  1,  2,  1 }
};

float intensity(in float4 color)
{
    return sqrt((color.x * color.x) + (color.y * color.y) + (color.z * color.z));

}

float RGBtoluma(float3 color)
{
    return sqrt(dot(color, float3(0.299, 0.587, 0.114)));
}

float rgb2luma(float3 color)
{
    return sqrt(dot(color, float3(0.299, 0.587, 0.114)));
}

float QUALITY(int level)
{
    float quality = 1.0f;
    for (int i = 1; i <= level; i++)
    {
        quality *= 0.5f;
    }
    return quality;
}

#endif


