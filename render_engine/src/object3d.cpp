#include "object3d.h"
#include "constant.h"
#include "geometry.h"
#include "material.h"

Object3D::Object3D()
{
	m_strClassName = "Object3D";
	m_id = GenerateID();
}

Object3D::~Object3D()
{

}

void Object3D::SetPosition(const glm::vec3& position)
{
	SetPosition(position.x, position.y, position.z);
}

void Object3D::SetPosition(float x, float y, float z)
{
	m_matrixLocal[3].x = x;
	m_matrixLocal[3].y = y;
	m_matrixLocal[3].z = z;

	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Object3D::RotateX(float angle)
{
	glm::vec3 rotateAxis = glm::vec3(m_matrixLocal[0]);

	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	m_matrixLocal = rotateMatrix * m_matrixLocal;

	Decompose();
}

void Object3D::RotateY(float angle)
{
	glm::vec3 rotateAxis = glm::vec3(m_matrixLocal[1]);

	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	m_matrixLocal = rotateMatrix * m_matrixLocal;

	Decompose();
}

void Object3D::RotateZ(float angle)
{
	glm::vec3 rotateAxis = glm::vec3(m_matrixLocal[2]);

	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	m_matrixLocal = rotateMatrix * m_matrixLocal;

	Decompose();
}

void Object3D::RotateAroundAxis(const glm::vec3& axis, float angle)
{
	m_matrixLocal = glm::rotate(m_matrixLocal, glm::radians(angle), axis);

	Decompose();
}

void Object3D::SetRotateAroundAxis(const glm::vec3& axis, float angle)
{
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), axis);

	float scaleX = glm::length(glm::vec3(m_matrixLocal[0]));
	float scaleY = glm::length(glm::vec3(m_matrixLocal[1]));
	float scaleZ = glm::length(glm::vec3(m_matrixLocal[2]));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, scaleZ));

	//列相等
	m_matrixLocal[0] = rotateMatrix[0];
	m_matrixLocal[1] = rotateMatrix[1];
	m_matrixLocal[2] = rotateMatrix[2];

	m_matrixLocal *= scaleMatrix;

	Decompose();
}

void Object3D::SetScale(const glm::vec3& scale)
{
	SetScale(scale.x, scale.y, scale.z);
}

void Object3D::SetScale(float x, float y, float z)
{
	auto col0 = glm::normalize(glm::vec3(m_matrixLocal[0])) * x;
	auto col1 = glm::normalize(glm::vec3(m_matrixLocal[1])) * y;
	auto col2 = glm::normalize(glm::vec3(m_matrixLocal[2])) * z;

	m_matrixLocal[0] = glm::vec4(col0, 0.0f);
	m_matrixLocal[1] = glm::vec4(col1, 0.0f);
	m_matrixLocal[2] = glm::vec4(col2, 0.0f);

	Decompose();
}

void Object3D::AddChild(const std::shared_ptr<Object3D>& child)
{
	if (child == shared_from_this())
		return;

	child->m_parent = shared_from_this();

	auto iter = std::find(m_children.begin(), m_children.end(), child);
	if (iter != m_children.end())
		return;

	m_children.push_back(child);
}

void Object3D::LookAt(const glm::vec3& target, const glm::vec3& up)
{
	float scaleX = glm::length(glm::vec3(m_matrixLocal[0]));
	float scaleY = glm::length(glm::vec3(m_matrixLocal[1]));
	float scaleZ = glm::length(glm::vec3(m_matrixLocal[2]));

	glm::vec3 position = glm::vec3(m_matrixLocal[3]);

	//make local coordinate
	auto nTarget = glm::normalize(target - position) * scaleZ;
	auto nRight = glm::normalize(glm::cross(up, -nTarget)) * scaleX;
	auto nUp = glm::normalize(glm::cross(nRight, nTarget)) * scaleY;


	m_matrixLocal[0] = glm::vec4(nRight, 0.0f);
	m_matrixLocal[1] = glm::vec4(nUp, 0.0f);
	m_matrixLocal[2] = glm::vec4(-nTarget, 0.0f);
	m_matrixLocal[3] = glm::vec4(position, 1.0f);

	Decompose();
}

glm::mat4 Object3D::UpdateLocalMatrix()
{
	auto translateMatrix = glm::translate(glm::mat4(1.0f), m_position);
	auto rotateMatrix = glm::mat4_cast(m_quaternion);
	auto scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

	m_matrixLocal = translateMatrix * rotateMatrix * scaleMatrix;

	return m_matrixLocal;
}

glm::mat4 Object3D::UpdateWorldMatrix(bool bUpdateParent /*= false*/, bool bUpdateChildren /*= false*/)
{
	if (bUpdateParent && !m_parent.expired())
	{
		auto parent = m_parent.lock();
		parent->UpdateWorldMatrix(true, false);
	}

	UpdateLocalMatrix();

	//初始化worldMatrix，如果没有父节点，那么二者相等
	m_matrixWorld = m_matrixLocal;

	//如果有父节点，那么需要做成父节点的worldMatrix，从而把上方所有节点的影响带入
	if (!m_parent.expired())
	{
		auto parent = m_parent.lock();
		m_matrixWorld = parent->m_matrixWorld * m_matrixWorld;
	}

	//依次更新子节点的worldMatrix
	if (bUpdateChildren)
	{
		for (auto& child : m_children) 
		{
			child->UpdateWorldMatrix(false, true);
		}
	}

	return m_matrixWorld;
}

glm::mat4 Object3D::UpdateModelViewMatrix(const glm::mat4& matrixView)
{
	m_matrixModelView = matrixView * m_matrixWorld;

	return m_matrixModelView;
}

glm::mat3 Object3D::UpdateNormalMatrix()
{
	m_matrixNormal = glm::transpose(glm::inverse(glm::mat3(m_matrixModelView)));

	return m_matrixNormal;
}

ID Object3D::GetID() const
{
	return m_id;
}

EObject3DType Object3D::GetType() const
{
	return m_type;
}

glm::vec3 Object3D::GetLocalPosition() const
{
	return glm::vec3(m_matrixLocal[3]);
}

glm::vec3 Object3D::GetWorldPosition() const
{
	return glm::vec3(m_matrixWorld[3]);
}

glm::vec3 Object3D::GetLocalDirection() const
{
	return glm::normalize(-glm::vec3(m_matrixLocal[2]));
}

glm::vec3 Object3D::GetWorldDirection() const
{
	return glm::normalize(-glm::vec3(m_matrixWorld[2]));
}

glm::vec3 Object3D::GetUp() const
{
	return glm::normalize(glm::vec3(m_matrixLocal[1]));
}

glm::vec3 Object3D::GetRight() const
{
	return glm::normalize(glm::vec3(m_matrixLocal[0]));
}

glm::mat4 Object3D::GetLocalMatrix() const
{
	return m_matrixLocal;
}

glm::mat4 Object3D::GetWorldMatrix() const
{
	return m_matrixWorld;
}

glm::mat4 Object3D::GetModelViewMatrix() const
{
	return m_matrixModelView;
}

glm::mat3 Object3D::GetNormalMatrix() const
{
	return m_matrixNormal;
}

const std::vector<std::shared_ptr<Object3D>>& Object3D::GetChildren() const
{
	return m_children;
}

bool Object3D::IsVisible() const
{
	return m_bVisible;
}

void Object3D::SetLocalMatrix(const glm::mat4& matrix)
{
	m_matrixLocal = matrix;

	Decompose();
}

void Object3D::SetWorldMatrix(const glm::mat4& matrix)
{
	m_matrixWorld = matrix;
}

void Object3D::Decompose()
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_matrixLocal, m_scale, m_quaternion, m_position, skew, perspective);
}

Scene::Scene()
{
	m_strClassName = "Scene";
}

std::shared_ptr<Scene> Scene::New()
{
	return std::make_shared<Scene>();
}

RenderableObject::RenderableObject(const PGeometry& geometry, const PMaterial& material)
	:m_geometry(geometry), m_material(material)
{
	m_strClassName = "RenderableObject";
}

PGeometry RenderableObject::GetGeometry() const
{
	return m_geometry;
}

PMaterial RenderableObject::GetMaterial() const
{
	return m_material;
}

Mesh::Mesh(const PGeometry& geometry, const PMaterial& material)
	:RenderableObject(geometry, material)
{
	m_strClassName = "Mesh";
}

std::shared_ptr<Mesh> Mesh::New(const PGeometry& geometry, const PMaterial& material)
{
	return std::make_shared<Mesh>(geometry, material);
}
