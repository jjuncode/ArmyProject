#include "Script.h"

uint32_t Script::id_cnt{};
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
