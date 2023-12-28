#include "pch.h"
#include "CAI.h"

void AI::AddState(const wstring& _strKey, CState* _pState)
{
}

CState* AI::FindState(const wstring& _strKey)
{
	return nullptr;
}

void AI::ChangeState(const wstring& _strKey)
{
}

void AI::tick()
{
}

void AI::finaltick()
{
}

//void AI::SaveToFile(FILE* _File)
//{
//}
//
//void AI::LoadFromFile(FILE* _File)
//{
//	
//}

AI::AI()
	:CComponent(COMPONENT_TYPE::AI)
{
}

AI::AI(const AI& _origin)
	:CComponent(COMPONENT_TYPE::AI)
{
}

AI::~AI()
{
}
