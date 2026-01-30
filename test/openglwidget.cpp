#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent /*= nullptr*/)
	: QOpenGLWidget(parent)
{
	m_renderer = std::make_shared<RenderDepth>();
}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
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
	m_renderer->mousePressEvent(event);
	update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	__super::mouseReleaseEvent(event);
	m_renderer->mouseReleaseEvent(event);
	update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	__super::mouseMoveEvent(event);
	m_renderer->mouseMoveEvent(event);
	update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	__super::wheelEvent(event);
	m_renderer->wheelEvent(event);
	update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	__super::keyPressEvent(event);
	m_renderer->keyPressEvent(event);
	update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
	__super::keyReleaseEvent(event);
	m_renderer->keyReleaseEvent(event);
	update();
}
