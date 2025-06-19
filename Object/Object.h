#pragma once
#include <vector>
#include <stack>
#include <memory>
#include "../struct.h"

#include "../Component/Transform.h"
#include "../Component/Renderer.h"

enum class CollisionObjectType {
    kPlayer = 0,
    kBox,
    
    kEND
};

enum class ObjectStatus{
	// Object 상태
	kActive,	
	kDeActive,
	kDead,
	END
};

class Object{
private:
    static std::stack<uint32_t> remain_id;
    uint32_t m_id;
    
    std::string m_name;
    ObjectStatus m_status;

    std::unique_ptr<Transform> m_transform;
    std::unique_ptr<Renderer> m_renderer;

    std::size_t m_mesh_key;
    std::size_t m_texture_key;

    std::size_t m_script_id;
    std::vector<uint32_t> m_vec_component_id; // component id vector

    

public:
    void SetCollider(CollisionObjectType _type, Vec2 _size);

    const uint32_t GetObjectID() const { return m_id; }
    const auto& GetStatus() const { return m_status; }
    auto& GetTransform() { return *(m_transform.get()); }
    void Render() const;

    static void DeadID(int _id);

    static std::unique_ptr<Object> CreateObject(Vec2 _pos = Vec2(), Vec2 _scale = Vec2()) {
        return std::unique_ptr<Object>(new Object(_pos, _scale));
    }

    // Component Template
    template<typename T, typename... V>
	std::unique_ptr<T> CreateComponent(V&&... params)
	{
		std::unique_ptr<T> comp{std::make_unique<T>(std::forward<V>(params)...)};
		comp->SetOwner(m_id);
       return comp;
    }

	// Script Template 
	template<typename T, typename... V>
	std::unique_ptr<T> CreateScript(V&&... params)
    {
        std::unique_ptr<T> script{std::make_unique<T>(std::forward<V>(params)...)};
        script->SetOwner(m_id);
        return script;
    }

    void SetTexutre(std::string &&_name);
    void SetMesh(std::string &&_name);

    const auto &GetTextureKey() { return m_texture_key; };
    const auto &GetMeshKey() { return m_mesh_key; };

private:
    virtual void Init(Vec2 _pos, Vec2 _scale);
    Object(Vec2 _pos = Vec2(), Vec2 _scale = Vec2());
};