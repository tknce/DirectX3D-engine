#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;

class CAnimator3D :
    public CComponent
{
private:
    // ���Ϳ� �����͸� ���� ����
    // ū ���͸� �Լ��� �����ϰų� ������ ���� �迭�� ���� �� ������ �� �ִ�.
    // �޸� ��뷮�� ���̰� ���α׷� ȿ������ �����ϴ� �� ������ �� ���� �ֱ� �����̴�.
    // ����
    // �޸� ���� �Ǵ� �̹� ������ �޸𸮿� �������� ���ɼ��� �����Ƿ� �̷��� ������ �����Ϸ��� ������ �޸� ���� ����� ����ؾ� �Ѵ�.
    const vector<tMTBone>* m_pVecBones;
    const vector<tMTAnimClip>* m_pVecClip;


    // �ִϸ��̼� ����� �ؾߵȴ�.
    // �� �� ��� ��� �ִϸ��̼� ����ǰ� �Ѵ�.
    // ����ؾߵ� ���
    // wstring			strAnimName;    �̸� �ֱ� ã�ƾ� ��
    // int				iStartFrame;    �ð� ������ �ڵ����� ���  dStartTime * 30
    // int				iEndFrame;      �ð� ������ �ڵ����� ���  dEndTime * 30
    // int				iFrameLength;   ���� �ð� ������ �ð� ������ �ڵ����� ���    iEndFrame - iStartFrame
    // 
    // double			dStartTime;     ���������� �־���ߵ� ��   iStartFrame / 30
    // double			dEndTime;       ���������� �־���ߵ� ��   dEndTime / 30
    // double			dTimeLength;    EndTime - StartTime
    // FbxTime::EMode	eMode;          �����Ӹ�忡 ���� �������� �޶���
    map<wstring, CAnimation3D*> m_mapAnim;
    CAnimation3D*               m_pCurAnim;

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat;      // �ؽ��Ŀ� ������ ���� �������
    int							m_iFrameCount;          // 30
    double						m_dCurTime;
    int							m_iCurClip;             // Ŭ�� �ε���	

    int							m_iFrameIdx;            // Ŭ���� ���� ������
    int							m_iNextFrameIdx;        // Ŭ���� ���� ������
    float						m_fRatio;	            // ������ ���� ����

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    CStructuredBuffer*          m_pBoneOriginMatBuffer; // Ư�� �������� ���� ���
    bool						m_bFinalMatUpdate;      // ������� ���� ���࿩��

    bool                        m_bStop;
    bool                        m_bSocket;

    // �޽������� �ҷ�����
    wstring                     m_strMeshData;

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
    void SetStop(bool _stop) { m_bStop = _stop; }
    void SetTime(float _time) { m_vecClipUpdateTime[m_iCurClip] += _time; }
    void SetSocket(bool _set) { m_bSocket = _set; }
    void SetResMeshData(wstring _wstr) { m_strMeshData = _wstr; }
    void ReSetCurtime() { m_dCurTime = 0; }
    void SetMeshData();

    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    CStructuredBuffer* GetOriginBoneMat() { return m_pBoneOriginMatBuffer; }
    vector<Matrix> GetFinalVecMat() { return m_vecFinalBoneMat; }
    CAnimation3D* GetAnimation3D() { return m_pCurAnim; }
    map<wstring, CAnimation3D*> GetAnimlist() { return m_mapAnim; }
    const vector<tMTAnimClip>* GetentireAnimClip() { return m_pVecClip; }    
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    int GetFrame() { return m_iFrameIdx; }
    double GetCurTime() { return m_dCurTime; }

    void ClearData();

public:
    void SetAnimation3D(wstring animname, tMTAnimClip information);

    CAnimation3D* FindAnimation3D(wstring _strKey);
    void Play(wstring _strKey);


private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};

