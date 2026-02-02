#pragma once
#include <QMatrix4x4>
#include <memory>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class GameCameraControlTest;
class TrackBallCameraControlTest;

//相机
class CameraTest
{
	friend class GameCameraControlTest;
	friend class TrackBallCameraControlTest;

public:
	CameraTest();
	virtual ~CameraTest();

	//设置位置
	void SetPosition(const QVector3D& eye, const QVector3D& right, const QVector3D& up);

	QMatrix4x4 GetViewMatrix() const;
	virtual QMatrix4x4 GetProjectionMatrix() const = 0;

	virtual void SetScale(float deltaScale) = 0;

protected:
	//生成视图矩阵
		//eye：相机的位置
		//center：相机注视的位置
		//up：相机上方向的参考向量
	QMatrix4x4 LookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up) const;

protected:
	QVector3D m_eye;	//相机位置
	QVector3D m_right;	//相机右方向的参考向量
	QVector3D m_up;		//相机上方向的参考向量
};
using PCameraTest = std::shared_ptr<CameraTest>;

//正交投影相机
class OrthoCameraTest : public CameraTest
{
public:
	OrthoCameraTest(float left, float right, float bottom, float top, float nearPlane, float farPlane);

	virtual QMatrix4x4 GetProjectionMatrix() const override;

	virtual void SetScale(float deltaScale) override;

protected:
	//生成正交投影矩阵
		//left：裁剪盒的左边界（x轴最小值）
		//right：裁剪盒的右边界（x轴最大值）
		//bottom：裁剪盒的下边界（y轴最小值）
		//top：裁剪盒的上边界（y轴最大值）
		//nearPlane：近裁剪面（z轴最小值，靠近相机的一面，通常为正数，Qt中默认右手坐标系）
		//farPlane：远裁剪面（z轴最大值，远离相机的一面，必须大于nearPlane）
	QMatrix4x4 Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) const;

protected:
	float m_left = 0;
	float m_right = 0;
	float m_bottom = 0;
	float m_top = 0;
	float m_nearPlane = 0;
	float m_farPlane = 0;

	float m_scale = 0;
};

//透视投影相机
class PerspectiveCameraTest : public CameraTest
{
public:
	PerspectiveCameraTest(float verticalAngle, float aspectRatio, float nearPlane, float farPlane);

	virtual QMatrix4x4 GetProjectionMatrix() const override;

	virtual void SetScale(float deltaScale) override;

protected:
	//生成透视投影矩阵
		//verticalAngle：垂直视野角度（Field of View Y），单位：度（°）（视张角，眼睛张开的角度）
		//aspectRatio：视口的宽高比（aspect ratio）= 窗口宽度 / 窗口高度
		//nearPlane：近裁剪面（靠近相机的平面，z轴值，必须是正数且很小，如0.1）
		//farPlane：远裁剪面（远离相机的平面，z轴值，必须大于nearPlane，如1000.0）
	QMatrix4x4 Perspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane) const;

protected:
	float m_verticalAngle = 0;
	float m_aspectRatio = 0;
	float m_nearPlane = 0;
	float m_farPlane = 0;
};

//相机控制
class CameraControlTest
{
public:
	CameraControlTest();
	virtual ~CameraControlTest();

	void SetCamera(const PCameraTest& camera);
	//设置敏感度
	void SetSensitivity(float sensitivity);
	//设置移动速度
	virtual void SetMoveSpeed(float speed) = 0;

	virtual void mousePressEvent(QMouseEvent* event) = 0;
	virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual void mouseMoveEvent(QMouseEvent* event) = 0;
	virtual void wheelEvent(QWheelEvent* event) = 0;
	virtual void keyPressEvent(QKeyEvent* event) = 0;
	virtual void keyReleaseEvent(QKeyEvent* event) = 0;

	QMatrix4x4 GetViewMatrix() const;
	QMatrix4x4 GetProjectionMatrix() const;

protected:
	PCameraTest m_camera;
	float m_sensitivity = 0.2f;//灵敏度
};
using PCameraControlTest = std::shared_ptr<CameraControlTest>;

//游戏相机控制
class GameCameraControlTest : public CameraControlTest
{
public:
	GameCameraControlTest();

	//设置移动速度
	virtual void SetMoveSpeed(float speed) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

protected:
	void pitch(float angle);
	void yaw(float angle);

protected:
	float m_fMoveSpeed = 0.1f;		//移动速度
	bool m_bLeftMousePress = false;		//鼠标左键按下
	bool m_bRightMousePress = false;	//鼠标右键按下
	bool m_bMiddleMousePress = false;	//鼠标中键按下
	QPoint m_curMousePos;				//当前鼠标点位置
	float m_pitch = 0.0f;
};

//轨迹球相机控制
class TrackBallCameraControlTest : public CameraControlTest
{
public:
	TrackBallCameraControlTest();

	//设置移动速度
	virtual void SetMoveSpeed(float speed) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

protected:
	void pitch(float angle);
	void yaw(float angle);

protected:
	float m_fMoveSpeed = 0.005f;		//移动速度
	bool m_bLeftMousePress = false;		//鼠标左键按下
	bool m_bRightMousePress = false;	//鼠标右键按下
	bool m_bMiddleMousePress = false;	//鼠标中键按下

	float m_curX = 0.0f;
	float m_curY = 0.0f;
};