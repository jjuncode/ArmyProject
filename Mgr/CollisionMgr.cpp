#include "CollisionMgr.h"
#include "SceneMgr.h"

#include "../Component/ColliderComponent.h"
#include "../Component/TransformComponent.h"


void CollisionMgr::Collision()
{
    const auto& cur_scene = SceneMgr::GetCurScene();

    const auto& layer = cur_scene->GetCollisionLayer();
    for (int i=0; i<static_cast<int>(CollisionEntityType::kEND); ++i){
        for (int j=i; j<static_cast<int>(CollisionEntityType::kEND); ++j){
            if ( layer[i][j] ){
                // collision check
                // that layer has to be collision
                const auto& vec_entity_left = cur_scene->GetCollisionEntity(i);
                const auto& vec_entity_right = cur_scene->GetCollisionEntity(j);
                 
                CollisionCheck(vec_entity_left, vec_entity_right);
            }
        }
    }
}

void CollisionMgr::CollisionCheck(const std::vector<uint32_t> &left, const std::vector<uint32_t> &right)
{
    for ( const auto& left_id : left ) {
        for ( const auto& right_id : right ){
            auto left_coll = SceneMgr::GetComponentOrigin<ColliderComponent>(left_id);
            auto right_coll = SceneMgr::GetComponentOrigin<ColliderComponent>(right_id);
            
            // Only Activate Entity
            if ( left_coll && right_coll ) {
                if (SceneMgr::GetCurScene()->GetEntityStatus(left_coll->GetID()) != EntityStatus::kActive 
                    || SceneMgr::GetCurScene()->GetEntityStatus(right_coll->GetID()) != EntityStatus::kActive)
                    continue;
                else{
                    if (CollisionLogic(left_coll.get(), right_coll.get())){
                        left_coll->Collision(right_coll->GetOwnerID());
                        right_coll->Collision(left_coll->GetOwnerID());
                    }
                }
            }
        }
    }
}

bool CollisionMgr::CollisionLogic(ColliderComponent *left, ColliderComponent *right)
{
    auto transform_left = SceneMgr::GetComponentOrigin<TransformComponent>(left->GetOwnerID());
    auto transform_right = SceneMgr::GetComponentOrigin<TransformComponent>(right->GetOwnerID());

    auto pos_left = transform_left->GetPos();
    auto pos_right = transform_right->GetPos();

    auto scale_left = transform_left->GetScale();
    auto scale_right = transform_right->GetScale();

    // Left AABB
    // float left1 = pos_left.x - scale_left.x / 2.0f;
    // float right1 = pos_left.x + scale_left.x / 2.0f;
    // float bottom1 = pos_left.y - scale_left.y / 2.0f;
    // float top1 = pos_left.y + scale_left.y / 2.0f;

    // Left AABB
    Vec2f l_left_bot = pos_left;
    Vec2f l_right_top = pos_left+scale_left;

    // Right AABB
    Vec2f r_left_bot = pos_right;
    Vec2f r_right_top = pos_right + scale_right;

    // AABB Collision
    // if (l_left_bot.x > r_right_top.x || l_left_bot.y > r_right_top.y 
    //     || l_right_top.x < r_left_bot.x || l_right_top.y < r_left_bot.y) {
    //     return false; 
    // }
    // In GRID Collision Check
    if (l_left_bot.x >= r_right_top.x || l_left_bot.y >= r_right_top.y 
        || l_right_top.x <= r_left_bot.x || l_right_top.y <= r_left_bot.y) {
        return false; 
    }
    return true; 
}
