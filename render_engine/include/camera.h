#pragma once
#include <base.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <QMouseEvent>

class GameCameraControl;
class TrackBallCameraControl;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//相机
class DLL_EXPORT Camera : public Object
{
	friend class GameCameraControl;
	friend class TrackBallCameraControl;

public:
	Camera();
	virtual ~Camera();

	//设置相机位置
	void SetPosition(float x, float y, float z);
	void SetPosition(const glm::vec3& position);
	//设置窗口宽高
	virtual void SetWindowWidthHeight(int width, int height);

	glm::mat4 GetViewMatrix() const;
	virtual glm::mat4 GetProjectionMatrix() const = 0;

	virtual void SetScale(float deltaScale) = 0;

protected:
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);	//相机位置
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);	//相机右方向的参考向量
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);		//相机上方向的参考向量

	int m_width = 800;
	int m_height = 600;
};
using PCamera = std::shared_ptr<Camera>;

//正交投影相机
class DLL_EXPORT OrthoCamera : public Camera
{
public:
	OrthoCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

	virtual glm::mat4 GetProjectionMatrix() const override;

	virtual void SetScale(float deltaScale) override;

protected:
	float m_left = 0;		//裁剪盒的左边界（x轴最小值）
	float m_right = 0;		//裁剪盒的右边界（x轴最大值）
	float m_bottom = 0;		//裁剪盒的下边界（y轴最小值）
	float m_top = 0;		//裁剪盒的上边界（y轴最大值）
	float m_nearPlane = 0;	//近裁剪面（z轴最小值，靠近相机的一面，通常为正数，Qt中默认右手坐标系）
	float m_farPlane = 0;	//远裁剪面（z轴最大值，远离相机的一面，必须大于nearPlane）

	float m_scale = 0;
};

//透视投影相机
class DLL_EXPORT PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float verticalAngle, float nearPlane, float farPlane);

	virtual glm::mat4 GetProjectionMatrix() const override;

	virtual void SetScale(float deltaScale) override;
	//设置窗口宽高
	virtual void SetWindowWidthHeight(int width, int height) override;

protected:
	float m_verticalAngle = 0;	//垂直视野角度（Field of View Y），单位：度（°）（视张角，眼睛张开的角度）
	float m_aspectRatio = 1;	//视口的宽高比（aspect ratio）= 窗口宽度 / 窗口高度
	float m_nearPlane = 0;		//近裁剪面（靠近相机的平面，z轴值，必须是正数且很小，如0.1）
	float m_farPlane = 0;		//远裁剪面（远离相机的平面，z轴值，必须大于nearPlane，如1000.0）
};

//相机控制
class DLL_EXPORT CameraControl
{
public:
	CameraControl();
	virtual ~CameraControl();

	void SetCamera(const PCamera& camera);
	//设置敏感度
	void SetSensitivity(float sensitivity);
	//设置移动速度
	virtual void SetMoveSpeed(float speed) = 0;

	virtual bool mousePressEvent(QMouseEvent* event) = 0;
	virtual bool mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual bool mouseMoveEvent(QMouseEvent* event) = 0;
	virtual bool wheelEvent(QWheelEvent* event) = 0;
	virtual bool keyPressEvent(QKeyEvent* event) = 0;
	virtual bool keyReleaseEvent(QKeyEvent* event) = 0;

protected:
	PCamera m_camera;
	float m_sensitivity = 0.2f;//灵敏度
};
using PCameraControl = std::shared_ptr<CameraControl>;

//游戏相机控制
class DLL_EXPORT GameCameraControl : public CameraControl
{
public:
	GameCameraControl();

	//设置移动速度
	virtual void SetMoveSpeed(float speed) override;

	virtual bool mousePressEvent(QMouseEvent* event) override;
	virtual bool mouseReleaseEvent(QMouseEvent* event) override;
	virtual bool mouseMoveEvent(QMouseEvent* event) override;
	virtual bool wheelEvent(QWheelEvent* event) override;
	virtual bool keyPressEvent(QKeyEvent* event) override;
	virtual bool keyReleaseEvent(QKeyEvent* event) override;

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
class DLL_EXPORT TrackBallCameraControl : public CameraControl
{
public:
	TrackBallCameraControl();

	//设置移动速度
	virtual void SetMoveSpeed(float speed) override;

	virtual bool mousePressEvent(QMouseEvent* event) override;
	virtual bool mouseReleaseEvent(QMouseEvent* event) override;
	virtual bool mouseMoveEvent(QMouseEvent* event) override;
	virtual bool wheelEvent(QWheelEvent* event) override;
	virtual bool keyPressEvent(QKeyEvent* event) override;
	virtual bool keyReleaseEvent(QKeyEvent* event) override;

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

#pragma warning(pop)  // 恢复之前的警告状态