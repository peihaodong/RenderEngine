#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include "renderertest.h"

class OpenGLWidgetTest  : public QOpenGLWidget, public QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
	OpenGLWidgetTest(QWidget *parent = nullptr);
	~OpenGLWidgetTest();

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

protected:
	PRendererTest m_renderer;
};
