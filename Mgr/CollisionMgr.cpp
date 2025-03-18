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

bool CollisionMgr::CollisionLogic(ColliderComponent *left, ColliderComponent *right)
{
    // SAT Collision
    return SATCollision_Logic(left, right);
}

bool CollisionMgr::SATCollision_Logic(ColliderComponent* left, ColliderComponent* right)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(left->GetOwnerID());
    auto transform_other = SceneMgr::GetComponentOrigin<TransformComponent>(right->GetOwnerID());

    auto left_vec_edge = left->GetEdge();
    auto right_vec_edge = right->GetEdge();

    for (auto& edge : left_vec_edge){
        edge.start += transform->GetPos();
        edge.end += transform->GetPos();
    }

    for (auto& edge : right_vec_edge){
        edge.start += transform_other->GetPos();
        edge.end += transform_other->GetPos();
    }

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
            Vec2 temp1;
            temp1.x = self_max.x - other_min.x;
            temp1.y = self_max.y - other_min.y;

            Vec2 temp2;
            temp2.x = self_min.x - other_max.x;
            temp2.y = self_min.y - other_max.y;

            Vec2 temp = (Vec::LengthSquare(temp1) < Vec::LengthSquare(temp2)) ? temp1 : temp2;

            float length = Vec::LengthSquare(temp);
            if (mtv.length > length){
                mtv.length = length;
                mtv.vec = vec_unit;
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
            Vec2 temp1;
            temp1.x = self_max.x - other_min.x;
            temp1.y = self_max.y - other_min.y;

            Vec2 temp2;
            temp2.x = self_min.x - other_max.x;
            temp2.y = self_min.y - other_max.y;

            Vec2 temp = (Vec::LengthSquare(temp1) < Vec::LengthSquare(temp2)) ? temp1 : temp2;

            float length = Vec::LengthSquare(temp);
            if (mtv.length > length){
                mtv.length = length;
                mtv.vec = vec_unit;
            }
        }
    }

    auto left_pos = transform->GetPos();
    auto right_pos = transform_other->GetPos();

    Vec2 vec = left_pos - right_pos;
    auto dot_result = Vec::Dot(vec, mtv.vec);
    if ( dot_result < 0 ){
        mtv.vec = Vec::Reverse(mtv.vec);
    }

    transform->AddPos(mtv.vec * sqrt(mtv.length));
    return true;
}
