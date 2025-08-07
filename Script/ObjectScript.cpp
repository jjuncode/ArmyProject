#include "ObjectScript.h"

#include "../Component/AnimationComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void ObjectScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{50};
    uint32_t player_rotate_speed{50};

    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    transform.AddLocalRotate(Vec3(0,1,0) * dt * player_rotate_speed);
}

void ObjectScript::ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv,float dt)
{
}

void ObjectScript::ExecuteCollStay(uint32_t other_entity_id, MTV _mtv,float dt)
{
}

void ObjectScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
}
