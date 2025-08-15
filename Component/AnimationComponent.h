#pragma once
#include "Component.h"
#include <unordered_map>
#include "../Object/Transform.h"

class Bone{
private:
    std::string m_name;
    std::string m_parent_name{"NULL"};

    Transform m_cur_transform;
    TransformInfo m_bind_transform;

public:
    Bone() = default;
    Bone(const std::string &_name, Vec3 _pos)
        : m_name{_name}, m_cur_transform{_pos}, m_bind_transform{_pos}
    {}

    void SetParent(Bone &_parent)
    {
        m_parent_name = _parent.GetName();
        m_cur_transform.SetParent(&_parent.GetCurTransform());
    }

    bool HasParent() const{
        if (m_parent_name == "NULL") {
            return false; // No parent
        }
        return true; // Has parent
    }

    const std::string &GetName() const { return m_name; }
    const std::string& GetParentName() const { return m_parent_name;}
    Transform &GetCurTransform() { return m_cur_transform; }
    TransformInfo &GetBindTransform() { return m_bind_transform; }
};

struct Weight{
    std::vector<std::string> bones;
    std::vector<float> values;
    float num_connected_bones;

    Weight(float _num, std::vector<std::string>&& _bones, std::vector<float>&& _values)
        : bones{std::move(_bones)}
        , values{std::move(_values)}
        , num_connected_bones{_num}
        {}
};

class AnimationComponent : public Component{
private:
    static std::unordered_map<std::size_t, std::unique_ptr<Bone>> repository_bones;
    static std::unordered_map<std::size_t, std::vector<Weight>> repository_weight;

    std::string m_ani_name;
    std::unordered_map<std::size_t, Bone> m_map_bones;

public:
    AnimationComponent(std::string &&_name)
        : m_ani_name{std::move(_name)}
    {
        Init();
    }

private:
    void Init(); // Create Bone using Weight ( Bone Repository )

public:
    static void CreateBone(const std::string &_name, const Vec3 &_pos);
    static void CreateBone(std::vector<std::pair<std::string, Vec3>> &&_vec);
    Bone &GetBone(const std::string &_name);

    static void CreateWeight(std::string &&_ani_name, std::vector<Weight> &&_weight);
    const std::vector<Weight> &GetWeight();

    void Update(float dt) override;
};