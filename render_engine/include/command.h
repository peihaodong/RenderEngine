#pragma once
#include <base.h>

enum ETaskType
{
	ETT_Object3DDestroy,
	ETT_AttributeDestroy,
	ETT_GeometryDestroy,
	ETT_MaterialDestroy,
};

class DLL_EXPORT UIntData : public Data
{
public:
	explicit UIntData(uint32_t value);

	static std::shared_ptr<UIntData> New(uint32_t value);

	uint32_t m_value = 0;
};