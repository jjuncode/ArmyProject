#pragma once
#include "../struct.h"
#include "Object.h"

class Camera : public Object{
    private:
        uint32_t m_target;

    public:
        Camera(uint32_t _target) :Object(), m_target(_target){
            AddScene<Camera>();
        }
        void CreateVertex(Vec2 _pos, Vec2 _scale) override; // 형식상 존재

        void SetMainCamera() noexcept{
            SceneMgr::SetMainCamera(obj.GetEntityID());
        }
};