#pragma once
#include <base.h>

enum ETaskType
{
	ETT_AttributeDestroy,	//vbo ebo
	ETT_GeometryDestroy,	//vao
	ETT_MaterialDestroy,	//program
	ETT_TextureDestroy		//texture
};

class DLL_EXPORT UIntData : public Data
{
public:
	explicit UIntData(uint32_t value);

	static std::shared_ptr<UIntData> New(uint32_t value);

	uint32_t m_value = 0;
};