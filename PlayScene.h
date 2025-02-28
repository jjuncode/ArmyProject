#pragma once
#include "Scene.h"

class PlayScene : public Scene{
    private:
        uint32_t gird_offset{40};

    public:
        void Init() override;
        void Update(float dt) override;
};