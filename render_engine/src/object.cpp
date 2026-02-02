#include "object.h"
#include <algorithm>
#include "object3d.h"
#include "geometry.h"

Box3D::Box3D()
{
	m_strClassName = "Box3D";
}

std::shared_ptr<Box3D> Box3D::New()
{
	return std::make_shared<Box3D>();
}

void Box3D::SetFromAttribute(const Attributef::Ptr& attribute)
{
	for (uint32_t i = 0; i < attribute->GetCount(); i++)
	{
		float x = attribute->GetX(i);
		float y = attribute->GetY(i);
		float z = attribute->GetZ(i);

		if (x < m_min.x)
			m_min.x = x;
		if (y < m_min.y)
			m_min.y = y;
		if (z < m_min.z)
			m_min.z = z;

		if (x > m_max.x)
			m_max.x = x;
		if (y > m_max.y)
			m_max.y = y;
		if (z > m_max.z)
			m_max.z = z;
	}
}

glm::vec3 Box3D::GetCenter() const
{
	if (IsEmpty())
		return glm::vec3(0.0f);

	return (m_max + m_min) / 2.0f;
}

bool Box3D::IsEmpty() const
{
	return (m_max.x < m_min.x || m_max.y < m_min.y || m_max.z < m_min.z);
}

Sphere::Sphere(const glm::vec3& center, float radius)
	:m_center(center), m_radius(radius)
{
	m_strClassName = "Sphere";
}

std::shared_ptr<Sphere> Sphere::New(const glm::vec3& center, float radius)
{
	return std::make_shared<Sphere>(center, radius);
}

void Sphere::ApplyMatrix4(const glm::mat4& matrix)
{
	m_center = glm::vec3(matrix * glm::vec4(m_center, 1.0));

	float scaleX = glm::length(glm::vec3(matrix[0]));
	float scaleY = glm::length(glm::vec3(matrix[1]));
	float scaleZ = glm::length(glm::vec3(matrix[2]));

	float maxScale = std::max(std::max(scaleX, scaleY), scaleZ);
	m_radius *= maxScale;
}

void Sphere::CopyFrom(const std::shared_ptr<Sphere>& other)
{
	m_center = other->m_center;
	m_radius = other->m_radius;
}

void Sphere::SetCenter(const glm::vec3& center)
{
	m_center = center;
}

void Sphere::SetRadius(float radius)
{
	m_radius = radius;
}

glm::vec3 Sphere::GetCenter() const
{
	return m_center;
}

float Sphere::GetRadius() const
{
	return m_radius;
}

Plane::Plane(const glm::vec3& normal, float constant)
{
	m_strClassName = "Plane";
	m_constant = constant;
	m_normal = glm::normalize(normal);
}

std::shared_ptr<Plane> Plane::New(const glm::vec3& normal, float constant)
{
	return std::make_shared<Plane>(normal, constant);
}

float Plane::DistanceToPoint(const glm::vec3& point) const
{
	return glm::dot(m_normal, point) + m_constant;
}

void Plane::SetComponents(float x, float y, float z, float w)
{
	m_normal.x = x;
	m_normal.y = y;
	m_normal.z = z;

	float length = glm::length(m_normal);
	m_normal /= length;
	m_constant = w / length;
}

Frustum::Frustum()
{
	m_sphereTool = Sphere::New(glm::vec3(0.0f), 0.0f);
	for (int i = 0; i < 6; i++)
	{
		m_planes.push_back(Plane::New(glm::vec3(0.0f), 0.0f));
	}
}

std::shared_ptr<Frustum> Frustum::New()
{
	return std::make_shared<Frustum>();
}

void Frustum::SetFromProjectionMatrix(const glm::mat4& matrix)
{
	auto m = glm::value_ptr(matrix);

	m_planes[0]->SetComponents(m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
	m_planes[1]->SetComponents(m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
	m_planes[2]->SetComponents(m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);
	m_planes[3]->SetComponents(m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);
	m_planes[4]->SetComponents(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
	m_planes[5]->SetComponents(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
}

bool Frustum::IntersectObject(const PRenderableObject& object)
{
	PGeometry geometry = object->GetGeometry();

	if (geometry->GetBoundingSphere() == nullptr) 
		geometry->ComputeBoundingSphere();

	m_sphereTool->CopyFrom(geometry->GetBoundingSphere());
	m_sphereTool->ApplyMatrix4(object->GetWorldMatrix());

	return IntersectSphere(m_sphereTool);
}

bool Frustum::IntersectSphere(const PSphere& sphere)
{
	glm::vec3 center = sphere->GetCenter();
	float radius = sphere->GetRadius();

	for (uint32_t i = 0; i < 6; ++i) 
	{
		//计算包围球的球心到当前平面的距离
		float distance = m_planes[i]->DistanceToPoint(center);

		//如果球心在平面的正面，那么distance一定是正数，下方if不进去
		// 如果球心在平面的反面，那么distance一定是负数，假设球与面的距离大于了radius
		// 即-distance > radius 则 distance < -radius 代表着球不予本平面相交且位于平面反面，则本物体需要被剪裁
		if (distance < -radius)
			return false;
	}

	//视景体剪裁通过，可以绘制
	return true;
}
