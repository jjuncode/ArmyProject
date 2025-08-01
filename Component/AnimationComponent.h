#pragma once
#include "Component.h"
#include <unordered_map>
#include "../Object/Transform.h"

struct Bone{
    std::string name;
    
    Transform cur_transform;
    Transform bind_transform;

    Bone() = default;
    Bone(const std::string& _name, Vec3 _pos)
        : name{_name}
        , cur_transform{_pos}
        , bind_transform{_pos}
        {}  
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