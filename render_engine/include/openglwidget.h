#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <base.h>
#include "renderer.h"

class DLL_EXPORT OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_3_Core
{
public:
	OpenGLWidget(QWidget *parent = nullptr);
	~OpenGLWidget();
	
	//设置相机
	void SetCamera(const PCamera& camera);
	//设置相机控制器
	void SetCameraControl(const PCameraControl& camera_control);
	//设置场景
	void SetScene(const PScene& scene);

	//设置背景清除色
	void SetClearColor(float r, float g, float b, float a);

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
	PRenderer m_renderer;
};
