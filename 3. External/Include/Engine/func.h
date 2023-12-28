#pragma once

#ifndef  FUNC_H_
#define  FUNC_H_



template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	typename vector<T*>::iterator iter = _vec.begin();
	for (; iter != _vec.end(); ++iter)
	{
		if (nullptr != (*iter))
			delete (*iter);
	}
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}

template<typename T, int SIZE>
void Safe_Del_Array(T* (&_arr)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}

// Relative Path 가져오기
wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath);

string WStringToString(const wstring& _str);

int GetSizeofFormat(DXGI_FORMAT _eFormat);

class CGameObject;
bool IsValid(CGameObject*& _pObj);

class CEntity;
inline bool IsValid(CEntity* _pCom)
{
	if (nullptr == _pCom)
		return false;
	return true;
}


Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);




// ================
// File Save / Load
// ================
void SaveStringToFile(const string& _str, FILE* _pFile);
void LoadStringFromFile(string& _str, FILE* _pFile);

void SaveWStringToFile(const wstring& _str, FILE* _pFile);
void LoadWStringFromFile(wstring& _str, FILE* _pFile);


#include "CResMgr.h"
#include "Ptr.h"
template<typename T>
void SaveResourceRef(Ptr<T> _res, FILE* _pFile)
{
	int bExist = !!_res.Get();
	fwrite(&bExist, sizeof(int), 1, _pFile);

	if (bExist)
	{
		SaveWStringToFile(_res->GetKey(), _pFile);
		SaveWStringToFile(_res->GetRelativePath(), _pFile);
	}
}

template<typename T>
void LoadResourceRef(Ptr<T>& _Res, FILE* _pFile)
{
	int bExist = 0;
	fread(&bExist, sizeof(int), 1, _pFile);

	if (bExist)
	{
		wstring strKey, strRelativePath;
		LoadWStringFromFile(strKey, _pFile);
		LoadWStringFromFile(strRelativePath, _pFile);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWString(COMPONENT_TYPE);
const char* ToString(RES_TYPE);
const wchar_t* ToWString(RES_TYPE);


// ===============
// Event 관련 함수
// ===============
class CGameObject;
void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx = 0);
void DeleteComponent(CGameObject* _pObject, COMPONENT_TYPE _pDelCom);
void ChangeIDX(CGameObject* _pObject, int _IDX);
class CRes;
void AddRes(CRes* _Res, RES_TYPE _Type);
void DeleteRes(CRes* _Res, RES_TYPE _Type);

#include "CLevel.h"
void ChangeLevel(CLevel* _Level);

// ==========
// Debug Draw
// ==========
#ifdef _DEBUG
void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration = 0.f);
void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration = 0.f);
void DebugDrawCube(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration = 0.f);
void DebugDrawSphere(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration = 0.f);
void DebugDrawCone(Vec4 _vColor, Vec3 _vPosition, Vec3 _vRotation, float _fRadius, float _fDuration = 0.f);

#endif


float valueReverse(float _Value);
Vec3 DecomposeRotMat(const Matrix& _matRot);


//string wstr_to_str_ver_0(const wstring& wstr_sourse);
//wstring str_to_wstr_ver_0(const string& str_sourse);

#endif