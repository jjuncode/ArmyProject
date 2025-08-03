#include "AnimationComponent.h"
#include "../Mgr/SceneMgr.h"

std::unordered_map<std::size_t, std::unique_ptr<Bone>> AnimationComponent::repository_bones;
std::unordered_map<std::size_t, std::vector<Weight>> AnimationComponent::repository_weight;

void AnimationComponent::Init()
{
    std::size_t key = std::hash<std::string>()(m_ani_name);

    // Initalizing Bones
    if ( repository_weight.find(key) != repository_weight.end()) {
        // Get Bones
        auto& weights = repository_weight[key];
        for (const auto& weight : weights) {
            for ( const auto& bone : weight.bones) {
                std::size_t bone_key = std::hash<std::string>()(bone);
                if ( repository_bones.find(bone_key) != repository_bones.end()) {
                    m_map_bones[bone_key] = *repository_bones[bone_key].get();
                } else {
                    std::cerr << "Bone not found" << std::endl;
                }
            }
        }
    }
}

void AnimationComponent::CreateBone(const std::string&_name, const Vec3 &_pos)
{
    std::size_t key = std::hash<std::string>()(_name);

    if ( repository_bones.find(key) != repository_bones.end()) {
        std::cerr << "Bone already exists: " << _name << std::endl;
        return; 
    }
    else{
        auto bone = std::unique_ptr<Bone>(new Bone(_name, _pos));
        std::cout<< "Creating Bone: " << bone->name << std::endl;
        repository_bones[key] = std::move(bone);
    }
}

void AnimationComponent::CreateBone(std::vector<std::pair<std::string, Vec3>> &&_vec)
{
    for(auto& v : _vec){
        CreateBone(v.first,v.second);
    }

}

Bone &AnimationComponent::GetBone(const std::string &_name)
{
    std::size_t key = std::hash<std::string>()(_name);
    
    if ( m_map_bones.find(key) != m_map_bones.end()) {
        return m_map_bones[key];
    }

    assert(false && "Bone not found");
}

void AnimationComponent::CreateWeight(std::string &&_ani_name, std::vector<Weight> &&_weight)
{
    std::size_t key = std::hash<std::string>()(_ani_name);

    if ( repository_weight.find(key) != repository_weight.end()) {
        std::cerr << "Ani Weight already exists: " << _ani_name << std::endl;
        return; 
    }
    else{
        repository_weight[key] = std::move(_weight);
        std::cout<<" Creating Animation Information about : "<< _ani_name<<std::endl;
    }
}

const std::vector<Weight> &AnimationComponent::GetWeight()
{
   std::size_t key = std::hash<std::string>()(m_ani_name);
    
    if ( repository_weight.find(key) != repository_weight.end()) {
        return repository_weight[key];
    }

    assert(false && "Bone not found");
}

void AnimationComponent::Update(float dt)
{

}

void NodeTransform::SetLocalPosition(const Vec3 &_pos)
{
    m_local_transform.SetPos(_pos);
    UpdateWorldTransformFromLocal();
    UpdateChildrenWorldTransform();
}

void NodeTransform::SetLocalScale(const Vec3 &_scale)
{
    m_local_transform.SetScale(_scale);
    UpdateWorldTransformFromLocal();
    UpdateChildrenWorldTransform();
}

Transform NodeTransform::UpdateWorldTransformFromLocal()
{
    Transform result;
    Transform world_parent = m_parent->GetWorldTransform();
    
    result.SetScale(world_parent.GetScale() * m_local_transform.GetScale());
    result.SetRotate(m_local_transform.GetRotate() * world_parent.GetRotate() );
    result.SetPos(world_parent.GetRotate() * ( m_local_transform.GetPos() * world_parent.GetScale())
                 + world_parent.GetPos());

    return result;
}

Transform NodeTransform::UpdateLocalTransformFromWorld()
{
    Transform world_parent_inv = m_parent->GetWorldTransform().GetInverse();
    Transform result;

    result.SetScale(world_parent_inv.GetScale() * m_world_transform.GetScale());
    result.SetRotate(world_parent_inv.GetRotate() * m_world_transform.GetRotate());
    result.SetPos(world_parent_inv.GetRotate() * (m_world_transform.GetPos() * world_parent_inv.GetScale())
                    + world_parent_inv.GetPos());
    
    return result;
}

void NodeTransform::UpdateChildrenWorldTransform()
{
    for (auto& child : m_children) {
        child->SetWorldTransform(child->UpdateWorldTransformFromLocal());
        child->UpdateChildrenWorldTransform();
    }
}