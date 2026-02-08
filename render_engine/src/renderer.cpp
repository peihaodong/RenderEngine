#include "renderer.h"
#include "camera.h"
#include "object.h"
#include "object3d.h"
#include "material.h"
#include "driver.h"
#include "uniform.h"

SRenderItem::SRenderItem()
{
}

std::shared_ptr<SRenderItem> SRenderItem::New()
{
	return std::make_shared<SRenderItem>();
}

RenderList::RenderList()
{
	m_strClassName = "RenderList";
	m_frustum = Frustum::New();
}

std::shared_ptr<RenderList> RenderList::New()
{
	return std::make_shared<RenderList>();
}

void RenderList::InitRenderList(const PScene& scene, const glm::mat4& matrixViewProjection)
{
	m_nValidRenderItemCount = 0;
	m_vecOpaques.clear();
	m_vecTransparents.clear();

	//根据视图投影矩阵更新视景体对象
	m_frustum->SetFromProjectionMatrix(matrixViewProjection);

	ProjectObject(scene, matrixViewProjection);

	auto nCacheSize = m_vecRenderItemCache.size();
	for (uint32_t i = m_nValidRenderItemCount; i < nCacheSize; i++)
	{
		PSRenderItem& item = m_vecRenderItemCache[i];
		if (!item)
			break;

		item->m_renderable_object = nullptr;
		item->m_id = 0;
		item->m_fZ = 0;
	}

	Sort();
}

const std::vector<PSRenderItem>& RenderList::GetOpaques() const
{
	return m_vecOpaques;
}

const std::vector<PSRenderItem>& RenderList::GetTransparents() const
{
	return m_vecTransparents;
}

void RenderList::ProjectObject(const PObject3D& object, const glm::mat4& matrixViewProjection)
{
	if (!object->IsVisible())
		return;

	glm::vec4 vecTool(1.0f);

	if (std::dynamic_pointer_cast<RenderableObject>(object))
	{///可渲染对象

		//排序
		vecTool = glm::vec4(object->GetWorldPosition(), 1.0);
		vecTool = matrixViewProjection * vecTool;

		PRenderableObject renderable_object = std::dynamic_pointer_cast<RenderableObject>(object);
		//视景体剪裁测试
		if (m_frustum->IntersectObject(renderable_object))
		{
			//添加到渲染列表中
			Push(renderable_object, vecTool.z);
		}
	}

	//循环子节点
	auto children = object->GetChildren();
	for (auto& child : children)
	{
		ProjectObject(child, matrixViewProjection);
	}
}

void RenderList::Push(const PRenderableObject& renderable_object, float z)
{
	const PSRenderItem& item = GetNextRenderItem(renderable_object, z);

	if (item->m_renderable_object->GetMaterial()->IsTransparent())
	{
		m_vecTransparents.push_back(item);
	}
	else
	{
		m_vecOpaques.push_back(item);
	}
}

void RenderList::Sort(const RenderListSortFunction& opaqueSort /*= SmallerZFirstSort*/, const RenderListSortFunction& transparentSort /*= BiggerZFirstSort*/)
{
	if (!m_vecOpaques.empty())
		std::sort(m_vecOpaques.begin(), m_vecOpaques.end(), opaqueSort);

	if (!m_vecTransparents.empty())
		std::sort(m_vecTransparents.begin(), m_vecTransparents.end(), transparentSort);
}

PSRenderItem RenderList::GetNextRenderItem(const PRenderableObject& renderable_object, float z)
{
	PSRenderItem item;

	if (m_nValidRenderItemCount >= m_vecRenderItemCache.size())
	{
		item = SRenderItem::New();
		m_vecRenderItemCache.push_back(item);
	}
	else
	{
		item = m_vecRenderItemCache[m_nValidRenderItemCount];
	}

	item->m_renderable_object = renderable_object;
	item->m_id = renderable_object->GetID();
	item->m_fZ = z;

	m_nValidRenderItemCount++;

	return item;
}

Renderer::Renderer()
{
	m_strClassName = "Renderer";

	m_render_list = RenderList::New();

	m_driver_manager = DriverManager::GetInstance();
}

std::shared_ptr<Renderer> Renderer::New()
{
	return std::make_shared<Renderer>();
}

void Renderer::SetCamera(const PCamera& camera)
{
	m_camera = camera;
	if (m_camera_control)
		m_camera_control->SetCamera(m_camera);
}

void Renderer::SetCameraControl(const PCameraControl& camera_control)
{
	m_camera_control = camera_control;
	if (m_camera)
		m_camera_control->SetCamera(m_camera);
}

void Renderer::SetScene(const PScene& scene)
{
	m_scene = scene;
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	m_clear_color.r = r;
	m_clear_color.g = g;
	m_clear_color.b = b;
	m_clear_color.a = a;
}

void Renderer::initializeGL()
{
	if (!m_driver_state)
	{
		m_driver_state = DriverState::New();
		//m_driver_state->initializeOpenGLFunctions();
	}

	m_driver_state->SetClearColor(m_clear_color);
}

void Renderer::resizeGL(int w, int h)
{
	m_camera->SetWindowWidthHeight(w, h);

	m_viewport.z = w;
	m_viewport.w = h;
}

void Renderer::paintGL()
{
	//更新场景世界变换矩阵
	m_scene->UpdateWorldMatrix(true, true);
	
	//获取相机视图矩阵
	glm::mat4 matrixCameraView = m_camera->GetViewMatrix();
	SetUniformValue("ViewMatrix", ToMatrix4x4(matrixCameraView));
	//获取相机投影矩阵
	glm::mat4 matrixCameraProjection = m_camera->GetProjectionMatrix();
	SetUniformValue("ProjectionMatrix", ToMatrix4x4(matrixCameraProjection));

	//计算当前摄像机视图投影矩阵
	glm::mat4 curCameraViewProjectionMatrix = matrixCameraProjection * matrixCameraView;
	//初始化渲染列表
	m_render_list->InitRenderList(m_scene, curCameraViewProjectionMatrix);

	//渲染
	Render();
}

bool Renderer::mousePressEvent(QMouseEvent* event)
{
	return m_camera_control->mousePressEvent(event);
}

bool Renderer::mouseReleaseEvent(QMouseEvent* event)
{
	return m_camera_control->mouseReleaseEvent(event);
}

bool Renderer::mouseMoveEvent(QMouseEvent* event)
{
	return m_camera_control->mouseMoveEvent(event);
}

bool Renderer::wheelEvent(QWheelEvent* event)
{
	return m_camera_control->wheelEvent(event);
}

bool Renderer::keyPressEvent(QKeyEvent* event)
{
	return m_camera_control->keyPressEvent(event);
}

bool Renderer::keyReleaseEvent(QKeyEvent* event)
{
	return m_camera_control->keyPressEvent(event);
}

void Renderer::Render()
{
	m_driver_state->SetViewport(m_viewport);
	m_driver_state->ClearBackground();

	const std::vector<PSRenderItem>& opaqueObjects = m_render_list->GetOpaques();
	const std::vector<PSRenderItem>& transparentObjects = m_render_list->GetTransparents();

	for (const auto& item : opaqueObjects)
	{
		RenderRenderableObject(item->m_renderable_object);
	}

	for (const auto& item : transparentObjects)
	{
		RenderRenderableObject(item->m_renderable_object);
	}
}

void Renderer::RenderRenderableObject(const PRenderableObject& object)
{
	object->UpdateModelViewMatrix(m_camera->GetViewMatrix());
	object->UpdateNormalMatrix();

	RenderBufferDirect(object);
}

void Renderer::RenderBufferDirect(const PRenderableObject& object)
{
	PMaterial material = object->GetMaterial();
	PGeometry geometry = object->GetGeometry();
	glm::mat4 matrixWorld = object->GetWorldMatrix();
	SetUniformValue("ModelMatrix", ToMatrix4x4(matrixWorld));

	//init program
	//upload texture
	PDriverMaterial driver_material = m_driver_manager->GetDriverMaterial(material);
	//upload vbo
	//upload ebo
	//upload vao
	PDriverGeometry driver_geometry = m_driver_manager->GetDriverGeometry(geometry);

	//bind program
	driver_material->Bind();
	//set uniform
	driver_material->UpdateUniform(m_mapUniform);
	//active texture uint
	driver_material->ActiveTextureUint();
	//bind vao
	driver_geometry->Bind();
	//draw
	m_driver_state->DrawElements(driver_geometry->GetIndexAttributeCount());

	//release vao
	//release program
}

void Renderer::SetUniformValue(const std::string& name, const QMatrix3x3& value)
{
	auto iter = m_mapUniform.find(name);
	if (iter == m_mapUniform.end())
	{
		PUniform uniform = Uniform::New();
		uniform->SetValue(value);
		m_mapUniform[name] = uniform;
	}
	else
	{
		iter->second->SetValue(value);
	}
}

void Renderer::SetUniformValue(const std::string& name, const QMatrix4x4& value)
{
	auto iter = m_mapUniform.find(name);
	if (iter == m_mapUniform.end())
	{
		PUniform uniform = Uniform::New();
		uniform->SetValue(value);
		m_mapUniform[name] = uniform;
	}
	else
	{
		iter->second->SetValue(value);
	}
}
