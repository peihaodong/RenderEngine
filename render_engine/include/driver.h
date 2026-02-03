#pragma once
#include <base.h>
#include <QOpenGLFunctions_4_3_Core>
#include <taskcommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>
#include "constant.h"
#include <QOpenGLShaderProgram>
#include "attribute.h"

class Material;
using PMaterial = std::shared_ptr<Material>;
class Texture;
using PTexture = std::shared_ptr<Texture>;
class Geometry;
using PGeometry = std::shared_ptr<Geometry>;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//驱动
class DLL_EXPORT Driver : public Object, public QOpenGLFunctions_4_3_Core, public std::enable_shared_from_this<Driver>
{
public:
	Driver();
};
using PDriver = std::shared_ptr<Driver>;

//驱动-状态
class DLL_EXPORT DriverState : public Driver
{
public:
	DriverState();

	static std::shared_ptr<DriverState> New();

	//设置视口
	void SetViewport(const glm::vec4& viewport);

	//
	void DrawElements(uint32_t count);

protected:
	glm::vec4 m_viewport;//当前视口
};
using PDriverState = std::shared_ptr<DriverState>;

//驱动-材质
class DLL_EXPORT DriverMaterial : public Driver
{
public:
	explicit DriverMaterial(const PMaterial& material);
	~DriverMaterial();

	void Bind();
	void UpdateUniform();
	void ActiveTextureUint();

protected:
	void Init();

protected:
	std::weak_ptr<Material> m_material;
	std::shared_ptr<QOpenGLShaderProgram> m_program;
};
using PDriverMaterial = std::shared_ptr<DriverMaterial>;

//驱动-纹理
class DLL_EXPORT DriverTexture : public Driver
{
public:
	DriverTexture(const PTexture& texture);
	~DriverTexture();

protected:
	void Init();

protected:
	std::weak_ptr<Texture> m_texture;
	GLuint m_nTexture = 0;
};
using PDriverTexture = std::shared_ptr<DriverTexture>;

//驱动-几何
class DLL_EXPORT DriverGeometry : public Driver
{
public:
	DriverGeometry(const PGeometry& geometry);
	~DriverGeometry();

	void Bind();

	uint32_t GetIndexAttributeCount() const;

protected:
	void Init();

protected:
	std::weak_ptr<Geometry> m_geometry;
	GLuint m_vao = 0;
};
using PDriverGeometry = std::shared_ptr<DriverGeometry>;

//驱动-属性
class DLL_EXPORT DriverAttribute : public Driver
{
public:
	DriverAttribute(const Attributef::Ptr& attribute);
	~DriverAttribute();

	GLuint GetGPUID() const;

protected:
	void Init();
	void OnAttributeDestroy(const PTaskParameter& parameter);

protected:
	std::weak_ptr<Attribute<float>> m_attribute;
	GLuint m_buffer = 0;
};
using PDriverAttribute = std::shared_ptr<DriverAttribute>;

//驱动-索引属性
class DLL_EXPORT DriverIndexAttribute : public Driver
{
public:
	DriverIndexAttribute(const Attributei::Ptr& attribute);
	~DriverIndexAttribute();

	GLuint GetGPUID() const;

protected:
	void Init();
	void OnAttributeDestroy(const PTaskParameter& parameter);

protected:
	std::weak_ptr<Attribute<uint32_t>> m_attribute;
	GLuint m_buffer = 0;
};
using PDriverIndexAttribute = std::shared_ptr<DriverIndexAttribute>;

//驱动-管理者
class DLL_EXPORT DriverManager : public Object
{
public:
	static DriverManager* GetInstance();

	PDriverMaterial GetDriverMaterial(const PMaterial& material);
	PDriverGeometry GetDriverGeometry(const PGeometry& geometry);

	GLuint GetAttributeGPUID(ID idAttribute) const;
	GLuint GetIndexAttributeGPUID(ID idAttribute) const;

protected:
	DriverManager();

protected:
	std::unordered_map<ID, PDriverMaterial> m_mapDriverMaterial;
	std::unordered_map<ID, PDriverTexture> m_mapDriverTexture;

	std::unordered_map<ID, PDriverGeometry> m_mapDriverGeometry;
	std::unordered_map<ID, PDriverAttribute> m_mapDriverAttribute;
	std::unordered_map<ID, PDriverIndexAttribute> m_mapDriverIndexAttribute;
};

#pragma warning(pop)  // 恢复之前的警告状态