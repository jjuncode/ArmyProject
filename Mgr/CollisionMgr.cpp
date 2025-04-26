#include "CollisionMgr.h"
#include "SceneMgr.h"

#include "../Component/ColliderComponent.h"
#include "../Component/TransformComponent.h"

void CollisionMgr::Collision(float dt)
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
                 
                CollisionCheck(vec_entity_left, vec_entity_right, dt);
            }
        }
    }
}

void CollisionMgr::CollisionCheck(const std::list<uint32_t> &left, const std::list<uint32_t> &right, float dt)
{
    for ( const auto& left_id : left ) {
        for ( const auto& right_id : right ){
            auto left_coll = SceneMgr::GetComponent<ColliderComponent>(left_id);
            auto right_coll = SceneMgr::GetComponent<ColliderComponent>(right_id);
            
            // Only Activate Entity
            if ( left_coll && right_coll ) {
                if (SceneMgr::GetCurScene()->GetEntityStatus(left_coll->GetOwnerID()) != EntityStatus::kActive 
                    || SceneMgr::GetCurScene()->GetEntityStatus(right_coll->GetOwnerID()) != EntityStatus::kActive)
                    continue;
                else{
                    auto coll_info = CollisionLogic(left_coll.get(), right_coll.get());
                    if (coll_info.first){
                        // Collision 
                        left_coll->Collision(right_coll->GetOwnerID(), coll_info.second,dt);
                        right_coll->Collision(left_coll->GetOwnerID(), coll_info.second,dt);
                    }
                    else{
                        // Not Collision
                        auto left_entity_id = left_coll->GetOwnerID();
                        auto right_entity_id = right_coll->GetOwnerID();

                        auto left_status = left_coll->GetCollisionStatus(right_entity_id);
                        auto right_status = right_coll->GetCollisionStatus(left_entity_id);
                        
                        if ( left_status == CollisionStatus::kStay){
                            left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kExit);
                            left_coll->CollisionExit(right_entity_id, coll_info.second,dt);
                        }
                        else if ( left_status == CollisionStatus::kExit)
                            left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kNone);
                        else if ( left_status == CollisionStatus::kEnter ) {
                            left_coll->SetCollisionStatus(right_entity_id, CollisionStatus::kExit);
                            left_coll->CollisionExit(right_entity_id, coll_info.second,dt);
                        }

                        if ( right_status == CollisionStatus::kStay ) {
                            right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kExit);
                            right_coll->CollisionExit(left_entity_id,coll_info.second, dt);
                        }
                        else if ( right_status == CollisionStatus::kExit)
                            right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kNone);
                        else if ( right_status == CollisionStatus::kEnter ) {
                            right_coll->SetCollisionStatus(left_entity_id, CollisionStatus::kExit);
                            right_coll->CollisionExit(left_entity_id, coll_info.second,dt);
                        }

                    }
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
    auto transform_left = SceneMgr::GetComponent<TransformComponent>(left->GetOwnerID());
    auto transform_right = SceneMgr::GetComponent<TransformComponent>(right->GetOwnerID());

    std::cout << "POS LEFT : " << transform_left->GetPos().x << " " << transform_left->GetPos().y << std::endl;
    std::cout << "POS RIGHT : " << transform_right->GetPos().x << " " << transform_right->GetPos().y << std::endl;

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
