#include "openglwidgettest.h"

OpenGLWidgetTest::OpenGLWidgetTest(QWidget *parent /*= nullptr*/)
	: QOpenGLWidget(parent)
{
	m_renderer = std::make_shared<RenderDepth>();
}

OpenGLWidgetTest::~OpenGLWidgetTest()
{

}

void OpenGLWidgetTest::initializeGL()
{
	m_renderer->initializeGL();
}

void OpenGLWidgetTest::resizeGL(int w, int h)
{
	m_renderer->resizeGL(w, h);
}

void OpenGLWidgetTest::paintGL()
{
	m_renderer->paintGL();
}

void OpenGLWidgetTest::mousePressEvent(QMouseEvent* event)
{
	__super::mousePressEvent(event);
	m_renderer->mousePressEvent(event);
	update();
}

void OpenGLWidgetTest::mouseReleaseEvent(QMouseEvent* event)
{
	__super::mouseReleaseEvent(event);
	m_renderer->mouseReleaseEvent(event);
	update();
}

void OpenGLWidgetTest::mouseMoveEvent(QMouseEvent* event)
{
	__super::mouseMoveEvent(event);
	m_renderer->mouseMoveEvent(event);
	update();
}

void OpenGLWidgetTest::wheelEvent(QWheelEvent* event)
{
	__super::wheelEvent(event);
	m_renderer->wheelEvent(event);
	update();
}

void OpenGLWidgetTest::keyPressEvent(QKeyEvent* event)
{
	__super::keyPressEvent(event);
	m_renderer->keyPressEvent(event);
	update();
}

void OpenGLWidgetTest::keyReleaseEvent(QKeyEvent* event)
{
	__super::keyReleaseEvent(event);
	m_renderer->keyReleaseEvent(event);
	update();
}
