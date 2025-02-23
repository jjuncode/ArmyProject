#include "../pch.h"
#include "Entity.h"
#include "../Core.h"

uint Entity::id_cnt{0};
std::stack<uint> Entity::remain_id{};
