#pragma once
#include <base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "constant.h"

class Geometry;
using PGeometry = std::shared_ptr<Geometry>;
class Material;
using PMaterial = std::shared_ptr<Material>;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

enum class EObject3DType
{
	EOT_Object,
	EOT_RenderableObject,
	EOT_Group,
	EOT_Light
};

//3D对象
class DLL_EXPORT Object3D : public Object, public std::enable_shared_from_this<Object3D>
{
public:
	Object3D();
	~Object3D();

	//平移
	void SetPosition(const glm::vec3& position);
	void SetPosition(float x, float y, float z);

	//旋转
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void RotateAroundAxis(const glm::vec3& axis, float angle);
	void SetRotateAroundAxis(const glm::vec3& axis, float angle);

	//缩放
	void SetScale(const glm::vec3& scale);
	void SetScale(float x, float y, float z);

	//添加子节点
	void AddChild(const std::shared_ptr<Object3D>& child);
	//设置朝向
	void LookAt(const glm::vec3& target, const glm::vec3& up);
	//更新本地变换矩阵
	glm::mat4 UpdateLocalMatrix();
	//更新世界变换矩阵
	virtual glm::mat4 UpdateWorldMatrix(bool bUpdateParent = false, bool bUpdateChildren = false);
	//更新模型视图矩阵（传入摄像机视图矩阵）
	glm::mat4 UpdateModelViewMatrix(const glm::mat4& matrixView);
	//更新法线矩阵
	glm::mat3 UpdateNormalMatrix();

	//获取
	ID GetID() const;
	EObject3DType GetType() const;
	glm::vec3 GetLocalPosition() const;
	glm::vec3 GetWorldPosition() const;
	glm::vec3 GetLocalDirection() const;
	glm::vec3 GetWorldDirection() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;
	glm::mat4 GetLocalMatrix() const;
	glm::mat4 GetWorldMatrix() const;
	glm::mat4 GetModelViewMatrix() const;
	glm::mat3 GetNormalMatrix() const;
	const std::vector<std::shared_ptr<Object3D>>& GetChildren() const;
	bool IsVisible() const;

	//设置
	void SetLocalMatrix(const glm::mat4& matrix);
	void SetWorldMatrix(const glm::mat4& matrix);

protected:
	//将矩阵中的平移、旋转、缩放提取出来
	void Decompose();

protected:
	ID m_id = 0;

	std::weak_ptr<Object3D> m_parent;					//父节点
	std::vector<std::shared_ptr<Object3D>> m_children;	//子节点

	bool m_bVisible = false;//是否有效（如果是可渲染物体，是否对其渲染）
	EObject3DType m_type = EObject3DType::EOT_Object;//类型

	glm::vec3 m_position = glm::vec3(0.0f);//位置
	glm::quat m_quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);//旋转（四元数）
	glm::vec3 m_scale = glm::vec3(1.0f);//缩放

	glm::mat4 m_matrixLocal = glm::mat4(1.0f);	//本地变换矩阵
	glm::mat4 m_matrixWorld = glm::mat4(1.0f);	//世界变换矩阵（将模型顶点从模型坐标系转换到世界坐标系）

	glm::mat4 m_matrixModelView = glm::mat4(1.0f);	//模型视图矩阵（将模型顶点从模型坐标系转换到摄像机坐标系）
	glm::mat3 m_matrixNormal = glm::mat3(1.0f);		//法线矩阵（将模型的normal从模型坐标系转换到摄像机坐标系）
};
using PObject3D = std::shared_ptr<Object3D>;

//可渲染对象
class DLL_EXPORT RenderableObject : public Object3D
{
public:
	RenderableObject(const PGeometry& geometry, const PMaterial& material);

	//获取几何对象
	PGeometry GetGeometry() const;
	//获取材质对象
	PMaterial GetMaterial() const;

protected:
	PGeometry m_geometry;	//几何对象
	PMaterial m_material;	//材质对象
};
using PRenderableObject = std::shared_ptr<RenderableObject>;

//网格
class DLL_EXPORT Mesh : public RenderableObject
{
public:
	Mesh(const PGeometry& geometry, const PMaterial& material);

	static std::shared_ptr<Mesh> New(const PGeometry& geometry, const PMaterial& material);
};
using PMesh = std::shared_ptr<Mesh>;

//场景
class DLL_EXPORT Scene : public Object3D
{
public:
	Scene();

	static std::shared_ptr<Scene> New();
};
using PScene = std::shared_ptr<Scene>;

#pragma warning(pop)  // 恢复之前的警告状态