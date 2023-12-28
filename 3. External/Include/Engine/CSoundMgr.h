#pragma once
#include "singleton.h"
class CSoundMgr :
    public CSingleton<CSoundMgr>
{
private:
    vector<Ptr<CSound>>     m_arrSound;

public:
    void RigsterSound(Ptr<CSound> _psound);
    void SetVolume(wstring _soundname, float _f);
    void SetallVolume(float _f);
    void Stop();

    Ptr<CSound> FindSound(wstring _name, int _RoofCount, float _fvolume = 1.f , bool _overlab = false);

    CSoundMgr();
    ~CSoundMgr();
};

