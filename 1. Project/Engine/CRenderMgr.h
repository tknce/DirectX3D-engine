#pragma once

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CMRT;
class CHDR;
//class CBloom;

#include "CLight2D.h"
#include "CLight3D.h"
#include "CTexture.h"
#include "CBloom.h"
class CRenderMgr
	: public CSingleton<CRenderMgr>
{
private:
	CMRT* m_arrMRT[(UINT)MRT_TYPE::END];

	vector<CCamera*>		m_vecCam;			// ���� ������ �ִ� ��� ī�޶�
	CCamera*				m_EditorCam;		// Editor �� ī�޶�

	vector<CLight2D*>		m_vecLight2D;		// ���� ������ �ִ� ��� 2D ����
	CStructuredBuffer*		m_pLight2DBuffer;	// 2D ���� ������ ������ ����ȭ ����

	vector<CLight3D*>		m_vecLight3D;		// ���� ������ �ִ� ��� 3D ����
	CStructuredBuffer*		m_pLight3DBuffer;	// 3D ���� ������ ������ ����ȭ ����

	vector<tDebugShapeInfo>	m_DebugDrawInfo;

	Ptr<CTexture>			m_RTCopyTex;



	CHDR*					m_HDR;
	CBloom*					m_Bloom;
	float					m_fLuminance;

public:
	void init();
	void tick();
	void render();

	void UpdateHLOOM();

	Ptr<CTexture> GetTex() { return m_Bloom->GetTex(); }

private:
	void render_game();
	void render_editor();
	void render_dynamic_shadowdepth();

	void UpdateNoiseTexture();
	void UpdateLight2D();
	void UpdateLight3D();

	// shadereffect
	void UpdateHDR();
	

	void CreateMRT();
	void ClearMRT();
	

public:
	CCamera* GetMainCam();
	CMRT* GetMRT(MRT_TYPE _type) { return m_arrMRT[(UINT)_type]; }
	const vector<CLight3D*>& GetLight3D() { return m_vecLight3D; }

	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterEditorCamera(CCamera* _pCam) { m_EditorCam = _pCam; }

	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D); }
	int RegisterLight3D(CLight3D* _pLight3D)
	{
		m_vecLight3D.push_back(_pLight3D);
		return m_vecLight3D.size() - 1;
	}

	float GetLuminance() { return m_fLuminance; }


	// ����Ÿ���� ī���ؽ��ķ� ����
	void CopyRenderTarget();

	void DebugDraw(DEBUG_SHAPE _eShape, Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fRadius, float _fDuration) 
	{
#ifdef _DEBUG
		m_DebugDrawInfo.push_back(tDebugShapeInfo{ _eShape , _vColor, _vPosition , _vScale , _vRotation , _fRadius , _fDuration , 0.f });
#endif
	}

	vector<tDebugShapeInfo>& GetDebutgDrawInfo() { return m_DebugDrawInfo;}


public:
	CRenderMgr();
	~CRenderMgr();
};

