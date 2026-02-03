#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include "constant.h"
#include "command.h"
#include <taskmanager.h>

enum EAttributeType
{
	EAT_Position,
	EAT_Color,
	EAT_UV,
	EAT_Normal
};

//属性
template<typename T>
class Attribute : public std::enable_shared_from_this<Attribute<T>>
{
public:
	Attribute(const std::vector<T>& datas, uint32_t nItemSize, EBufferAllocType nBufferAllocType = EBufferAllocType::EBAT_StaticDraw);
	~Attribute();

	using Ptr = std::shared_ptr<Attribute<T>>;
	static Ptr New(const std::vector<T>& datas, uint32_t nItemSize, EBufferAllocType nBufferAllocType = EBufferAllocType::EBAT_StaticDraw)
	{
		return std::make_shared<Attribute<T>>(datas, nItemSize, nBufferAllocType);
	}

	//设置第index个顶点的值
	void SetX(const uint32_t& index, T value);
	void SetY(const uint32_t& index, T value);
	void SetZ(const uint32_t& index, T value);

	//获取第index个顶点的值
	T GetX(const uint32_t& index) const;
	T GetY(const uint32_t& index) const;
	T GetZ(const uint32_t& index) const;

	//获取
	ID GetID() const;
	EDataType GetDataType() const;
	EBufferAllocType GetBufferAllocType() const;
	std::vector<T> GetDatas() const;
	uint32_t GetItemSize() const;
	uint32_t GetCount() const;

protected:
	ID m_id = 0;
	EDataType m_nDataType = EDataType::EDT_Float;//数据类型
	EBufferAllocType m_nBufferAllocType = EBufferAllocType::EBAT_StaticDraw;//
	std::vector<T> m_datas;//数据数组
	uint32_t m_nItemSize = 0;//多少个数据为一个顶点数据
	uint32_t m_nCount = 0;//多少个顶点数据
};

using Attributef = Attribute<float>;
using Attributei = Attribute<uint32_t>;

template<typename T>
Attribute<T>::Attribute(const std::vector<T>& datas, uint32_t nItemSize, EBufferAllocType nBufferAllocType /*= EBufferAllocType::EBAT_StaticDraw*/)
{
	m_id = GenerateID();
	m_datas = datas;
	m_nItemSize = nItemSize;
	m_nBufferAllocType = nBufferAllocType;
	m_nDataType = ToDataType<T>();
	m_nCount = static_cast<uint32_t>(m_datas.size()) / m_nItemSize;
}

template<typename T>
Attribute<T>::~Attribute()
{
	PTaskParameter parameter = TaskParameter::Create(ETT_AttributeDestroy);
	parameter->m_informer = this;
	parameter->m_data = UIntData::New(m_id);
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Notify(parameter);
}

template<typename T>
void Attribute<T>::SetX(const uint32_t& index, T value)
{
	assert(index < m_nCount);
	m_datas[index * m_nItemSize] = value;
}

template<typename T>
void Attribute<T>::SetY(const uint32_t& index, T value)
{
	assert(index < m_nCount);
	m_datas[index * m_nItemSize + 1] = value;
}

template<typename T>
void Attribute<T>::SetZ(const uint32_t& index, T value)
{
	assert(index < m_nCount);
	m_datas[index * m_nItemSize + 2] = value;
}

template<typename T>
T Attribute<T>::GetX(const uint32_t& index) const
{
	assert(index < m_nCount);
	return m_datas[index * m_nItemSize];
}

template<typename T>
T Attribute<T>::GetY(const uint32_t& index) const
{
	assert(index < m_nCount);
	return m_datas[index * m_nItemSize + 1];
}

template<typename T>
T Attribute<T>::GetZ(const uint32_t& index) const
{
	assert(index < m_nCount);
	return m_datas[index * m_nItemSize + 2];
}

template<typename T>
ID Attribute<T>::GetID() const
{
	return m_id;
}

template<typename T>
EDataType Attribute<T>::GetDataType() const
{
	return m_nDataType;
}

template<typename T>
EBufferAllocType Attribute<T>::GetBufferAllocType() const
{
	return m_nBufferAllocType;
}

template<typename T>
std::vector<T> Attribute<T>::GetDatas() const
{
	return m_datas;
}

template<typename T>
uint32_t Attribute<T>::GetItemSize() const
{
	return m_nItemSize;
}

template<typename T>
uint32_t Attribute<T>::GetCount() const
{
	return m_nCount;
}