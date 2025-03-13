#include "RectObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"

void RectObject::SetShape()
{
    auto render = SceneMgr::GetComponentOrigin<RenderComponent>(obj.GetEntityID());
    render->SetShape<sf::RectangleShape>(sf::Vector2f(1,1));
}

void RectObject::SetScript()
{
}
