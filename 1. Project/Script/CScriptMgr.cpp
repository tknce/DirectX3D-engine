#include "pch.h"
#include "CScriptMgr.h"

#include "CAIScript.h"
#include "CBloodScript.h"
#include "CBrokenObject.h"
#include "CCamereScript.h"
#include "CClearScript.h"
#include "CClear_1Script.h"
#include "CDeadScript.h"
#include "CDoorScript.h"
#include "CFactory_1Script.h"
#include "CFloordownScript.h"
#include "CFloorScript.h"
#include "CGasterScript.h"
#include "CGrountScript.h"
#include "CGunProjScript.h"
#include "CGunsparkScript.h"
#include "CLastScript.h"
#include "CLighterScript.h"
#include "CMainScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CNongrabwallScript.h"
#include "CPlayerScript.h"
#include "CPompScript.h"
#include "CRazerScript.h"
#include "CStairScript.h"
#include "CTargetScript.h"
#include "CThreeDPlayerScript.h"
#include "CTileMapState.h"
#include "CTimerScript.h"
#include "CTitleScript.h"
#include "CTurnbackScript.h"
#include "CUIcursorScript.h"
#include "CUIIconScript.h"
#include "CUIScript.h"
#include "CUIskillScript.h"
#include "CVentScript.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAIScript");
	_vec.push_back(L"CBloodScript");
	_vec.push_back(L"CBrokenObject");
	_vec.push_back(L"CCamereScript");
	_vec.push_back(L"CClearScript");
	_vec.push_back(L"CClear_1Script");
	_vec.push_back(L"CDeadScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CFactory_1Script");
	_vec.push_back(L"CFloordownScript");
	_vec.push_back(L"CFloorScript");
	_vec.push_back(L"CGasterScript");
	_vec.push_back(L"CGrountScript");
	_vec.push_back(L"CGunProjScript");
	_vec.push_back(L"CGunsparkScript");
	_vec.push_back(L"CLastScript");
	_vec.push_back(L"CLighterScript");
	_vec.push_back(L"CMainScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNongrabwallScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPompScript");
	_vec.push_back(L"CRazerScript");
	_vec.push_back(L"CStairScript");
	_vec.push_back(L"CTargetScript");
	_vec.push_back(L"CThreeDPlayerScript");
	_vec.push_back(L"CTileMapState");
	_vec.push_back(L"CTimerScript");
	_vec.push_back(L"CTitleScript");
	_vec.push_back(L"CTurnbackScript");
	_vec.push_back(L"CUIcursorScript");
	_vec.push_back(L"CUIIconScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CUIskillScript");
	_vec.push_back(L"CVentScript");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAIScript" == _strScriptName)
		return new CAIScript;
	if (L"CBloodScript" == _strScriptName)
		return new CBloodScript;
	if (L"CBrokenObject" == _strScriptName)
		return new CBrokenObject;
	if (L"CCamereScript" == _strScriptName)
		return new CCamereScript;
	if (L"CClearScript" == _strScriptName)
		return new CClearScript;
	if (L"CClear_1Script" == _strScriptName)
		return new CClear_1Script;
	if (L"CDeadScript" == _strScriptName)
		return new CDeadScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CFactory_1Script" == _strScriptName)
		return new CFactory_1Script;
	if (L"CFloordownScript" == _strScriptName)
		return new CFloordownScript;
	if (L"CFloorScript" == _strScriptName)
		return new CFloorScript;
	if (L"CGasterScript" == _strScriptName)
		return new CGasterScript;
	if (L"CGrountScript" == _strScriptName)
		return new CGrountScript;
	if (L"CGunProjScript" == _strScriptName)
		return new CGunProjScript;
	if (L"CGunsparkScript" == _strScriptName)
		return new CGunsparkScript;
	if (L"CLastScript" == _strScriptName)
		return new CLastScript;
	if (L"CLighterScript" == _strScriptName)
		return new CLighterScript;
	if (L"CMainScript" == _strScriptName)
		return new CMainScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNongrabwallScript" == _strScriptName)
		return new CNongrabwallScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPompScript" == _strScriptName)
		return new CPompScript;
	if (L"CRazerScript" == _strScriptName)
		return new CRazerScript;
	if (L"CStairScript" == _strScriptName)
		return new CStairScript;
	if (L"CTargetScript" == _strScriptName)
		return new CTargetScript;
	if (L"CThreeDPlayerScript" == _strScriptName)
		return new CThreeDPlayerScript;
	if (L"CTileMapState" == _strScriptName)
		return new CTileMapState;
	if (L"CTimerScript" == _strScriptName)
		return new CTimerScript;
	if (L"CTitleScript" == _strScriptName)
		return new CTitleScript;
	if (L"CTurnbackScript" == _strScriptName)
		return new CTurnbackScript;
	if (L"CUIcursorScript" == _strScriptName)
		return new CUIcursorScript;
	if (L"CUIIconScript" == _strScriptName)
		return new CUIIconScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	if (L"CUIskillScript" == _strScriptName)
		return new CUIskillScript;
	if (L"CVentScript" == _strScriptName)
		return new CVentScript;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{

	case (UINT)SCRIPT_TYPE::AISCRIPT:
		return new CAIScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOODSCRIPT:
		return new CBloodScript;
		break;
	case (UINT)SCRIPT_TYPE::BROKENOBJECT:
		return new CBrokenObject;
		break;
	case (UINT)SCRIPT_TYPE::CAMERESCRIPT:
		return new CCamereScript;
		break;
	case (UINT)SCRIPT_TYPE::CLEARSCRIPT:
		return new CClearScript;
		break;
	case (UINT)SCRIPT_TYPE::CLEAR_1SCRIPT:
		return new CClear_1Script;
		break;
	case (UINT)SCRIPT_TYPE::DEADSCRIPT:
		return new CDeadScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::FACTORY_1SCRIPT:
		return new CFactory_1Script;
		break;
	case (UINT)SCRIPT_TYPE::FLOORDOWNSCRIPT:
		return new CFloordownScript;
		break;
	case (UINT)SCRIPT_TYPE::FLOORSCRIPT:
		return new CFloorScript;
		break;
	case (UINT)SCRIPT_TYPE::GASTERSCRIPT:
		return new CGasterScript;
		break;
	case (UINT)SCRIPT_TYPE::GROUNTSCRIPT:
		return new CGrountScript;
		break;
	case (UINT)SCRIPT_TYPE::GUNPROJSCRIPT:
		return new CGunProjScript;
		break;
	case (UINT)SCRIPT_TYPE::GUNSPARKSCRIPT:
		return new CGunsparkScript;
		break;
	case (UINT)SCRIPT_TYPE::LASTSCRIPT:
		return new CLastScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTERSCRIPT:
		return new CLighterScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINSCRIPT:
		return new CMainScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::NONGRABWALLSCRIPT:
		return new CNongrabwallScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::POMPSCRIPT:
		return new CPompScript;
		break;
	case (UINT)SCRIPT_TYPE::RAZERSCRIPT:
		return new CRazerScript;
		break;
	case (UINT)SCRIPT_TYPE::STAIRSCRIPT:
		return new CStairScript;
		break;
	case (UINT)SCRIPT_TYPE::TARGETSCRIPT:
		return new CTargetScript;
		break;
	case (UINT)SCRIPT_TYPE::THREEDPLAYERSCRIPT:
		return new CThreeDPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::TILEMAPSTATE:
		return new CTileMapState;
		break;
	case (UINT)SCRIPT_TYPE::TIMERSCRIPT:
		return new CTimerScript;
		break;
	case (UINT)SCRIPT_TYPE::TITLESCRIPT:
		return new CTitleScript;
		break;
	case (UINT)SCRIPT_TYPE::TURNBACKSCRIPT:
		return new CTurnbackScript;
		break;
	case (UINT)SCRIPT_TYPE::UICURSORSCRIPT:
		return new CUIcursorScript;
		break;
	case (UINT)SCRIPT_TYPE::UIICONSCRIPT:
		return new CUIIconScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
		break;
	case (UINT)SCRIPT_TYPE::UISKILLSCRIPT:
		return new CUIskillScript;
		break;
	case (UINT)SCRIPT_TYPE::VENTSCRIPT:
		return new CVentScript;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AISCRIPT:
		return L"CAIScript";
		break;

	case SCRIPT_TYPE::BLOODSCRIPT:
		return L"CBloodScript";
		break;

	case SCRIPT_TYPE::BROKENOBJECT:
		return L"CBrokenObject";
		break;

	case SCRIPT_TYPE::CAMERESCRIPT:
		return L"CCamereScript";
		break;

	case SCRIPT_TYPE::CLEARSCRIPT:
		return L"CClearScript";
		break;

	case SCRIPT_TYPE::CLEAR_1SCRIPT:
		return L"CClear_1Script";
		break;

	case SCRIPT_TYPE::DEADSCRIPT:
		return L"CDeadScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::FACTORY_1SCRIPT:
		return L"CFactory_1Script";
		break;

	case SCRIPT_TYPE::FLOORDOWNSCRIPT:
		return L"CFloordownScript";
		break;

	case SCRIPT_TYPE::FLOORSCRIPT:
		return L"CFloorScript";
		break;

	case SCRIPT_TYPE::GASTERSCRIPT:
		return L"CGasterScript";
		break;

	case SCRIPT_TYPE::GROUNTSCRIPT:
		return L"CGrountScript";
		break;

	case SCRIPT_TYPE::GUNPROJSCRIPT:
		return L"CGunProjScript";
		break;

	case SCRIPT_TYPE::GUNSPARKSCRIPT:
		return L"CGunsparkScript";
		break;

	case SCRIPT_TYPE::LASTSCRIPT:
		return L"CLastScript";
		break;

	case SCRIPT_TYPE::LIGHTERSCRIPT:
		return L"CLighterScript";
		break;

	case SCRIPT_TYPE::MAINSCRIPT:
		return L"CMainScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::NONGRABWALLSCRIPT:
		return L"CNongrabwallScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::POMPSCRIPT:
		return L"CPompScript";
		break;

	case SCRIPT_TYPE::RAZERSCRIPT:
		return L"CRazerScript";
		break;

	case SCRIPT_TYPE::STAIRSCRIPT:
		return L"CStairScript";
		break;

	case SCRIPT_TYPE::TARGETSCRIPT:
		return L"CTargetScript";
		break;

	case SCRIPT_TYPE::THREEDPLAYERSCRIPT:
		return L"CThreeDPlayerScript";
		break;

	case SCRIPT_TYPE::TILEMAPSTATE:
		return L"CTileMapState";
		break;

	case SCRIPT_TYPE::TIMERSCRIPT:
		return L"CTimerScript";
		break;

	case SCRIPT_TYPE::TITLESCRIPT:
		return L"CTitleScript";
		break;

	case SCRIPT_TYPE::TURNBACKSCRIPT:
		return L"CTurnbackScript";
		break;

	case SCRIPT_TYPE::UICURSORSCRIPT:
		return L"CUIcursorScript";
		break;

	case SCRIPT_TYPE::UIICONSCRIPT:
		return L"CUIIconScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	case SCRIPT_TYPE::UISKILLSCRIPT:
		return L"CUIskillScript";
		break;

	case SCRIPT_TYPE::VENTSCRIPT:
		return L"CVentScript";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}