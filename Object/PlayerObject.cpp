#include "PlayerObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/PlayerScript.h"


void PlayerObject::SetShape()
{
    auto render = SceneMgr::GetComponentOrigin<RenderComponent>(obj.GetEntityID());
    render->SetShape<sf::RectangleShape>(sf::Vector2f(1,1));
}

void PlayerObject::SetScript()
{
    auto& cur_scene = SceneMgr::GetCurScene();
    auto script = obj.AddComponent<PlayerScript>();
    cur_scene ->AddComponent<PlayerScript>(std::move(script));
}
