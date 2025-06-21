#pragma once
#include <memory>
#include "../Scene.h"
#include "../Script/Script.h"
#include "../Object/Object.h"

class SceneMgr{
    private:
        static std::unique_ptr<Scene> m_cur_scene;
    
    public:
        static void Update(float dt);
        static void Render();
        static void Exit();

        template<typename T>
        static void ChangeScene(){
            auto scene_new = new T();
            if ( m_cur_scene ) 
                m_cur_scene->Exit();
            m_cur_scene.reset(scene_new);
            m_cur_scene->Init();
        }

        static void LoadResource();

        // =========================
        // Collision Method
        // =========================
        static void SetCollisionLayer(CollisionObjectType l_type, CollisionObjectType r_type, bool check){m_cur_scene->SetCollisionLayer(l_type, r_type, check);};
	    static const auto& GetCollisionLayer(){ return m_cur_scene->GetCollisionLayer();}
	    static const auto& GetCollisionObject(uint32_t _type ){ return m_cur_scene->GetCollisionObject(_type);}
	    static void AddCollisionObject(CollisionObjectType _type, uint32_t entity_id){  m_cur_scene->AddCollisionObject(_type, entity_id);}
	    static void DeleteCollisionObject(CollisionObjectType _type, const uint32_t& entity_id) {   m_cur_scene->DeleteCollisionObject(_type, entity_id);};

        // =========================
        // Component Method
        // =========================
        template <typename T>
        static void AddComponent(std::shared_ptr<T>&& _comp){m_cur_scene->AddComponent<T>(std::move(_comp));};
        template <typename T>
        static std::shared_ptr<T> GetComponent(const uint32_t &_owner_id){return m_cur_scene->GetComponent<T>(_owner_id);};
        static void DeleteComponent(uint32_t _comp_id) noexcept{m_cur_scene->DeleteComponent(_comp_id);}
        
        // =========================
        // Object Method
        // =========================
        static Object& GetObject(const uint32_t& _id ) noexcept{return m_cur_scene->GetObject(_id);}
        static void AddObject(std::unique_ptr<Object>&& _obj) noexcept{m_cur_scene->AddObject(std::move(_obj));}    // Muse be placed in the order they were created  
        static void DeleteObject(uint32_t _id) noexcept{m_cur_scene->DeleteObject(_id);}
	    static bool IsActiveObject(uint32_t _obj_id) {return m_cur_scene->IsActiveObject(_obj_id);}
        static void AddLight(std::unique_ptr<Object>&& _obj) noexcept{m_cur_scene->AddLight(std::move(_obj));}
        static const auto& GetLightIDs() noexcept {return m_cur_scene->GetLightIDs();}

        // =========================
        // Camera Method
        // =========================
        static void SetMainCamera(const std::unique_ptr<Object>& _obj) noexcept{m_cur_scene->SetMainCamera(_obj->GetObjectID());}
        static uint32_t GetMainCamera() noexcept{return m_cur_scene->GetMainCamera(); }
};