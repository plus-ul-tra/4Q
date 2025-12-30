#pragma once
#include "Reflection.h"
//매크로만 등록 하는 곳

#define REGISTER_COMPONENT(TYPE) \
    static ComponentTypeInfo TYPE##_TypeInfo = { \
        #TYPE, [](){ return std::make_unique<TYPE>(); }, {} \
    }; \
    const char* TYPE::GetTypeName() const { return #TYPE; } \
    static bool TYPE##_Registered = [](){ \
		std::cout << "Registering component: " << #TYPE << std::endl;\
        ComponentRegistry::Instance().Register(&TYPE##_TypeInfo); \
        return true; \
    }();

#define REGISTER_PROPERTY(TYPE, FIELD) \
    static bool TYPE##_##FIELD##_registered = [](){ \
    TYPE##_TypeInfo.properties.emplace_back( \
    std::make_unique<MemberProperty<TYPE, decltype(TYPE::FIELD)>>( \
    #FIELD, &TYPE::FIELD \
	)); \
    return true; \
    }();