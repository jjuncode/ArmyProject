#include "Component.h"

std::stack<uint32_t> Component::remain_id{};

void Component::Update(float dt)
{
}

void Component::UpdateLate(float dt)
{
}

void Component::Render()
{
}

Component::Component()
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