#pragma once
class Object;

class Component{
    public:
        Object* m_owner;

        virtual void Update();
        virtual void Render();

        void SetOwner(Object* obj){m_owner = obj;};
        void Reset();   // reset self 

        virtual ~Component();


};