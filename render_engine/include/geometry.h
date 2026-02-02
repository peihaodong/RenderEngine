#pragma once
#include <base.h>
#include <unordered_map>
#include "attribute.h"

class Box3D;
using PBox3D = std::shared_ptr<Box3D>;
class Sphere;
using PSphere = std::shared_ptr<Sphere>;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//几何
class DLL_EXPORT Geometry : public Object, public std::enable_shared_from_this<Geometry>
{
public:
	Geometry();
	~Geometry();

	static std::shared_ptr<Geometry> New();

	//设置属性
	void SetAttribute(EAttributeType type, const Attributef::Ptr& attribute);
	//设置索引属性
	void SetIndexAttribute(const Attributei::Ptr& attribute);

	//获取属性
	Attributef::Ptr GetAttribute(EAttributeType type) const;
	//获取索引属性
	Attributei::Ptr GetIndexAttribute() const;
	//获取ID
	ID GetID() const;
	//获取包围盒
	PBox3D GetBoundindBox() const;
	//获取包围球
	PSphere GetBoundingSphere() const;

	//删除属性
	void DeleteAttribute(EAttributeType type);
	//是否存在指定属性
	bool HasAttribute(EAttributeType type) const;
	//计算包围盒
	void ComputeBoundingBox();
	//计算包围球
	void ComputeBoundingSphere();

protected:
	ID m_id = 0;

	std::unordered_map<EAttributeType, Attributef::Ptr> m_mapAttribute;
	Attributei::Ptr m_IndexAttribute;

	PBox3D m_BoundingBox;		//包围盒
	PSphere m_BoundingSphere;	//包围球
};
using PGeometry = std::shared_ptr<Geometry>;

#pragma warning(pop)  // 恢复之前的警告状态