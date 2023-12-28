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
    // 벡터에 포인터를 쓰는 이유
    // 큰 벡터를 함수에 전달하거나 벡터의 동적 배열을 만들 때 유용할 수 있다.
    // 메모리 사용량을 줄이고 프로그램 효율성을 개선하는 데 도움이 될 수도 있기 때문이다.
    // 단점
    // 메모리 누수 또는 이미 해제된 메모리에 엑세스할 가능성도 있으므로 이러한 문제를 방지하려면 적절한 메모리 관리 기술을 사용해야 한다.
    const vector<tMTBone>* m_pVecBones;
    const vector<tMTAnimClip>* m_pVecClip;


    // 애니메이션 등록을 해야된다.
    // 안 할 경우 모든 애니메이션 재생되게 한다.
    // 등록해야될 목록
    // wstring			strAnimName;    이름 넣기 찾아야 됌
    // int				iStartFrame;    시간 넣으면 자동으로 계산  dStartTime * 30
    // int				iEndFrame;      시간 넣으면 자동으로 계산  dEndTime * 30
    // int				iFrameLength;   시작 시간 끝나는 시간 넣으면 자동으로 계산    iEndFrame - iStartFrame
    // 
    // double			dStartTime;     실질적으로 넣어줘야될 것   iStartFrame / 30
    // double			dEndTime;       실질적으로 넣어줘야될 것   dEndTime / 30
    // double			dTimeLength;    EndTime - StartTime
    // FbxTime::EMode	eMode;          프레임모드에 따라 프레임이 달라짐
    map<wstring, CAnimation3D*> m_mapAnim;
    CAnimation3D*               m_pCurAnim;

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat;      // 텍스쳐에 전달할 최종 행렬정보
    int							m_iFrameCount;          // 30
    double						m_dCurTime;
    int							m_iCurClip;             // 클립 인덱스	

    int							m_iFrameIdx;            // 클립의 현재 프레임
    int							m_iNextFrameIdx;        // 클립의 다음 프레임
    float						m_fRatio;	            // 프레임 사이 비율

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    CStructuredBuffer*          m_pBoneOriginMatBuffer; // 특정 프레임의 원래 행렬
    bool						m_bFinalMatUpdate;      // 최종행렬 연산 수행여부

    bool                        m_bStop;
    bool                        m_bSocket;

    // 메쉬데이터 불러오기
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

