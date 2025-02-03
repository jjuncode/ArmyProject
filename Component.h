#pragma once
#include <cstdint>

class Component{
    private:
        uint32_t m_owner_id;

    public:
        virtual void Update();
        virtual void Render();

        void SetOwner(uint32_t _id){m_owner_id = _id;}

        virtual ~Component();
};