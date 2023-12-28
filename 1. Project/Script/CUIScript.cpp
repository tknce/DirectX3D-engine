#include "pch.h"
#include "CUIScript.h"

CUIScript::CUIScript()
	:CScript(UISCRIPT)
{
}

CUIScript::~CUIScript()
{
}

void CUIScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"BAR", false);
	}
}

void CUIScript::tick()
{
}
