#pragma once
#include <base.h>
#include <QMatrix4x4>

enum class EUniformDataType
{
	EDT_NULL,
	EDT_Mat3,
	EDT_Mat4,
};

class DLL_EXPORT UniformData : public Data
{
public:
	UniformData();
};
using PUniformData = std::shared_ptr<UniformData>;

class DLL_EXPORT UniformDataMat3 : public UniformData
{
public:
	explicit UniformDataMat3(const QMatrix3x3& value);

	QMatrix3x3 m_value;
};

class DLL_EXPORT UniformDataMat4 : public UniformData
{
public:
	explicit UniformDataMat4(const QMatrix4x4& value);

	QMatrix4x4 m_value;
};

class DLL_EXPORT Uniform : public Object
{
public:
	Uniform();

	static std::shared_ptr<Uniform> New();

	void Update(const std::shared_ptr<Uniform>& other);

	EUniformDataType GetType() const;
	bool IsNeedUpdate() const;

	void SetValue(const QMatrix3x3& value);
	void SetValue(const QMatrix4x4& value);

	bool GetValue(QMatrix3x3& value);
	bool GetValue(QMatrix4x4& value);

protected:
	EUniformDataType m_type = EUniformDataType::EDT_NULL;
	PUniformData m_data;
	bool m_bNeedUpdate = true;
};
using PUniform = std::shared_ptr<Uniform>;