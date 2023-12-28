#include "pch.h"
#include "CAnimation3D.h"


CAnimation3D::CAnimation3D()
{
}

CAnimation3D::~CAnimation3D()
{
}


void CAnimation3D::SaveToFile(FILE* _File)
{
	CEntity::SaveToFile(_File);

	// 프레임 개수, 데이터
	size_t iFrameCount = m_pVecClip.size();
	fwrite(&iFrameCount, sizeof(size_t), 1, _File);
	// fwrite(m_pVecClip.data(), sizeof(tMTAnimClip), iFrameCount, _File);

	for (int i = 0; i < m_pVecClip.size(); ++i)
	{
		size_t iLen = m_pVecClip[i].strAnimName.length();
		fwrite(&iLen, sizeof(size_t), 1, _File);
		fwrite(m_pVecClip[i].strAnimName.data(), sizeof(wchar_t), iLen, _File);
		fwrite(&m_pVecClip[i].dStartTime, sizeof(double),1,_File);
		fwrite(&m_pVecClip[i].dEndTime, sizeof(double), 1, _File);
		fwrite(&m_pVecClip[i].dTimeLength, sizeof(double), 1, _File);
		fwrite(&m_pVecClip[i].iStartFrame, sizeof(int), 1, _File);
		fwrite(&m_pVecClip[i].iEndFrame, sizeof(int), 1, _File);
		fwrite(&m_pVecClip[i].iFrameLength, sizeof(int), 1, _File);
	}
}

//void CAnimation3D::SaveToJson(Json::Value& _value)
//{
//	/*_value["strName"] = string(m_pVecClip[i].strAnimName.begin(), m_pVecClip[i].strAnimName.end());
//	_value["iStartFrame"] = m_pVecClip[i].iStartFrame;
//	_value["iEndFrame"] = m_pVecClip[i].iEndFrame;
//	_value["iFrameLength"] = m_pVecClip[i].iFrameLength;
//	_value["dStartTime"] = m_pVecClip[i].dStartTime;
//	_value["dEndTime"] = m_pVecClip[i].dEndTime;
//	_value["dTimeLength"] = m_pVecClip[i].dTimeLength;*/
//}

void CAnimation3D::LoadFromFile(FILE* _File)
{
	CEntity::LoadFromFile(_File);

	// 프레임 개수, 데이터
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, _File);
	for (int i = 0; i < iFrameCount; ++i)
	{
		tMTAnimClip frm;
		size_t iLen = 0;
		fread(&iLen, sizeof(size_t), 1, _File);
		wchar_t szBuffer[256] = {};
		fread(szBuffer, sizeof(wchar_t), iLen, _File);
		frm.strAnimName = szBuffer;
		fread(&frm.dStartTime, sizeof(double), 1, _File);
		fread(&frm.dEndTime, sizeof(double), 1, _File);
		fread(&frm.dTimeLength, sizeof(double), 1, _File);
		fread(&frm.iStartFrame, sizeof(int), 1, _File);
		fread(&frm.iEndFrame, sizeof(int), 1, _File);
		fread(&frm.iFrameLength, sizeof(int), 1, _File);
		SetAnim3D(frm);
	}
}

//void CAnimation3D::LoadFromJson(Json::Value& _value)
//{
//	/*tMTAnimClip frm;
//	string a = _value["strName"].asCString();
//	frm.strAnimName =	wstring(a.begin(), a.end());
//	frm.iStartFrame =	_value["iStartFrame"].asInt();
//	frm.iEndFrame	=	_value["iEndFrame"].asInt();
//	frm.iFrameLength =	_value["iFrameLength"].asInt();
//	frm.dStartTime =	_value["dStartTime"].asDouble();
//	frm.dEndTime =		_value["dEndTime"].asDouble();
//	frm.dTimeLength =	_value["dTimeLength"].asDouble();
//
//	SetAnim3D(frm);*/
//}
