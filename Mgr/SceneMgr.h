#pragma once
#include <memory>
#include "../Scene.h"

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
            m_cur_scene.reset(scene_new);
            m_cur_scene->Init();
        }

        static std::unique_ptr<Scene> &GetCurScene()
        {
            return m_cur_scene;
        }

        template <typename T>
        static std::shared_ptr<T> GetComponent(const uint32_t &_id)
        {
            return m_cur_scene->GetComponentOrigin<T>(_id);
        };

        static void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept{
            m_cur_scene ->DeleteComponent(std::move(_comp));
        }

        template<typename T>
        static const std::vector<uint32_t> GetEntityVector(){
            return m_cur_scene->GetEntityVector<T>();
        }

        template<typename T>
        static void AddEntity(const Entity& _entity){
            m_cur_scene->AddEntity<T>(_entity);
        }
};