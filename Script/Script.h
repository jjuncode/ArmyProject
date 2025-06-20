#pragma once
#include <stack>
#include <stdint.h>
#include <memory>
#include "../struct.h"

class Script {
    private:
    static std::stack<uint32_t> remain_id;

    uint32_t m_id;          // my id
    uint32_t m_owner_id;    // owner id

public:
    void SetOwner(uint32_t _id) { m_owner_id = _id; }

    uint32_t GetOwnerID() { return m_owner_id; }
    uint32_t GetID() { return m_id; }
    
    static bool IsValid(int _id) { return _id != -1; }

    Script();
    virtual ~Script() {remain_id.push(m_id);};

public:
    virtual void Execute(float dt); // script execute

    virtual void ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv, float dt);
    virtual void ExecuteCollStay(uint32_t other_entity_id, MTV _mtv, float dt);
    virtual void ExecuteCollExit(uint32_t other_entity_id, MTV _mtv, float dt);

    // Script Template
    template <typename T, typename... V>
    static std::unique_ptr<T> CreateScript(V &&...params)
    {
        std::unique_ptr<T> script{std::make_unique<T>(std::forward<V>(params)...)};
        return script;
    }
};