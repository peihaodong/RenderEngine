#include "geometry.h"
#include "command.h"
#include <taskmanager.h>
#include "object.h"

Geometry::Geometry()
{
	m_id = GenerateID();
}

Geometry::~Geometry()
{
	PTaskParameter parameter = TaskParameter::Create(ETT_AttributeDestroy);
	parameter->m_informer = this;
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Notify(parameter);
}

std::shared_ptr<Geometry> Geometry::New()
{
	return std::make_shared<Geometry>();
}

void Geometry::SetAttribute(EAttributeType type, const Attributef::Ptr& attribute)
{
	m_mapAttribute[type] = attribute;
}

void Geometry::SetIndexAttribute(const Attributei::Ptr& attribute)
{
	m_IndexAttribute = attribute;
}

Attributef::Ptr Geometry::GetAttribute(EAttributeType type) const
{
	auto iter = m_mapAttribute.find(type);
	if (iter != m_mapAttribute.end())
		return iter->second;

	return nullptr;
}

Attributei::Ptr Geometry::GetIndexAttribute() const
{
	return m_IndexAttribute;
}

ID Geometry::GetID() const
{
	return m_id;
}

PBox3D Geometry::GetBoundindBox() const
{
	return m_BoundingBox;
}

PSphere Geometry::GetBoundingSphere() const
{
	return m_BoundingSphere;
}

void Geometry::DeleteAttribute(EAttributeType type)
{
	auto iter = m_mapAttribute.find(type);
	if (iter != m_mapAttribute.end())
		m_mapAttribute.erase(iter);
}

bool Geometry::HasAttribute(EAttributeType type) const
{
	auto iter = m_mapAttribute.find(type);
	if (iter == m_mapAttribute.end())
		return false;
	return true;
}

void Geometry::ComputeBoundingBox()
{
	Attributef::Ptr position = GetAttribute(EAttributeType::EAT_Position);
	if (!position)
		return;

	if (!m_BoundingBox)
		m_BoundingBox = Box3D::New();

	m_BoundingBox->SetFromAttribute(position);
}

void Geometry::ComputeBoundingSphere()
{
	ComputeBoundingBox();

	if (!m_BoundingSphere)
		m_BoundingSphere = Sphere::New(glm::vec3(0.0f), 0.0f);

	//包围球跟包围盒共享一个center
	m_BoundingSphere->SetCenter(m_BoundingBox->GetCenter());

	Attributef::Ptr position = GetAttribute(EAttributeType::EAT_Position);
	if (!position)
		return;

	//找到距离当前球心最大距离的点
	float maxRadiusSq = 0;
	for (uint32_t i = 0; i < position->GetCount(); i++)
	{
		//把每个顶点的xyz装成一个point
		glm::vec3 point = glm::vec3(position->GetX(i), position->GetY(i), position->GetZ(i));

		//计算point到center的距离
		glm::vec3 radiusVector = m_BoundingSphere->GetCenter() - point;

		//原本应该对比每一个点到center的距离，找到最大。但是计算向量长度，必须经过开方这个运算
		//为了性能考虑，直接记录其平方，最后得到最大值，再开二次方
		maxRadiusSq = std::max(glm::dot(radiusVector, radiusVector), maxRadiusSq);
	}

	//开方求取radius
	m_BoundingSphere->SetRadius(std::sqrt(maxRadiusSq));
}
