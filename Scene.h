#pragma once

class Object;
typedef std::list<std::unique_ptr<Object>> OBJCONTAINER;

class Scene
{
private:
	static std::list <OBJCONTAINER*> m_mgr_obj;

public:
	Scene() {}
	~Scene() {}

	// =====================
	// Object function
	// =====================
	static void Init();
	static void Update();		// 임시로 오브젝트를 업데이트 돌리기 
	static void Render();		// 임시로 오브젝트를 업데이트 돌리기 
	static void ShowList();		// 리스트 출력 

	// =====================
	// Object Template
	// =====================
	template<typename T>
	static void Attach(std::unique_ptr<T>& _obj)
	{
		auto list_obj = GetList(_obj);

		// 최초 생성 to do 
		if (!list_obj->size()) {
			m_mgr_obj.push_back(list_obj);
			std::cout << "리스트 최초생성 " << std::endl;
		}

		// obj객체 연결 
		list_obj->push_back(std::move(_obj));
		std::cout << "해당 List 객체 개수 : " << list_obj->size() << std::endl;
	}


private:
	template<typename T>
	static OBJCONTAINER* GetList(T& _obj)
	{
		// list 최초생성
		static OBJCONTAINER list{};
		return &list;
	}

	static void DeleteDead();
};