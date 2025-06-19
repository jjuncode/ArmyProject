#pragma once
#include <cstdint>

class Renderer
{
    private:
        uint32_t m_id_owner;

    public:
        Renderer(){}
        void SetOwner(uint32_t _id) { m_id_owner = _id; }
        
        void Render();
};
