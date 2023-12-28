#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParent;    // �ֻ��� �θ�
    vector<CGameObject*>    m_vecObjects;   // �θ�, �ڽ� ������� ��� ������Ʈ
    int                     m_iLayerIdx;

public:
    void begin();
    void tick();
    void finaltick();  

public:
    void AddGameObject(CGameObject* _pObject);
    int GetLayerIdx() {return m_iLayerIdx;}

    const vector<CGameObject*>& GetParentObjects() { return m_vecParent; }
    const vector<CGameObject*>& GetObjects() { return m_vecObjects; }
    bool FindObjectName(wstring _Name); // �̹� �־��� ���

private:
    void RegisterObject(CGameObject* _pObj){m_vecObjects.push_back(_pObj);}
    void DeregisterObject(CGameObject* _pObj);
    void clear() { m_vecObjects.clear(); }


public:
    CLONE_DUMMY(CLayer);

public:
    CLayer();
    ~CLayer();

    friend class CLevel;
    friend class CGameObject;
};

