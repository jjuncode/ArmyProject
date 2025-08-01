#pragma once
#include <cstdint>
#include <stack>
#include <memory>

class Component{
    private:
	    static std::stack<uint32_t> remain_id;

        uint32_t m_id;          // my id
        uint32_t m_owner_id;    // owner id

    public:
        Component();
       
        virtual ~Component() {
            remain_id.push(m_id);
        };

        virtual void Update(float dt);
        virtual void UpdateLate(float dt);
        virtual void Render();

        void SetOwner(uint32_t _id){m_owner_id = _id;}
        
        uint32_t GetOwnerID(){return m_owner_id; }
        uint32_t GetID(){return m_id;}

        // Component Template
        template <typename T, typename... V>
        static std::unique_ptr<T> CreateComponent(V &&...params)
        {
            std::unique_ptr<T> comp{std::make_unique<T>(std::forward<V>(params)...)};
            return comp;
        }
};