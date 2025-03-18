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
                    auto collision_info = CollisionLogic(left_coll.get(), right_coll.get());
                    if (collision_info.is_collision){
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
                        
                        if ( left_status == CollisionStatus::kStay){
                            left_coll->CollisionExit(right_entity_id);
                        }

                        if ( right_status == CollisionStatus::kStay ) {
                            right_coll->CollisionExit(left_entity_id);
                        }

                    }
                }
            }
        }
    }
}

CollisionInfo CollisionMgr::CollisionLogic(ColliderComponent *left, ColliderComponent *right)
{
    // SAT Collision
    return SATCollision_Logic(left, right);
}

CollisionInfo CollisionMgr::SATCollision_Logic(ColliderComponent* left, ColliderComponent* right)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(left->GetOwnerID());
    auto transform_other = SceneMgr::GetComponentOrigin<TransformComponent>(right->GetOwnerID());

    const auto& left_vec_edge = left->GetEdge();
    const auto& right_vec_edge = right->GetEdge();

    MTV mtv{Vec2{}, std::numeric_limits<float>::max()};

    // self edge
    for (auto &_edge : left_vec_edge){
        Vec2 self_min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vec2 self_max{};

        Vec2 other_min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vec2 other_max{};

        auto vec_unit = Vec::NormalizeEdge(_edge);

        // self projection area
        for (auto &vertex : transform->GetVertexs()){
            auto proj_v = Vec::Projection(vec_unit, vertex);
            self_min.x = std::min(proj_v.x, self_min.x);
            self_max.x = std::max(proj_v.x, self_max.x);

            self_min.y = std::min(proj_v.y, self_min.y);
            self_max.y = std::max(proj_v.y, self_max.y);
        }

        // other projection area
        for (auto &vertex : transform_other->GetVertexs()){
            auto proj_v = Vec::Projection(vec_unit, vertex);
            other_min.x = std::min(proj_v.x, other_min.x);
            other_max.x = std::max(proj_v.x, other_max.x);

            other_min.y = std::min(proj_v.y, other_min.y);
            other_max.y = std::max(proj_v.y, other_max.y);
        }

        // Overlap Check
        if (self_max.x < other_min.x || other_max.x < self_min.x || self_max.y < other_min.y || other_max.y < self_min.y){
            return false;
        }
        else{
            // overlap 
            // mtv = minimum translation vector
            mtv.vec = vec_unit;
            Vec2 temp;
            temp.x = self_max.x - other_min.x;
            temp.y = self_max.y - other_min.y;

            float length = Vec::LengthSquare(temp);
            if (mtv.length > length){
                mtv.length = length;
            }
        }
    }

    // other edge
    for (auto &_edge : right_vec_edge){
        Vec2 self_min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vec2 self_max{std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

        Vec2 other_min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vec2 other_max{std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

        auto vec_unit = Vec::NormalizeEdge(_edge);

        // self projection area
        for (auto &vertex : transform->GetVertexs()){
            auto proj_v = Vec::Projection(vec_unit, vertex);
            self_min.x = std::min(proj_v.x, self_min.x);
            self_max.x = std::max(proj_v.x, self_max.x);

            self_min.y = std::min(proj_v.y, self_min.y);
            self_max.y = std::max(proj_v.y, self_max.y);
        }

        // other projection area
        for (auto &vertex : transform_other->GetVertexs()){
            auto proj_v = Vec::Projection(vec_unit, vertex);
            other_min.x = std::min(proj_v.x, other_min.x);
            other_max.x = std::max(proj_v.x, other_max.x);

            other_min.y = std::min(proj_v.y, other_min.y);
            other_max.y = std::max(proj_v.y, other_max.y);
        }

        // Overlap Check
        if (self_max.x <= other_min.x || other_max.x <= self_min.x || self_max.y <= other_min.y || other_max.y <= self_min.y){
            return false;
        }
        else{
            // overlap 
            Vec2 temp;
            temp.x = self_max.x - other_min.x;
            temp.y = self_max.y - other_min.y;
            
            float length = Vec::LengthSquare(temp);
            if (mtv.length > length){
                mtv.length = length;
                mtv.vec = vec_unit;
            }
        }
    }

    transform->AddPos(mtv.vec * sqrt(mtv.length));
    std::cout <<"MTV : " << mtv.vec.x << " " << mtv.vec.y << " " << mtv.length << std::endl;

    return CollisionInfo(true, mtv);
}
