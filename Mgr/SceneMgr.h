#pragma once
#include <memory>
#include "../Scene.h"
#include "../Script/Script.h"

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

        static std::unique_ptr<Scene> &GetCurScene()
        {
            return m_cur_scene;
        }

        // =========================
        // Component Method
        // =========================
        template <typename T>
        static std::shared_ptr<T> GetComponent(const uint32_t &_owner_id)
        {
            return m_cur_scene->GetComponent<T>(_owner_id);
        };
        
        // static std::shared_ptr<Component>& GetComponent(const uint32_t _id){
        //     return m_cur_scene -> GetComponent(_id);
        // }
        
        static const auto& GetComponentsID(const uint32_t& _owner_id){
            return m_cur_scene->GetComponentsID(_owner_id);
        }
        
        static void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept{
            m_cur_scene ->DeleteComponent(std::move(_comp));
        }
        
        // =========================
        // Script Method
        // =========================
        // Get Script by Entity ID + Class Name 
        template<typename T>
        static std::shared_ptr<T> GetScript(const uint32_t &_owner_id){
            return m_cur_scene->GetScript<T>(_owner_id);
        };

        // Get Script by Script ID
        static std::shared_ptr<Script> &GetScript(const uint32_t _script_id){
            return m_cur_scene->GetScript(_script_id);
        }

        static void DeleteScript(std::shared_ptr<Script>&& _script) noexcept{
            m_cur_scene ->DeleteScript(std::move(_script));
        }

        static int GetScriptID(const uint32_t& _owner_id){
            return m_cur_scene->GetScriptID(_owner_id);
        }

        // =========================
        // Entity Method
        // =========================
        template<typename T>
        static const std::vector<uint32_t>& GetEntityVector(){
            return m_cur_scene->GetEntityVector<T>();
        }

        template<typename T>
        static void AddEntity(const Entity& _entity){
            m_cur_scene->AddEntity<T>(_entity);
        }

        // =========================
        // Camera Method
        // =========================
        static void SetMainCamera(uint32_t _id) noexcept{
            m_cur_scene->SetMainCamera(_id);
        }
        static uint32_t GetMainCamera() noexcept{
            return m_cur_scene->GetMainCamera();    // entity id 
        }
};