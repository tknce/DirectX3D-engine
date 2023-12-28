#pragma once
#include "CComponent.h"

#include "CFrustum.h"


enum PROJ_TYPE
{
    PERSPECTIVE,
    ORTHOGRAHPICS,
};


class CGameObject;


class CCamera :
    public CComponent
{
private:
    CFrustum                m_Frustum;

    Matrix                  m_matView;      // �� ���
    Matrix                  m_matViewInv;   // �� �����
    Matrix                  m_matProj;      // ���� ���
    Matrix                  m_matProjInv;   // ���� �����

    PROJ_TYPE               m_eProjType;    // ���� ���
    float                   m_fWidth;       // ���� ���α���
    float                   m_fAspectRatio; // ��Ⱦ ��

    float                   m_fFar;         // ī�޶� �ִ� �þ� �Ÿ�
    float                   m_FOV;          // �þ߰�
    float                   m_fScale;       // ���� ���� ����

    tRay                    m_ray;      // ���콺 ������ ���ϴ� ����

    UINT                    m_iLayerMask;

    CGameObject*            m_pTarget;

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

    //vector<CGameObject*>    m_vecDeferred;
    //vector<CGameObject*>    m_vecQpaque;
    //vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;
    vector<CGameObject*>    m_vecDynamicShadow;     // ���� �׸��� ��ü

    int                     m_iCamIdx;  // ī�޶� �켱����



public:
    virtual void finaltick();
    virtual void finaltick_module();
    void render();


public:
    CFrustum& GetFrustum() { return m_Frustum; }
    int GetCamIdx() { return m_iCamIdx; }

    float GetOrthographicScale() { return m_fScale; }
    void SetOrthographicScale(float _fScale) { m_fScale = _fScale; }

    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    PROJ_TYPE GetProjType() { return m_eProjType; }

    void SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }
    float GetAspectRatio() { return m_fAspectRatio; }

    void SetFar(float _fFar) { m_fFar = _fFar; }
    float GetFar() { return m_fFar; }

    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }

    const tRay& GetRay() { return m_ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewMatInv() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjMatInv() { return m_matProjInv; }

    void SetLayerMask(const wstring& _strLayerName);
    void SetLayerMask(int _iLayerIdx);
    void SetLayerMaskAll() { m_iLayerMask = 0xffffffff; }
    void SetLayerMaskZero() { m_iLayerMask = 0; }
    
    void SetObject(CGameObject* _Object) { m_pTarget = _Object; }
public:
    void Adaptation();


protected:
    void CalcViewMat();
    void CalcProjMat();
    void CalRay();  // ���콺 �������� ���� ����

public:
    void SortObject();
    void SortShadowObject();
    void render_deferred();
    //void render_opaque();
    //void render_mask();
    void render_forward();
    void render_decal();
    void render_transparent();
    void render_postprocess();
    void render_depthmap();
    

public:
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    CLONE(CCamera);
public:
    CCamera();
    ~CCamera();
};

