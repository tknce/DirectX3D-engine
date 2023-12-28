#pragma once



class CPathMgr
	: public CSingleton<CPathMgr>
{
private:
	wchar_t		m_szContentPath[256];

public:
	void init();

	const wchar_t* GetContentPath() { return m_szContentPath; }
	string wstr_to_str(wstring _wstr)
	{
		return string().assign(_wstr.begin(), _wstr.end());
	}
	wstring str_to_wstr(string _str)
	{
		return wstring().assign(_str.begin(), _str.end());
	}

public:
	CPathMgr();
	~CPathMgr();
};

