#include "driver.h"
#include "taskmanager.h"
#include "command.h"
#include "material.h"
#include "texture.h"
#include "geometry.h"

Driver::Driver()
{
	m_strClassName = "Driver";

	initializeOpenGLFunctions();
}

void Driver::CheckError()
{
	if (glGetError() != GL_NO_ERROR)
		assert(false);
}

DriverState::DriverState()
{
	m_strClassName = "DriverState";
}

std::shared_ptr<DriverState> DriverState::New()
{
	return std::make_shared<DriverState>();
}

void DriverState::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void DriverState::SetViewport(const glm::vec4& viewport)
{
	if (m_viewport == viewport)
		return;

	m_viewport = viewport;

	//设置视口
	glViewport(
		static_cast<GLint>(m_viewport.x),
		static_cast<GLint>(m_viewport.y),
		static_cast<GLsizei>(m_viewport.z),
		static_cast<GLsizei>(m_viewport.w)
	);
}

void DriverState::ClearBackground()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void DriverState::DrawElements(uint32_t count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

DriverMaterial::DriverMaterial(const PMaterial& material)
{
	m_strClassName = "DriverMaterial";

	m_material = material;

	Init();
}

DriverMaterial::~DriverMaterial()
{

}

void DriverMaterial::Bind()
{
	m_program->bind();
}

void DriverMaterial::UpdateUniform(const char *name, const QMatrix4x4& value)
{
	m_program->setUniformValue(name, value);
}

void DriverMaterial::ActiveTextureUint()
{
	PMaterial material = m_material.lock();
	if (!material)
		return;
	PTexture texture = material->GetDiffuseTexture();
	if (!texture)
		return;
	DriverManager* manager = DriverManager::GetInstance();
	GLuint nTextureGPUID = manager->GetTextureGPUID(texture->GetID());

	m_program->setUniformValue("sampler", 0);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(ToGL(texture->GetTextureType()), nTextureGPUID);//绑定texture
}

void DriverMaterial::Init()
{
	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/material.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/material.frag"))
	{
		qDebug() << "片段着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->link())
	{
		qDebug() << "Shader程序链接失败：" << m_program->log();
		return;
	}
}

DriverTexture::DriverTexture(const PTexture& texture)
{
	m_strClassName = "DriverTexture";

	m_texture = texture;

	Init();
}

DriverTexture::~DriverTexture()
{
	if (m_nTexture)
	{
		glDeleteTextures(1, &m_nTexture);
		m_nTexture = 0;
	}
}

GLuint DriverTexture::GetGPUID() const
{
	return m_nTexture;
}

void DriverTexture::Init()
{
	auto texture = m_texture.lock();
	if (!texture)
		return;

	GLenum target = ToGL(texture->GetTextureType());

	glGenTextures(1, &m_nTexture);//创建texture
	glBindTexture(target, m_nTexture);//绑定texture

	//传输texture数据（开辟显存）
	glTexImage2D(
		target,
		0, 
		ToGL(texture->GetInternalFormat()),
		texture->GetWidth(), 
		texture->GetHeight(),
		0, 
		ToGL(texture->GetFormat()),
		ToGL(texture->GetDataType()),
		texture->GetData()->m_datas.data()
	);

	glGenerateMipmap(target);

	//设置纹理包裹方式
	glTexParameteri(target, GL_TEXTURE_WRAP_S, ToGL(texture->GetWrapS()));
	glTexParameteri(target, GL_TEXTURE_WRAP_T, ToGL(texture->GetWrapT()));

	//设置纹理过滤方式
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, ToGL(texture->GetMagFilter()));
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, ToGL(texture->GetMinFilter()));

	glBindTexture(target, 0);//解绑texture
}

DriverGeometry::DriverGeometry(const PGeometry& geometry)
{
	m_strClassName = "DriverGeometry";
	m_geometry = geometry;

	Init();
}

DriverGeometry::~DriverGeometry()
{
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}

void DriverGeometry::Bind()
{
	glBindVertexArray(m_vao);
}

uint32_t DriverGeometry::GetIndexAttributeCount() const
{
	return m_nIndexAttributeCount;
}

void DriverGeometry::Init()
{
	auto geometry = m_geometry.lock();
	if (!geometry)
		return;

	DriverManager* driver_manager = DriverManager::GetInstance();

	std::unordered_map<EAttributeType, Attributef::Ptr> mapAttribute = geometry->GetAllAttribute();
	Attributei::Ptr index_attribute = geometry->GetIndexAttribute();
	m_nIndexAttributeCount = index_attribute->GetCount();

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	for (auto iter = mapAttribute.begin(); iter != mapAttribute.end(); iter++)
	{
		EAttributeType nAttributeType = iter->first;
		Attributef::Ptr attribute = iter->second;
		GLuint vbo = driver_manager->GetAttributeGPUID(attribute->GetID());
		uint32_t nItemSize = attribute->GetItemSize();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);//绑定vbo
		glEnableVertexAttribArray(nAttributeType);//激活vao的位置
		glVertexAttribPointer(nAttributeType, nItemSize, GL_FLOAT, GL_FALSE, sizeof(float) * nItemSize, (void*)0);//vao的位置绑定vbo描述属性
	}

	GLuint ebo = driver_manager->GetIndexAttributeGPUID(index_attribute->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);//将ebo绑定在vao中
	
	glBindVertexArray(0);//解绑vao
}

DriverAttribute::DriverAttribute(const Attributef::Ptr& attribute)
{
	m_strClassName = "DriverAttribute";
	m_attribute = attribute;

	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Register(ETT_AttributeDestroy, this, &DriverAttribute::OnAttributeDestroy);

	Init();
}

DriverAttribute::~DriverAttribute()
{
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Cancel(ETT_AttributeDestroy, this, &DriverAttribute::OnAttributeDestroy);

	if (m_buffer)
	{
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
}

GLuint DriverAttribute::GetGPUID() const
{
	return m_buffer;
}

void DriverAttribute::Init()
{
	auto attribute = m_attribute.lock();
	if (!attribute)
		return;

	auto datas = attribute->GetDatas();

	glGenBuffers(1, &m_buffer);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, datas.size() * sizeof(float), datas.data(), GL_STATIC_DRAW);//向vbo中填充数据
}

void DriverAttribute::OnAttributeDestroy(const PTaskParameter& parameter)
{
	std::shared_ptr<UIntData> data = std::dynamic_pointer_cast<UIntData>(parameter->m_data);
	if (!data)
		return;

}

DriverIndexAttribute::DriverIndexAttribute(const Attributei::Ptr& attribute)
{
	m_strClassName = "DriverIndexAttribute";
	m_attribute = attribute;

	Init();
}


DriverIndexAttribute::~DriverIndexAttribute()
{
	if (m_buffer)
	{
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
}

GLuint DriverIndexAttribute::GetGPUID() const
{
	return m_buffer;
}

void DriverIndexAttribute::Init()
{
	auto attribute = m_attribute.lock();
	if (!attribute)
		return;

	auto datas = attribute->GetDatas();

	glGenBuffers(1, &m_buffer);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, datas.size() * sizeof(uint32_t), datas.data(), GL_STATIC_DRAW);//向ebo中填充数据
}

void DriverIndexAttribute::OnAttributeDestroy(const PTaskParameter& parameter)
{

}

DriverManager* DriverManager::GetInstance()
{
	static DriverManager inst;
	return &inst;
}

PDriverMaterial DriverManager::GetDriverMaterial(const PMaterial& material)
{
	ID idMaterial = material->GetID();
	auto iterMaterial = m_mapDriverMaterial.find(idMaterial);
	if (iterMaterial != m_mapDriverMaterial.end())
		return iterMaterial->second;

	PTexture texture = material->GetDiffuseTexture();
	if (texture)
	{
		ID idTexture = texture->GetID();
		auto iterTexture = m_mapDriverTexture.find(idTexture);
		if (iterTexture == m_mapDriverTexture.end())
		{
			std::shared_ptr<DriverTexture> driver_texture = std::make_shared<DriverTexture>(texture);
			m_mapDriverTexture[idTexture] = driver_texture;
		}
	}

	std::shared_ptr<DriverMaterial> driver_material = std::make_shared<DriverMaterial>(material);
	m_mapDriverMaterial[idMaterial] = driver_material;
	return driver_material;
}

PDriverGeometry DriverManager::GetDriverGeometry(const PGeometry& geometry)
{
	ID idGeometry = geometry->GetID();
	auto iterGeometry = m_mapDriverGeometry.find(idGeometry);
	if (iterGeometry != m_mapDriverGeometry.end())
		return iterGeometry->second;

	std::unordered_map<EAttributeType, Attributef::Ptr> attributes = geometry->GetAllAttribute();
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		auto attribute = iter->second;
		ID idAttribute = attribute->GetID();
		auto iterAttribute = m_mapDriverAttribute.find(idAttribute);
		if (iterAttribute == m_mapDriverAttribute.end())
		{
			PDriverAttribute driver_attribute = std::make_shared<DriverAttribute>(attribute);
			m_mapDriverAttribute[idAttribute] = driver_attribute;
		}
	}

	Attributei::Ptr attribute = geometry->GetIndexAttribute();
	ID idAttribute = attribute->GetID();
	auto iterAttribute = m_mapDriverIndexAttribute.find(idAttribute);
	if (iterAttribute == m_mapDriverIndexAttribute.end())
	{
		PDriverIndexAttribute driver_index_attribute = std::make_shared<DriverIndexAttribute>(attribute);
		m_mapDriverIndexAttribute[idAttribute] = driver_index_attribute;
	}

	std::shared_ptr<DriverGeometry> driver_geometry = std::make_shared<DriverGeometry>(geometry);
	m_mapDriverGeometry[idGeometry] = driver_geometry;
	return driver_geometry;
}

GLuint DriverManager::GetAttributeGPUID(ID idAttribute) const
{
	auto iter = m_mapDriverAttribute.find(idAttribute);
	if (iter == m_mapDriverAttribute.end())
		return 0;

	return iter->second->GetGPUID();
}

GLuint DriverManager::GetIndexAttributeGPUID(ID idAttribute) const
{
	auto iter = m_mapDriverIndexAttribute.find(idAttribute);
	if (iter == m_mapDriverIndexAttribute.end())
		return 0;

	return iter->second->GetGPUID();
}

GLuint DriverManager::GetTextureGPUID(ID idTexture) const
{
	auto iter = m_mapDriverTexture.find(idTexture);
	if (iter == m_mapDriverTexture.end())
		return 0;

	return iter->second->GetGPUID();
}

DriverManager::DriverManager()
{
	m_strClassName = "DriverManager";
}