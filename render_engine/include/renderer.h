#pragma once
#include <base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "constant.h"

class Object3D;
using PObject3D = std::shared_ptr<Object3D>;
class Camera;
using PCamera = std::shared_ptr<Camera>;
class CameraControl;
using PCameraControl = std::shared_ptr<CameraControl>;
class Scene;
using PScene = std::shared_ptr<Scene>;
class Frustum;
using PFrustum = std::shared_ptr<Frustum>;
class RenderableObject;
using PRenderableObject = std::shared_ptr<RenderableObject>;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//渲染项
struct DLL_EXPORT SRenderItem 
{
public:
	SRenderItem();

	static std::shared_ptr<SRenderItem> New();

	PRenderableObject m_renderable_object;
	ID m_id = 0;	//m_renderable_object的id
	float m_fZ = 0;//用来排序（渲染透明物体的时候，是从远到近进行渲染）
};
using PSRenderItem = std::shared_ptr<SRenderItem>;
using RenderListSortFunction = std::function<bool(const PSRenderItem&, const PSRenderItem&)>;

static bool SmallerZFirstSort(const PSRenderItem& item0, const PSRenderItem& item1)
{
	if (item0->m_fZ != item1->m_fZ)
	{
		//小的z，排在前面
		return item0->m_fZ < item1->m_fZ;
	}
	else
	{
		//id越大，说明创建的越晚，则创建越晚的物体，越先绘制
		return item0->m_id > item1->m_id;
	}
}

static bool BiggerZFirstSort(const PSRenderItem& item0, const PSRenderItem& item1)
{
	if (item0->m_fZ != item1->m_fZ)
	{
		//z越大，排序越靠前
		return item0->m_fZ > item1->m_fZ;
	}
	else 
	{
		//id越大，说明创建的越晚，则创建越晚的物体，越先绘制
		return item0->m_id > item1->m_id;
	}
}

//渲染列表
class DLL_EXPORT RenderList : public Object, public std::enable_shared_from_this<RenderList>
{
public:
	RenderList();

	static std::shared_ptr<RenderList> New();

	void InitRenderList(const PScene& scene, const glm::mat4& matrixViewProjection);

	const std::vector<PSRenderItem>& GetOpaques() const;
	const std::vector<PSRenderItem>& GetTransparents() const;

protected:
	void ProjectObject(const PObject3D& object, const glm::mat4& matrixViewProjection);

	void Push(const PRenderableObject& renderable_object, float z);
	void Sort(const RenderListSortFunction& opaqueSort = SmallerZFirstSort,
		const RenderListSortFunction& transparentSort = BiggerZFirstSort);

	PSRenderItem GetNextRenderItem(const PRenderableObject& renderable_object, float z);

protected:
	PFrustum m_frustum;	//视景体

	uint32_t m_nValidRenderItemCount = 0;//有效渲染项数量
	std::vector<PSRenderItem> m_vecOpaques;//非透明物体集合
	std::vector<PSRenderItem> m_vecTransparents;//透明物体集合
	std::vector<PSRenderItem> m_vecRenderItemCache;//渲染项缓存
};
using PRenderList = std::shared_ptr<RenderList>;

//渲染器
class DLL_EXPORT Renderer : public Object, public std::enable_shared_from_this<Renderer>
{
public:
	Renderer();

	static std::shared_ptr<Renderer> New();

	//设置相机
	void SetCamera(const PCamera& camera);
	//设置相机控制器
	void SetCameraControl(const PCameraControl& camera_control);
	//设置场景
	void SetScene(const PScene& scene);

	//设置背景清除色
	void SetClearColor(float r, float g, float b, float a);

	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	bool mousePressEvent(QMouseEvent* event);
	bool mouseReleaseEvent(QMouseEvent* event);
	bool mouseMoveEvent(QMouseEvent* event);
	bool wheelEvent(QWheelEvent* event);
	bool keyPressEvent(QKeyEvent* event);
	bool keyReleaseEvent(QKeyEvent* event);

protected:
	void Render();

protected:
	PCamera m_camera;
	PCameraControl m_camera_control;
	PScene m_scene;

	glm::vec4 m_clear_color = glm::vec4(1.0, 1.0, 1.0, 1.0);

protected:
	PRenderList m_render_list;	//渲染列表
};
using PRenderer = std::shared_ptr<Renderer>;

#pragma warning(pop)  // 恢复之前的警告状态