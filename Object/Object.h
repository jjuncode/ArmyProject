#pragma once
#include <vector>
#include "Entity.h"
#include "../struct.h"
#include "../Mgr/SceneMgr.h"
#include "SFML/Graphics.hpp"

class Object{
    protected:
        Entity obj;

    public:
        Object(Vec2 _pos,Vec2 _scale){
            Init(_pos, _scale);
        }

        Object(){
            Init(Vec2(0,0), Vec2(0,0));
        }
        
        template<typename T>
        void AddScene(){
            SceneMgr::AddEntity<T>(obj);
        }
        
        void SetCollider(CollisionEntityType _type, Vec2 _size);

        uint32_t GetEntityID() const noexcept{
            return obj.GetEntityID();
        }

        // ======================
        // Abstract Method 
        // ======================
        virtual void SetScript() = 0;
        virtual void CreateVertex(Vec2 _pos,Vec2 _scale) = 0;

    private:
        virtual void Init(Vec2 _pos,Vec2 _scale);
};