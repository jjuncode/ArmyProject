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

void CollisionMgr::CollisionCheck(const std::list<uint32_t> &left, const std::list<uint32_t> &right)
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
                        // Collision 
                        left_coll->Collision(right_coll->GetOwnerID());
                        right_coll->Collision(left_coll->GetOwnerID());
                    }
                    else{
                        // Not Collision
                        auto left_entity_id = left_coll->GetOwnerID();
                        auto right_entity_id = right_coll->GetOwnerID();

                        auto left_status = left_coll->GetCollisionStatus(right_entity_id);
                        auto right_status = right_coll->GetCollisionStatus(left_entity_id);
                        
                        CollisionInfoID info;
                        info.left = left_entity_id;
                        info.right = right_entity_id;

                        if ( left_status == CollisionStatus::kStay){
                            left_coll->CollisionExit(info);
                        }

                        if ( right_status == CollisionStatus::kStay ) {
                            right_coll->CollisionExit(info);
                        }

                    }
                }
            }
        }
    }
}

bool CollisionMgr::CollisionLogic(ColliderComponent *left, ColliderComponent *right)
{
    // OBB Collision
    return OBBCollision_Logic(left, right);
}

bool CollisionMgr::OBBCollision_Logic(ColliderComponent *left, ColliderComponent *right)
{
    auto transform_left = SceneMgr::GetComponentOrigin<TransformComponent>(left->GetOwnerID());
    auto transform_right = SceneMgr::GetComponentOrigin<TransformComponent>(right->GetOwnerID());

    auto left_obb = left->GetOBB();
    auto right_obb = right->GetOBB();

    MTV mtv{Vec2{}, std::numeric_limits<float>::max()};

    std::vector<Vec2> vec_check{left_obb.width_half, left_obb.height_half
                                , right_obb.width_half, right_obb.height_half};

    for(const auto& v : vec_check){
        // standard vector
        auto vec_unit = Vec::Normalize(v);
        vec_unit = Vec::Normal(vec_unit);
        
        // center Proj vec
        Vec2 center_vec = transform_left->GetPos() - transform_right->GetPos();
        Vec2 center_vec_proj= Vec::Projection(vec_unit,center_vec);   // right is fixed

        // Get Corner Proj Vec
        // left projection
        auto left_proj_width = Vec::Projection(vec_unit, left_obb.width_half);
        auto left_proj_height = Vec::Projection(vec_unit, left_obb.height_half);

        // right projection
        auto right_proj_width = Vec::Projection(vec_unit, right_obb.width_half);
        auto right_proj_height = Vec::Projection(vec_unit, right_obb.height_half);

        float corner_proj_left = Vec::Length(left_proj_width) +  Vec::Length(left_proj_height);
        float corner_proj_right =  Vec::Length(right_proj_width) +  Vec::Length(right_proj_height);
        float sum_corner = corner_proj_left + corner_proj_right;

        auto length = sqrt(Vec::LengthSquare(center_vec_proj)) - sum_corner;

        // Not Collision
        if ( length >= 0){
            return false;
        }
        
        // Collision 
        if (mtv.length > abs(length)){
            mtv.length = abs(length);

            // Set MTV correct direction
            auto result = Vec::Dot(vec_unit, center_vec_proj);
            if ( result < 0 )
                mtv.vec = Vec::Reverse(vec_unit);
            else
                mtv.vec = vec_unit;
        }
    }

    transform_left->AddPos(mtv.vec * mtv.length);
    return true;
}
