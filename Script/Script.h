#pragma once
#include <stack>
#include <stdint.h>
#include "../struct.h"

class Script {
    private:
    static uint32_t id_cnt;
    static std::stack<uint32_t> remain_id;

    uint32_t m_id;          // my id
    uint32_t m_owner_id;    // owner id

public:
    Script(){
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
    virtual ~Script() = default;

    void SetOwner(uint32_t _id){m_owner_id = _id;}

    uint32_t GetOwnerID(){return m_owner_id; }
    uint32_t GetID(){return m_id;}

    void Delete(){remain_id.push(m_id);}

    static bool IsValid(int _id) 
    {
        return _id != -1;
    }

    public:
        virtual void Execute(float dt); // script execute

        virtual void ExecuteCollEnter(uint32_t other_entity_id,MTV _mtv, float dt);
        virtual void ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt);
        virtual void ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt);
};