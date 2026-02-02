#pragma once
#include "object3d.h"
#include <QMouseEvent>

class GameCameraControl;
class TrackBallCameraControl;

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//相机
class DLL_EXPORT Camera : public Object3D
{
	friend class GameCameraControl;
	friend class TrackBallCameraControl;
public:
	Camera();

	void SetWindowWidthHeight(int nWidth, int nHeight);

	//更新世界变换矩阵
	virtual glm::mat4 UpdateWorldMatrix(bool bUpdateParent = false, bool bUpdateChildren = false) override;
	//更新投影变换矩阵
	virtual glm::mat4 UpdateProjectionMatrix() = 0;

	//获取
	glm::mat4 GetWorldInverseMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

protected:
	glm::mat4 m_matrixWorldInverse = glm::mat4(1.0f);
	glm::mat4 m_matrixProjection = glm::mat4(1.0f);

	float m_fWindowWidth = 1.0f;
	float m_fWindowHeight = 1.0f;
};
using PCamera = std::shared_ptr<Camera>;

//正交投影相机
class DLL_EXPORT OrthographicCamera : public Camera
{
public:
	OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

	//更新投影变换矩阵
	virtual glm::mat4 UpdateProjectionMatrix() override;

protected:
	float m_left = 0;		//裁剪盒的左边界（x轴最小值）
	float m_right = 0;		//裁剪盒的右边界（x轴最大值）
	float m_bottom = 0;		//裁剪盒的下边界（y轴最小值）
	float m_top = 0;		//裁剪盒的上边界（y轴最大值）
	float m_nearPlane = 0;	//近裁剪面（z轴最小值，靠近相机的一面，通常为正数，Qt中默认右手坐标系）
	float m_farPlane = 0;	//远裁剪面（z轴最大值，远离相机的一面，必须大于nearPlane）
};

//透视投影相机
class DLL_EXPORT PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float verticalAngle, float nearPlane, float farPlane);

	//更新投影变换矩阵
	virtual glm::mat4 UpdateProjectionMatrix() override;

protected:
	float m_verticalAngle = 0;	//垂直视野角度（Field of View Y），单位：度（°）（视张角，眼睛张开的角度）
	float m_aspectRatio = 0;	//视口的宽高比（aspect ratio）= 窗口宽度 / 窗口高度
	float m_nearPlane = 0;		//近裁剪面（靠近相机的平面，z轴值，必须是正数且很小，如0.1）
	float m_farPlane = 0;		//远裁剪面（远离相机的平面，z轴值，必须大于nearPlane，如1000.0）
};

//////////////////////////////////////////////////////////////////////////

//相机控制器
class DLL_EXPORT CameraControl : public Object
{
public:
	CameraControl();
	
	//设置相机
	void SetCamera(const PCamera& camera);
	//获取相机
	PCamera GetCamera() const;

	void SetWindowWidthHeight(int nWidth, int nHeight);

	virtual bool mousePressEvent(QMouseEvent* event) = 0;
	virtual bool mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual bool mouseMoveEvent(QMouseEvent* event) = 0;
	virtual bool wheelEvent(QWheelEvent* event) = 0;
	virtual bool keyPressEvent(QKeyEvent* event) = 0;
	virtual bool keyReleaseEvent(QKeyEvent* event) = 0;

protected:
	std::weak_ptr<Camera> m_camera;
};
using PCameraControl = std::shared_ptr<CameraControl>;

//游戏相机控制器
class DLL_EXPORT GameCameraControl : public CameraControl
{
public:
	GameCameraControl();

	void SetSpeed(float speed);
	void SetSensitivity(float sensitivity);

	virtual bool mousePressEvent(QMouseEvent* event) override;
	virtual bool mouseReleaseEvent(QMouseEvent* event) override;
	virtual bool mouseMoveEvent(QMouseEvent* event) override;
	virtual bool wheelEvent(QWheelEvent* event) override;
	virtual bool keyPressEvent(QKeyEvent* event) override;
	virtual bool keyReleaseEvent(QKeyEvent* event) override;

protected:
	float m_fSpeed = 0.1f;	//移动速度
	float m_fSensitivity = 0.1f;//鼠标灵敏度

	glm::vec3 m_front = glm::vec3(0.0f);//当前相机的冲向
	float m_fPitchAngle = 0.0f;	//上下角度
	float m_fYawAngle = -90.0f;	//左右角度

	float m_curMouseX = 0.0f;
	float m_curMouseY = 0.0f;

	bool m_bLeftMousePress = false;		//鼠标左键按下
	bool m_bRightMousePress = false;	//鼠标右键按下
	bool m_bMiddleMousePress = false;	//鼠标中键按下
};

#pragma warning(pop)  // 恢复之前的警告状态