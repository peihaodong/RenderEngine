#pragma once
#include <base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <limits>
#include "attribute.h"
#include <vector>

class RenderableObject;
using PRenderableObject = std::shared_ptr<RenderableObject>;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//盒子
class DLL_EXPORT Box3D : public Object, public std::enable_shared_from_this<Box3D>
{
public:
	Box3D();

	static std::shared_ptr<Box3D> New();

	//传入一个Mesh的PositionAttribute，计算该包围盒
	void SetFromAttribute(const Attributef::Ptr& attribute);

	//获取中心点
	glm::vec3 GetCenter() const;

protected:
	bool IsEmpty() const;

protected:
	glm::vec3 m_min = glm::vec3(std::numeric_limits<float>::infinity());//用float最大值初始化
	glm::vec3 m_max = glm::vec3(-std::numeric_limits<float>::infinity());//用float最大值的负数初始化
};
using PBox3D = std::shared_ptr<Box3D>;

//球体
class DLL_EXPORT Sphere : public Object, public std::enable_shared_from_this<Sphere>
{
public:
	Sphere(const glm::vec3& center, float radius);

	static std::shared_ptr<Sphere> New(const glm::vec3& center, float radius);

	//应用物体的matrix变换
	void ApplyMatrix4(const glm::mat4& matrix);

	//
	void CopyFrom(const std::shared_ptr<Sphere>& other);

	//设置中心点
	void SetCenter(const glm::vec3& center);
	//设置半径
	void SetRadius(float radius);

	//获取中心点
	glm::vec3 GetCenter() const;
	//获取半径
	float GetRadius() const;

protected:
	glm::vec3 m_center = glm::vec3(0.0f);
	float m_radius = 0.0f;
};
using PSphere = std::shared_ptr<Sphere>;

//平面
class DLL_EXPORT Plane : public Object, public std::enable_shared_from_this<Plane>
{
public:
	Plane(const glm::vec3& normal, float constant);

	static std::shared_ptr<Plane> New(const glm::vec3& normal, float constant);

	//点到平面的距离
	float DistanceToPoint(const glm::vec3& point) const;

	//规范化平面的法向量和常量
	void SetComponents(float x, float y, float z, float w);

protected:
	glm::vec3 m_normal = glm::vec3(0.0f);//法向量
	float m_constant = 0.0f;//常量
};
using PPlane = std::shared_ptr<Plane>;

//视景体
class DLL_EXPORT Frustum : public Object, public std::enable_shared_from_this<Frustum>
{
public:
	Frustum();

	static std::shared_ptr<Frustum> New();

	void SetFromProjectionMatrix(const glm::mat4& matrix);

	bool IntersectObject(const PRenderableObject& object);

protected:
	bool IntersectSphere(const PSphere& sphere);

protected:
	PSphere m_sphereTool;
	std::vector<PPlane> m_planes;
};
using PFrustum = std::shared_ptr<Frustum>;

#pragma warning(pop)  // 恢复之前的警告状态