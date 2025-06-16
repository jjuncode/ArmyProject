#pragma once
#include "../struct.h"
#include "Object.h"

class Camera : public Object{
    private:
        uint32_t m_target;

    public:
        Camera(uint32_t _target) :Object(), m_target(_target){
        }

        void SetMainCamera() noexcept{
            SceneMgr::SetMainCamera(obj.GetEntityID());
        }
};