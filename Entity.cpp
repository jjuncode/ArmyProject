#include "pch.h"
#include "Entity.h"
#include "Core.h"

uint Entity::id_cnt{};
std::stack<uint> Entity::remain_id{};
