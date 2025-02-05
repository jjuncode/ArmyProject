#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include "Entity.h"

class Scene
{
private:
	std::vector<std::unique_ptr<Component>> m_vec_component;

public:
	Scene() {}
	virtual ~Scene() = default;

	// =====================
	// Object function
	// =====================
	virtual void Init();
	virtual void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	virtual void Render();				// 임시로 오브젝트를 업데이트 돌리기 

	void AddComponent(std::unique_ptr<Component>&& _comp){
		auto idx{_comp->GetID()};
		if (idx+1 <= m_vec_component.size()){	// id - 2 , 요소가 3개면 0 1 2 
			// 만약 넣으려는 요소가 앞쪽이라면
			// 기존것은 삭제되고 새로 대체된다.
			m_vec_component[idx] = std::move(_comp);
		}
		else
			// 아니면 그냥 insert
			m_vec_component.emplace_back(std::move(_comp));
	}

	void DeleteComponent(const std::unique_ptr<Component>& _comp){
		_comp->Delete();
	};

};