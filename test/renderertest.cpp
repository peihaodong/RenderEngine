#include "renderertest.h"
#include <assert.h>
#include <QImage>

RendererTest::RendererTest()
{}

void RendererTest::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//设置清屏颜色为白色

	PrepareShader();
	PrepareVAO();
	PrepareTexture();
	PrepareCamera();
}

void RendererTest::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, m_width, m_height);//设置视口

	PrepareProjection();
}

void RendererTest::paintGL()
{
	Render();
}

void RendererTest::mousePressEvent(QMouseEvent* event)
{
	
}

void RendererTest::mouseReleaseEvent(QMouseEvent* event)
{
	
}

void RendererTest::mouseMoveEvent(QMouseEvent* event)
{
	
}

void RendererTest::wheelEvent(QWheelEvent* event)
{
	
}

void RendererTest::keyPressEvent(QKeyEvent* event)
{
	
}

void RendererTest::keyReleaseEvent(QKeyEvent* event)
{
	
}

void RendererTest::checkError()
{
	if (glGetError() == GL_NO_ERROR)
		return;
	assert(false);
}

QMatrix4x4 RendererTest::LookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up) const
{
	// 步骤1：计算相机的三个正交轴（和原逻辑一致，无修改）
	QVector3D f = (center - eye).normalized(); // 相机前向向量（从eye指向center）
	QVector3D s = QVector3D::crossProduct(f, up).normalized(); // 相机右向向量
	QVector3D u = QVector3D::crossProduct(s, f); // 相机真实上方向（修正后正交）

	// 步骤2：构建旋转矩阵数据（列主序，和原数据一致）
	float rotMatData[16] = {
		s.x(),    u.x(),    -f.x(),   0.0f,
		s.y(),    u.y(),    -f.y(),   0.0f,
		s.z(),    u.z(),    -f.z(),   0.0f,
		-QVector3D::dotProduct(s, eye), -QVector3D::dotProduct(u, eye), QVector3D::dotProduct(f, eye), 1.0f
	};

	// 步骤3：正确加载旋转矩阵数据
	QMatrix4x4 rotMatrix(rotMatData, 4, 4);

	return rotMatrix;
}

QMatrix4x4 RendererTest::Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) const
{
	QMatrix4x4 m;
	m.ortho(left, right, bottom, top, nearPlane, farPlane);
	return m;
}

QMatrix4x4 RendererTest::Perspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane) const
{
	QMatrix4x4 m;
	m.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
	return m;
}

void RendererTest::PrepareTexture()
{

}

void RendererTest::PrepareCamera()
{

}

void RendererTest::PrepareProjection()
{

}

RenderTriangle::~RenderTriangle()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
}

void RenderTriangle::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_triangle.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_triangle.frag"))
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

void RenderTriangle::PrepareVAO()
{
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderTriangle::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);//清理画布

	m_program->bind();

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_program->release();
}

RenderImage::~RenderImage()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void RenderImage::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_image.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_image.frag"))
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

void RenderImage::PrepareVAO()
{
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderImage::PrepareTexture()
{
	QImage image;
	if (!image.load(":/image/wukong.jpg"))
		return;
	QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

	glGenTextures(1, &m_texture);//创建texture
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//解绑texture
}

void RenderImage::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);//清理画布

	m_program->bind();

	m_program->setUniformValue("sampler", 0);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_program->release();
}

RenderViewMatrix::~RenderViewMatrix()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void RenderViewMatrix::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_view_matrix.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_view_matrix.frag"))
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

void RenderViewMatrix::PrepareVAO()
{
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderViewMatrix::PrepareTexture()
{
	QImage image;
	if (!image.load(":/image/wukong.jpg"))
		return;
	QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

	glGenTextures(1, &m_texture);//创建texture
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//解绑texture
}

void RenderViewMatrix::PrepareCamera()
{
	m_ViewMatrix = LookAt(QVector3D(0.5f, 0.0f, 0.5f), QVector3D(0.5f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
}

void RenderViewMatrix::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);//清理画布

	m_program->bind();

	m_program->setUniformValue("sampler", 0);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture
	m_program->setUniformValue("ModelMatrix", m_ModelMatrix);
	m_program->setUniformValue("ViewMatrix", m_ViewMatrix);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_program->release();
}

RenderProjectionMatrix::~RenderProjectionMatrix()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void RenderProjectionMatrix::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_projection_matrix.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_projection_matrix.frag"))
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

void RenderProjectionMatrix::PrepareVAO()
{
// 	float positions[] = {
// 		-0.5f, -0.5f, 0.0f,
// 		0.5f, -0.5f, 0.0f,
// 		0.0f, 0.5f, 0.0f
// 	};
	float positions[] = {
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderProjectionMatrix::PrepareTexture()
{
	QImage image;
	if (!image.load(":/image/wukong.jpg"))
		return;
	QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

	glGenTextures(1, &m_texture);//创建texture
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//解绑texture
}

void RenderProjectionMatrix::PrepareCamera()
{
	m_ViewMatrix = LookAt(QVector3D(0.0f, 0.0f, 5.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
}

void RenderProjectionMatrix::PrepareProjection()
{
	//m_ProjectionMatrix = Ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	m_ProjectionMatrix = Perspective(60, (float)m_width / (float)m_height, 0.1f, 1000.0f);
}

void RenderProjectionMatrix::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);//清理画布

	m_program->bind();

	m_program->setUniformValue("sampler", 0);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture
	m_program->setUniformValue("ModelMatrix", m_ModelMatrix);
	m_program->setUniformValue("ViewMatrix", m_ViewMatrix);
	m_program->setUniformValue("ProjectionMatrix", m_ProjectionMatrix);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_program->release();
}

RenderCamera::RenderCamera()
{
	
}

RenderCamera::~RenderCamera()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void RenderCamera::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, m_width, m_height);//设置视口

	//m_camera_control = std::make_shared<GameCameraControlTest>();
	m_camera_control = std::make_shared<TrackBallCameraControlTest>();
	m_camera_control->SetSensitivity(0.4);
	int size = 2.0f;
	std::shared_ptr<OrthoCameraTest> camera = std::make_shared<OrthoCameraTest>(-size, size, -size, size, -size, size);
	//std::shared_ptr<PerspectiveCamera> camera = std::make_shared<PerspectiveCameraTest>(60.0f, (float)m_width / (float)m_height, 0.1f, 1000.0f);
	camera->SetPosition(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
	m_camera_control->SetCamera(camera);
}

void RenderCamera::mousePressEvent(QMouseEvent* event)
{
	m_camera_control->mousePressEvent(event);
}

void RenderCamera::mouseReleaseEvent(QMouseEvent* event)
{
	m_camera_control->mouseReleaseEvent(event);
}

void RenderCamera::mouseMoveEvent(QMouseEvent* event)
{
	m_camera_control->mouseMoveEvent(event);
}

void RenderCamera::wheelEvent(QWheelEvent* event)
{
	m_camera_control->wheelEvent(event);
}

void RenderCamera::keyPressEvent(QKeyEvent* event)
{
	m_camera_control->keyPressEvent(event);
}

void RenderCamera::keyReleaseEvent(QKeyEvent* event)
{
	m_camera_control->keyReleaseEvent(event);
}

void RenderCamera::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_camera.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_camera.frag"))
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

void RenderCamera::PrepareVAO()
{
	// 	float positions[] = {
// 		-0.5f, -0.5f, 0.0f,
// 		0.5f, -0.5f, 0.0f,
// 		0.0f, 0.5f, 0.0f
// 	};
	float positions[] = {
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderCamera::PrepareTexture()
{
	QImage image;
	if (!image.load(":/image/wukong.jpg"))
		return;
	QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

	glGenTextures(1, &m_texture);//创建texture
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//解绑texture
}

void RenderCamera::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);//清理画布

	m_program->bind();

	m_program->setUniformValue("sampler", 0);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture);//绑定texture
	m_program->setUniformValue("ModelMatrix", m_ModelMatrix);
	m_program->setUniformValue("ViewMatrix", m_camera_control->GetViewMatrix());
	m_program->setUniformValue("ProjectionMatrix", m_camera_control->GetProjectionMatrix());

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_program->release();
}

RenderDepth::RenderDepth()
{

}

RenderDepth::~RenderDepth()
{
	if (m_vbo[0])
	{
		glDeleteBuffers(2, m_vbo);
		m_vbo[0] = m_vbo[1] = 0;
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_texture[0])
	{
		glDeleteTextures(2, m_texture);
		m_texture[0] = m_texture[1] = 0;
	}
}

void RenderDepth::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);//开启深度检测
	glDepthFunc(GL_LESS);//当前片元深度值较小的时候，才能通过测试

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//设置清屏颜色为白色
	glClearDepth(1.0);//设置清屏深度为1.0

	PrepareShader();
	PrepareVAO();
	PrepareTexture();
}

void RenderDepth::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, m_width, m_height);//设置视口

	//m_camera_control = std::make_shared<GameCameraControlTest>();
	m_camera_control = std::make_shared<TrackBallCameraControlTest>();
	m_camera_control->SetSensitivity(0.4);
	int size = 4.0f;
	std::shared_ptr<OrthoCameraTest> camera = std::make_shared<OrthoCameraTest>(-size, size, -size, size, -size, size * 2);
	//std::shared_ptr<PerspectiveCamera> camera = std::make_shared<PerspectiveCameraTest>(60.0f, (float)m_width / (float)m_height, 0.1f, 1000.0f);
	camera->SetPosition(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
	m_camera_control->SetCamera(camera);
}

void RenderDepth::mousePressEvent(QMouseEvent* event)
{
	m_camera_control->mousePressEvent(event);
}

void RenderDepth::mouseReleaseEvent(QMouseEvent* event)
{
	m_camera_control->mouseReleaseEvent(event);
}

void RenderDepth::mouseMoveEvent(QMouseEvent* event)
{
	m_camera_control->mouseMoveEvent(event);
}

void RenderDepth::wheelEvent(QWheelEvent* event)
{
	m_camera_control->wheelEvent(event);
}

void RenderDepth::keyPressEvent(QKeyEvent* event)
{
	m_camera_control->keyPressEvent(event);
}

void RenderDepth::keyReleaseEvent(QKeyEvent* event)
{
	m_camera_control->keyReleaseEvent(event);
}

void RenderDepth::PrepareShader()
{
	if (m_program)
		return;

	m_program = std::make_shared<QOpenGLShaderProgram>();
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/render_camera.vert"))
	{
		qDebug() << "顶点着色器编译失败：" << m_program->log();
		return;
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/render_camera.frag"))
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

void RenderDepth::PrepareVAO()
{
	// 	float positions[] = {
// 		-0.5f, -0.5f, 0.0f,
// 		0.5f, -0.5f, 0.0f,
// 		0.0f, 0.5f, 0.0f
// 	};
	float positions[] = {
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	glGenBuffers(2, m_vbo);//创建vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);//向vbo中填充数据
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//向vbo中填充数据

	glGenBuffers(1, &m_ebo);//创建ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//向ebo中填充数据

	glGenVertexArrays(1, &m_vao);//创建vao
	glBindVertexArray(m_vao);//绑定vao

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);//绑定vbo
	glEnableVertexAttribArray(0);//激活vao的0号位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);//vao的0号位置绑定vbo描述属性
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);//绑定vbo
	glEnableVertexAttribArray(1);//激活vao的1号位置
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);//vao的1号位置绑定vbo描述属性

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);//将ebo绑定在vao中

	glBindVertexArray(0);//解绑vao
}

void RenderDepth::PrepareTexture()
{
	QImage imageWuKong;
	if (!imageWuKong.load(":/image/wukong.jpg"))
		return;
	QImage imageLuFei;
	if (!imageLuFei.load(":/image/lufei.jpg"))
		return;
	QImage glImageWuKong = imageWuKong.convertToFormat(QImage::Format_RGBA8888).mirrored();
	QImage glImageLuFei = imageLuFei.convertToFormat(QImage::Format_RGBA8888).mirrored();

	glGenTextures(2, m_texture);//创建texture

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//绑定texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImageWuKong.width(), glImageWuKong.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImageWuKong.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//////////////////////////////////////////////////////////////////////////

	glBindTexture(GL_TEXTURE_2D, m_texture[1]);//绑定texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImageLuFei.width(), glImageLuFei.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImageLuFei.bits());//传输texture数据（开辟显存）

	//glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//解绑texture
}

void RenderDepth::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清理画布

	m_program->bind();
	glBindVertexArray(m_vao);

	m_program->setUniformValue("sampler", 0);
	m_program->setUniformValue("ViewMatrix", m_camera_control->GetViewMatrix());
	m_program->setUniformValue("ProjectionMatrix", m_camera_control->GetProjectionMatrix());

	m_program->setUniformValue("ModelMatrix", m_ModelMatrix);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//绑定texture
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	QMatrix4x4 matrixTransform;
	matrixTransform.translate(0.8f, 0.0f, -1.0f);
	m_program->setUniformValue("ModelMatrix", matrixTransform);
	glActiveTexture(GL_TEXTURE0);//激活0号纹理单元
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);//绑定texture
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	m_program->release();
}
