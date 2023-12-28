#include "pch.h"
#include "CAIScript.h"


CAIScript::CAIScript()
    : CScript(AISCRIPT)
{
}

CAIScript::CAIScript(const CAIScript& _origin)
    : CScript(AISCRIPT)
{
}

CAIScript::~CAIScript()
{
}

void CAIScript::AddState()
{
}


void CAIScript::tick()
{

}

void CAIScript::IDLE()
{
    if(m_StateValue.enter == true)
    {
        m_StateValue.enter = false;
    }
    if(m_StateValue.tick == true)
    { 
    }

}

void CAIScript::TRACE()
{
    if (m_StateValue.enter == true)
    {
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
    }

}

void CAIScript::DEAD()
{
    if (m_StateValue.enter == true)
    {
        m_StateValue.enter = false;
    }
    if (m_StateValue.tick == true)
    {
    }

}

void CAIScript::exit()
{
    switch (m_State)
    {
    case State::IDLE:
        break;
    case State::TRACE:
        break;
    case State::DEAD:
        break;
    default:
        break;
    }
}

void CAIScript::ChangeState(State _State)
{
    exit();
    m_State = _State;
    m_StateValue.enter = true;
}