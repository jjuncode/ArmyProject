#pragma once
#include <cstdint>
#include <stack>

class Component{
    private:
        static uint32_t id_cnt;
	    static std::stack<uint32_t> remain_id;

        uint32_t m_id;          // my id
        uint32_t m_owner_id;    // owner id

    public:
        Component(){
            if (!remain_id.size())
            {
                // Is Empty
                m_id = id_cnt++;
            }
            else
            {
                m_id = remain_id.top();
                remain_id.pop();
            }
        }
        virtual ~Component() = default;

        virtual void Update(float dt);
        virtual void Render();

        void SetOwner(uint32_t _id){m_owner_id = _id;}
        
        uint32_t GetOwnerID(){return m_owner_id; }
        uint32_t GetID(){return m_id;}

        void Delete(){
            remain_id.push(m_id);
        }

};