#pragma once
#include <vector>
#include "Entity.h"
#include "../struct.h"
#include "../Mgr/SceneMgr.h"
#include "SFML/Graphics.hpp"
#include "../Component/Texture.h"
#include "../Component/Mesh.h"

class Object{
    protected:
        Entity obj;
        std::size_t m_mesh_key;
        std::size_t m_texture_key;

    public:
        Object(Vec2 _pos,Vec2 _scale){
            Init(_pos, _scale);
        }

        Object(){
            Init(Vec2(0,0), Vec2(0,0));
        }
        
        void SetCollider(CollisionEntityType _type, Vec2 _size);

        uint32_t GetEntityID() const noexcept{
            return obj.GetEntityID();
        }

        // Component Template
        template <typename T, typename... V>
        void AddComponent(V &&...params)
        {
            auto &cur_scene = SceneMgr::GetCurScene();
            auto comp = obj.AddComponent<T>(std::forward<V>(params)...);
            cur_scene->AddComponent<T>(std::move(comp));
        }

        // Script Template
        template <typename T, typename... V>
        void SetScript(V &&...params)
        {
            auto &cur_scene = SceneMgr::GetCurScene();
            auto script = obj.SetScript<T>(std::forward<V>(params)...);
            cur_scene->AddScript<T>(std::move(script));
        }

        void SetTexutre(std::string&& _name);
        void SetMesh(std::string&& _name);

        const auto& GetTexture(){ return Texture::GetTexture(m_texture_key);};
        // const Mesh& GetMesh();

    private:
        virtual void Init(Vec2 _pos,Vec2 _scale);
};