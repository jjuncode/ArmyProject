#include "CollisionMgr.h"
#include "SceneMgr.h"

#include "../Component/ColliderComponent.h"

void CollisionMgr::Collision(float dt)
{
    const auto& layer = SceneMgr::GetCollisionLayer();
    for (int i=0; i<static_cast<int>(CollisionObjectType::kEND); ++i){
        for (int j=i; j<static_cast<int>(CollisionObjectType::kEND); ++j){
            if ( layer[i][j] ){
                // collision check
                // that layer has to be collision
                const auto& vec_object_left = SceneMgr::GetCollisionObject(i);
                const auto& vec_object_right = SceneMgr::GetCollisionObject(j);

                // not Same Group
                if (i != j){
                    for (const auto &left_id : vec_object_left){
                        for (const auto &right_id : vec_object_right){
                            if (left_id == right_id)
                                continue;

                            auto left_coll = SceneMgr::GetComponent<ColliderComponent>(left_id);
                            auto right_coll = SceneMgr::GetComponent<ColliderComponent>(right_id);
                            CollisionCheck(left_coll.get(), right_coll.get(), dt);
                        }
                    }
                }
                else{
                    // same group
                    for (auto i = vec_object_left.begin(); i != vec_object_left.end(); ++i){
                        for (auto j = i; j != vec_object_left.end(); ++j){
                            if (i == j)
                                continue;
                            // id setting
                            auto id_left = *i;
                            auto id_right = *j;

                            auto left_coll = SceneMgr::GetComponent<ColliderComponent>(id_left);
                            auto right_coll = SceneMgr::GetComponent<ColliderComponent>(id_right);
                            CollisionCheck(left_coll.get(), right_coll.get(), dt);
                        }
                    }
                }
            }
        }
    }
}

void CollisionMgr::CollisionCheck(ColliderComponent *left_coll, ColliderComponent *right_coll, float dt)
{
    // Only Activate Entity
    if (left_coll && right_coll){
        if (SceneMgr::IsActiveObject(left_coll->GetOwnerID()) || SceneMgr::IsActiveObject(right_coll->GetOwnerID()))
            return;
        else{
            auto coll_info = CollisionLogic(left_coll, right_coll);
            if (coll_info.first){
                // Collision
                left_coll->Collision(right_coll->GetOwnerID(), coll_info.second, dt);   // MTV -> left one is moving
                right_coll->Collision(left_coll->GetOwnerID(), coll_info.second, dt);   // MTV -> right one is fixed
            }
            else{
                // Not Collision
                auto left_entity_id = left_coll->GetOwnerID();
                auto right_entity_id = right_coll->GetOwnerID();

                auto left_status = left_coll->GetCollisionStatus(right_entity_id);
                auto right_status = right_coll->GetCollisionStatus(left_entity_id);

                if (left_status == CollisionStatus::kStay){
                    left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kExit);
                    left_coll->CollisionExit(right_entity_id, coll_info.second, dt);
                }
                else if (left_status == CollisionStatus::kExit)
                    left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kNone);
                else if (left_status == CollisionStatus::kEnter){
                    left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kExit);
                    left_coll->CollisionExit(right_entity_id, coll_info.second, dt);
                }

                if (right_status == CollisionStatus::kStay){
                    right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kExit);
                    right_coll->CollisionExit(left_entity_id, coll_info.second, dt);
                }
                else if (right_status == CollisionStatus::kExit)
                    right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kNone);
                else if (right_status == CollisionStatus::kEnter){
                    right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kExit);
                    right_coll->CollisionExit(left_entity_id, coll_info.second, dt);
                }
            }
        }
    }
}

std::pair<bool, MTV>  CollisionMgr::CollisionLogic(ColliderComponent *left, ColliderComponent *right)
{
    // OBB Collision
    return OBBCollision_Logic(left, right);
}

std::pair<bool, MTV>  CollisionMgr::OBBCollision_Logic(ColliderComponent *left, ColliderComponent *right)
{
    auto& transform_left = SceneMgr::GetObject(left->GetOwnerID()).GetTransform();
    auto& transform_right = SceneMgr::GetObject(right->GetOwnerID()).GetTransform();

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
        Vec2 center_vec = transform_left.GetPos() - transform_right.GetPos();
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
            return std::make_pair(false,MTV());
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

    return std::make_pair(true, mtv);
}
