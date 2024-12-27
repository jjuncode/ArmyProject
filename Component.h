#pragma once

class Component{
    private:
        uint m_owner_id;

    public:
        virtual void Update();
        virtual void Render();

        void SetOwner(uint _id){m_owner_id = _id;}

        virtual ~Component();
};