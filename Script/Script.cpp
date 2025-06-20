#include "Script.h"

std::stack<uint32_t> Script::remain_id{};

void Script::Execute(float dt)
{
}

void Script::ExecuteCollEnter(uint32_t other_entity_id,MTV _mtv, float dt)
{
}

void Script::ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
}

void Script::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
}

Script::Script()
{
    static uint32_t id_cnt{};

    if (!remain_id.size())
    {
        // Is Empty
        m_id = id_cnt++;
    }
    else
    {
        m_id = remain_id.top();
        remain_id.pop();
    }
}
