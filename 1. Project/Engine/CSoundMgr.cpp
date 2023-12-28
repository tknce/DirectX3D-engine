#include "pch.h"
#include "CSoundMgr.h"

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
}

void CSoundMgr::RigsterSound(Ptr<CSound> _psound)
{
	m_arrSound.push_back(_psound);
}

void CSoundMgr::SetVolume(wstring _soundname, float _f)
{
	for (size_t i = 0; i < m_arrSound.size(); ++i)
	{
		if (m_arrSound[i]->GetName() == _soundname)
		{
			m_arrSound[i]->SetVolume(_f, m_arrSound[i]->Getidx());
		}
	}
}

void CSoundMgr::SetallVolume(float _f)
{
	for (size_t i = 0; i < m_arrSound.size(); ++i)
	{
		m_arrSound[i]->SetVolume(_f, m_arrSound[i]->Getidx());
	}
}

void CSoundMgr::Stop()
{
	for (size_t i = 0; i < m_arrSound.size(); ++i)
	{
		m_arrSound[i]->Stop();
	}
	m_arrSound.clear();
}

Ptr<CSound> CSoundMgr::FindSound(wstring _name, int _RoofCount, float _fvolume, bool _overlab)
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(_name);

	CSoundMgr::GetInst()->RigsterSound(pSound);
	pSound->Play(_RoofCount, _fvolume, _overlab);

	return pSound;
}


