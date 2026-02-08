#include "uniform.h"

UniformData::UniformData()
{
	m_strClassName = "UniformData";
}

UniformDataMat3::UniformDataMat3(const QMatrix3x3& value)
	:m_value(value)
{

}

UniformDataMat4::UniformDataMat4(const QMatrix4x4& value)
	:m_value(value)
{

}

Uniform::Uniform()
{
	m_strClassName = "Uniform";
}

std::shared_ptr<Uniform> Uniform::New()
{
	return std::make_shared<Uniform>();
}

void Uniform::Update(const std::shared_ptr<Uniform>& other)
{
	if (m_type != other->GetType())
		m_type = other->GetType();

	if (m_type == EUniformDataType::EDT_Mat3)
	{
		QMatrix3x3 value;
		if (other->GetValue(value))
			SetValue(value);
	}
	else if (m_type == EUniformDataType::EDT_Mat4)
	{
		QMatrix4x4 value;
		if (other->GetValue(value))
			SetValue(value);
	}
}

EUniformDataType Uniform::GetType() const
{
	return m_type;
}

bool Uniform::IsNeedUpdate() const
{
	return m_bNeedUpdate;
}

void Uniform::SetValue(const QMatrix3x3& value)
{
	if (m_type == EUniformDataType::EDT_Mat3)
	{
		std::shared_ptr<UniformDataMat3> data = std::dynamic_pointer_cast<UniformDataMat3>(m_data);
		if (data)
		{
			if (data->m_value != value)
			{
				data->m_value = value;
				m_bNeedUpdate = true;
			}
		}
		else
		{
			m_data = std::make_shared<UniformDataMat3>(value);
			m_bNeedUpdate = true;
		}
	}
	else
	{
		m_type = EUniformDataType::EDT_Mat3;
		m_data = std::make_shared<UniformDataMat3>(value);
		m_bNeedUpdate = true;
	}
}

void Uniform::SetValue(const QMatrix4x4& value)
{
	if (m_type == EUniformDataType::EDT_Mat4)
	{
		std::shared_ptr<UniformDataMat4> data = std::dynamic_pointer_cast<UniformDataMat4>(m_data);
		if (data)
		{
			if (data->m_value != value)
			{
				data->m_value = value;
				m_bNeedUpdate = true;
			}
		}
		else
		{
			m_data = std::make_shared<UniformDataMat4>(value);
			m_bNeedUpdate = true;
		}
	}
	else
	{
		m_type = EUniformDataType::EDT_Mat4;
		m_data = std::make_shared<UniformDataMat4>(value);
		m_bNeedUpdate = true;
	}
}

bool Uniform::GetValue(QMatrix3x3& value)
{
	if (m_type != EUniformDataType::EDT_Mat3)
		return false;
	std::shared_ptr<UniformDataMat3> data = std::dynamic_pointer_cast<UniformDataMat3>(m_data);
	if (!data)
		return false;
	if (!m_bNeedUpdate)
		return false;
	value = data->m_value;
	m_bNeedUpdate = false;
	return true;
}

bool Uniform::GetValue(QMatrix4x4& value)
{
	if (m_type != EUniformDataType::EDT_Mat4)
		return false;
	std::shared_ptr<UniformDataMat4> data = std::dynamic_pointer_cast<UniformDataMat4>(m_data);
	if (!data)
		return false;
	if (!m_bNeedUpdate)
		return false;
	value = data->m_value;
	m_bNeedUpdate = false;
	return true;
}
