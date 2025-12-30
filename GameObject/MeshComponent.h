#pragma once
#include "Component.h"
#include "ResourceHandle.h"


class MeshComponent : public Component
{
	friend class Editor;
	friend class Property;
	template<typename, typename> friend class MemberProperty;
public:
	static constexpr const char* StaticTypeName = "MeshComponent";
	const char* GetTypeName() const override; // 매크로에서 정의 및 사용

	MeshComponent() = default;
	virtual ~MeshComponent() = default;

	void SetMeshHandle(MeshHandle handle) { m_MeshHandle = handle; }
	MeshHandle GetMeshHandle() const      { return m_MeshHandle;   }

	void Update(float deltaTime) override;
	void OnEvent(EventType type, const void* data) override;

	void Serialize(nlohmann::json& j) const override;
	void Deserialize(const nlohmann::json& j) override;
protected:
	MeshHandle m_MeshHandle;
};

