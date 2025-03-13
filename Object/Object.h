#pragma once
#include <vector>
#include "Entity.h"
#include "../struct.h"
#include "../Mgr/SceneMgr.h"
#include "SFML/Graphics.hpp"

class Object{
    protected:
        Entity obj;
        std::vector<Vec2> m_vec_vertex;

    public:
        Object(Vec2 _pos,Vec2 _scale){
            Init(_pos, _scale);
            CreateVertex();
        }
        
        template<typename T>
        void AddScene(){
            SceneMgr::AddEntity<T>(obj);
        }
        
        void SetCollider(CollisionEntityType _type);
        virtual void SetColor(sf::Color _color);

        virtual void SetShape();
        virtual void SetScript() = 0;

        private:
            virtual void Init(Vec2 _pos,Vec2 _scale);
            virtual void CreateVertex();
};