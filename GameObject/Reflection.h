#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "Component.h"
#include "json.hpp"

using namespace std;


class Property {
public:
	explicit Property(const char* name) : m_Name(name){}
	virtual ~Property() = default;

	const string& GetName() const { return m_Name; }

	virtual void DrawEditor(Component* c) const = 0;
	virtual void Serialize(Component* c, nlohmann::json& j) const = 0;
	virtual void DeSerialize(Component* c, nlohmann::json& j) const = 0;

private:
	std::string m_Name;
};


template<typename T, typename Field>
class MemberProperty : public Property {
public : 
	using MemberPtr = Field T::*; 

	MemberProperty(const char* name, MemberPtr member) 
		: Property(name), m_Member(member) { }

	//void DrawEditor(Component* c) const override {
	//	T* obj = static_cast<T*>(c);
	//	Editor::Draw(GetName().c_str(), obj->*m_Member);
	//}

	void Serialize(Component* c, nlohmann::json& j) const override
	{
		T* obj = static_cast<T*>(c);
		j[GetName()] = obj->*m_Member;
	}

	void Deserialize(Component* c, const nlohmann::json& j) const override
	{
		if (!j.contains(GetName())) return;
		T* obj = static_cast<T*>(c);
		obj->*m_Member = j[GetName()].get<Field>();
	}

private: 
	MemberPtr m_Member;
};



using ComponentCreateFunc = function<unique_ptr<Component>()>;

struct ComponentTypeInfo {
	string name;  // 컴포넌트 이름
	ComponentCreateFunc factory;  
	vector<unique_ptr<Property>> properties; // 컴포넌트가 가지고 있는 Property(변수 등)
};

class ComponentRegistry {
public:
	static ComponentRegistry& Instance() {
		static ComponentRegistry instance;
		return instance;
	}

	void Register(ComponentTypeInfo* info) {
		m_Types[info->name] = info;
	}

	ComponentTypeInfo* Find(const string& name) {
		auto it = m_Types.find(name);
		return (it != m_Types.end()) ? it->second : nullptr;
	}

private:
	unordered_map<string, ComponentTypeInfo*> m_Types;
};


//등록 메크로
#define REGISTER_COMPONENT(TYPE) \
    static ComponentTypeInfo TYPE##_TypeInfo = { \
        #TYPE, [](){ return std::make_unique<TYPE>(); }, {} \
    }; \
    const char* TYPE::GetTypeName() const { return #TYPE; } \
    static bool TYPE##_Registered = [](){ \
        ComponentRegistry::Instance().Register(&TYPE##_TypeInfo); \
        return true; \
    }();

#define REGISTER_PROPERTY(TYPE, FIELD) \
    static bool TYPE##_##FIELD##_registered = [](){ \
        TYPE##_TypeInfo.properties.emplace_back( \
            std::make_unique<MemberProperty<TYPE, decltype(TYPE::FIELD)>>( \
                #FIELD, &TYPE::FIELD \
            ) \
        ); \
        return true; \
    }()

//#define REGISTER_PROPERTY(TYPE, FIELD) \
//	TYPE##_TypeInfo.properties.emplace_back( \
//	std::make_unique<MemberProperty<TYPE, decltype(TYPE::FIELD)>>( \
//	#FIELD, &TYPE::FIELD \
//	));