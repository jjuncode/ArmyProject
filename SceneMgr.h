#pragma once
#include <memory>
#include "Scene.h"

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

        static std::unique_ptr<Scene>& GetCurScene(){
            return m_cur_scene;
        }

};