#pragma once
#include <memory>
#include "Scene.h"

class SceneMgr{
    private:
        static std::unique_ptr<Scene> m_cur_scene;
    
    public:
        static void Init();
        static void Update(float dt);
        static void Render();
        static void Exit();

};