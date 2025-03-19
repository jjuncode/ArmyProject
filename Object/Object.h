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
        
        template<typename T>
        void AddScene(){
            SceneMgr::AddEntity<T>(obj);
        }
        
        void SetCollider(CollisionEntityType _type);

        // ======================
        // Abstract Method 
        // ======================
        virtual void SetScript() = 0;
        virtual void CreateVertex(Vec2 _pos,Vec2 _scale) = 0;

    private:
        virtual void Init(Vec2 _pos,Vec2 _scale);
};