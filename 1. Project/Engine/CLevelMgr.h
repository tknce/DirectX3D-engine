#pragma once


class CLevel;
class CGameObject;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;

	float m_facctime;

	bool			m_a;
public:
	CLevel* GetCurLevel() { return m_pCurLevel; }
	void SetCurLevel(CLevel* _Level) { m_pCurLevel = _Level; }
	LEVEL_STATE GetLevelState();

	void setstate() { m_a = true; m_facctime = 0; }
public:
	void init();
	void Progress();

	
public:
	CGameObject* FindObjectByName(const wstring& _strName);
	CGameObject* FindObjectByID(int _ID);
	void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out);
	void FindObjectByPos(CGameObject* _Pos, vector<CGameObject*>& _out);

	void ChangeLevel(CLevel* _NextLevel);

private:
	void ChangeLevelState(LEVEL_STATE _State);

public:
	CLevelMgr();
	~CLevelMgr();

	friend class CEventMgr;
};

