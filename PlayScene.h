#pragma once
#include "Scene.h"

class PlayScene : public Scene{

    public:
        void Init() override;
        void Update(float dt) override;
};