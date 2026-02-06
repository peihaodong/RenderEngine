#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent /*= nullptr*/)
	: QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);  // 关键：允许控件通过点击/键盘获取焦点
	setFocus();  // 主动获取焦点（可选，确保启动后立即响应）

	m_renderer = std::make_shared<Renderer>();
}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::SetCamera(const PCamera& camera)
{
	m_renderer->SetCamera(camera);
}

void OpenGLWidget::SetCameraControl(const PCameraControl& camera_control)
{
	m_renderer->SetCameraControl(camera_control);
}

void OpenGLWidget::SetScene(const PScene& scene)
{
	m_renderer->SetScene(scene);
}

void OpenGLWidget::SetClearColor(float r, float g, float b, float a)
{
	m_renderer->SetClearColor(r, g, b, a);
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	m_renderer->initializeGL();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	m_renderer->resizeGL(w, h);
}

void OpenGLWidget::paintGL()
{
	m_renderer->paintGL();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	__super::mousePressEvent(event);

	if (!m_renderer->mousePressEvent(event))
		update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	__super::mouseReleaseEvent(event);

	if (!m_renderer->mouseReleaseEvent(event))
		update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	__super::mouseMoveEvent(event);

	if (!m_renderer->mouseMoveEvent(event))
		update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	__super::wheelEvent(event);

	if (!m_renderer->wheelEvent(event))
		update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	__super::keyPressEvent(event);

	if (!m_renderer->keyPressEvent(event))
		update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
	__super::keyReleaseEvent(event);

	if (!m_renderer->keyReleaseEvent(event))
		update();
}
