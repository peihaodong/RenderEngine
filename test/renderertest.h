#pragma once

#include <QOpenGLFunctions_4_3_Core>
#include <memory>
#include <QOpenGLShaderProgram>
#include "cameratest.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class RendererTest  : public QOpenGLFunctions_4_3_Core
{
public:
	RendererTest();

	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

protected:

	void checkError();

	//生成视图矩阵
		//eye：相机的位置
		//center：相机注视的位置
		//up：相机上方向的参考向量
	QMatrix4x4 LookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up) const;
	//生成正交投影矩阵
		//left：裁剪盒的左边界（x轴最小值）
		//right：裁剪盒的右边界（x轴最大值）
		//bottom：裁剪盒的下边界（y轴最小值）
		//top：裁剪盒的上边界（y轴最大值）
		//nearPlane：近裁剪面（z轴最小值，靠近相机的一面，通常为正数，Qt中默认右手坐标系）
		//farPlane：远裁剪面（z轴最大值，远离相机的一面，必须大于nearPlane）
	QMatrix4x4 Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) const;
	//生成透视投影矩阵
		//verticalAngle：垂直视野角度（Field of View Y），单位：度（°）（视张角，眼睛张开的角度）
		//aspectRatio：视口的宽高比（aspect ratio）= 窗口宽度 / 窗口高度
		//nearPlane：近裁剪面（靠近相机的平面，z轴值，必须是正数且很小，如0.1）
		//farPlane：远裁剪面（远离相机的平面，z轴值，必须大于nearPlane，如1000.0）
	QMatrix4x4 Perspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane) const;

protected:
	virtual void PrepareShader() = 0;
	virtual void PrepareVAO() = 0;
	virtual void PrepareTexture();
	virtual void PrepareCamera();
	virtual void PrepareProjection();

	virtual void Render() = 0;

protected:
	int m_width = 0;
	int m_height = 0;
};
using PRendererTest = std::shared_ptr<RendererTest>;

//渲染-三角形
class RenderTriangle : public RendererTest
{
public:
	~RenderTriangle();

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = {0};//position color
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
};

//渲染-图片
class RenderImage : public RendererTest
{
public:
	~RenderImage();

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;
	virtual void PrepareTexture() override;

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = { 0 };//position uv
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
	GLuint m_texture = 0;
};

//渲染-视图矩阵
class RenderViewMatrix : public RendererTest
{
public:
	~RenderViewMatrix();

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;
	virtual void PrepareTexture() override;
	virtual void PrepareCamera();

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = { 0 };//position uv
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
	GLuint m_texture = 0;
	QMatrix4x4 m_ModelMatrix;	//模型矩阵
	QMatrix4x4 m_ViewMatrix;	//视图矩阵
};

//渲染-投影矩阵
class RenderProjectionMatrix : public RendererTest
{
public:
	~RenderProjectionMatrix();

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;
	virtual void PrepareTexture() override;
	virtual void PrepareCamera() override;
	virtual void PrepareProjection() override;

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = { 0 };//position uv
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
	GLuint m_texture = 0;
	QMatrix4x4 m_ModelMatrix;		//模型矩阵
	QMatrix4x4 m_ViewMatrix;		//视图矩阵
	QMatrix4x4 m_ProjectionMatrix;	//投影矩阵
};

//渲染-相机
class RenderCamera : public RendererTest
{
public:
	RenderCamera();
	~RenderCamera();

	virtual void resizeGL(int w, int h) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;
	virtual void PrepareTexture() override;

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = { 0 };//position uv
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
	GLuint m_texture = 0;
	QMatrix4x4 m_ModelMatrix;		//模型矩阵
	QMatrix4x4 m_ViewMatrix;		//视图矩阵
	QMatrix4x4 m_ProjectionMatrix;	//投影矩阵
	PCameraControlTest m_camera_control;
};

//渲染-深度
class RenderDepth : public RendererTest
{
public:
	RenderDepth();
	~RenderDepth();

	virtual void initializeGL();
	virtual void resizeGL(int w, int h) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

protected:
	virtual void PrepareShader() override;
	virtual void PrepareVAO() override;
	virtual void PrepareTexture() override;

	virtual void Render() override;

protected:
	std::shared_ptr<QOpenGLShaderProgram> m_program;
	GLuint m_vbo[2] = { 0 };//position uv
	GLuint m_vao = 0;
	GLuint m_ebo = 0;
	GLuint m_texture[2] = {0};
	QMatrix4x4 m_ModelMatrix;		//模型矩阵
	QMatrix4x4 m_ViewMatrix;		//视图矩阵
	QMatrix4x4 m_ProjectionMatrix;	//投影矩阵
	PCameraControlTest m_camera_control;
};

/*
glsl
		基础数据类型
		void	空类型
		bool	true、false
		int		至少16位的整数
		float	32位浮点数
		double	64位浮点数

		向量数据类型
		vecn	包含n个float分量的向量
		bvecn	包含n个bool分量的向量
		ivecn	包含n个int分量的向量
		uvecn	包含n个unsigned int分量的向量
		dvecn	包含n个double分量的向量
*/