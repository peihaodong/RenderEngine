#include "camera.h"
#include "constant.h"

Camera::Camera()
{
	m_strClassName = "Camera";
}

Camera::~Camera()
{}

void Camera::SetPosition(float x, float y, float z)
{
	SetPosition(glm::vec3(x, y, z));
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void Camera::SetWindowWidthHeight(int width, int height)
{
	m_width = width;
	m_height = height;
}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::vec3 front = glm::cross(m_up, m_right);
	glm::vec3 center = m_position + front;

	return glm::lookAt(m_position, center, m_up);
}

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	:m_left(left), m_right(right), m_bottom(bottom), m_top(top), m_nearPlane(nearPlane), m_farPlane(farPlane)
{

}

glm::mat4 OrthoCamera::GetProjectionMatrix() const
{
	float scale = std::pow(2.0f, m_scale);
	return glm::ortho(m_left * scale, m_right * scale, m_bottom * scale, m_top * scale, m_nearPlane, m_farPlane);
}

void OrthoCamera::SetScale(float deltaScale)
{
	m_scale += deltaScale;
}

PerspectiveCamera::PerspectiveCamera(float verticalAngle, float nearPlane, float farPlane)
	:m_verticalAngle(verticalAngle), m_nearPlane(nearPlane), m_farPlane(farPlane)
{

}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const
{
	return glm::perspective(m_verticalAngle, m_aspectRatio, m_nearPlane, m_farPlane);
}

void PerspectiveCamera::SetScale(float deltaScale)
{
	auto front = glm::cross(m_up, m_right);
	m_position += (front * deltaScale);
}

void PerspectiveCamera::SetWindowWidthHeight(int width, int height)
{
	m_aspectRatio = float(width) / float(height);
}

CameraControl::CameraControl()
{

}

CameraControl::~CameraControl()
{

}

void CameraControl::SetCamera(const PCamera& camera)
{
	m_camera = camera;
}

void CameraControl::SetSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

GameCameraControl::GameCameraControl()
{

}

void GameCameraControl::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}

bool GameCameraControl::mousePressEvent(QMouseEvent* event)
{
	m_curMousePos = event->pos();

	switch (event->button())
	{
	case Qt::LeftButton:
	{
		m_bLeftMousePress = true;
	}
	break;
	case Qt::RightButton:
	{
		m_bRightMousePress = true;
	}
	break;
	case Qt::MiddleButton:
	{
		m_bMiddleMousePress = true;
	}
	break;
	}

	return false;
}

bool GameCameraControl::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
	{
		m_bLeftMousePress = false;
	}
	break;
	case Qt::RightButton:
	{
		m_bRightMousePress = false;
	}
	break;
	case Qt::MiddleButton:
	{
		m_bMiddleMousePress = false;
	}
	break;
	}

	return false;
}

bool GameCameraControl::mouseMoveEvent(QMouseEvent* event)
{
	float xpos = event->pos().x();
	float ypos = event->pos().y();
	float curX = m_curMousePos.x();
	float curY = m_curMousePos.y();

	float deltaX = (xpos - curX) * m_sensitivity;
	float deltaY = (ypos - curY) * m_sensitivity;

	if (m_bRightMousePress)
	{
		pitch(deltaY);
		yaw(deltaX);
	}

	m_curMousePos = event->pos();

	return false;
}

bool GameCameraControl::wheelEvent(QWheelEvent* event)
{
	return true;
}

bool GameCameraControl::keyPressEvent(QKeyEvent* event)
{
	glm::vec3 direction(0.0f);

	glm::vec3 front = glm::cross(m_camera->m_up, m_camera->m_right);
	glm::vec3 right = m_camera->m_right;

	switch (event->key())
	{
	case Qt::Key_W:
	{
		direction += front;
	}
	break;
	case Qt::Key_S:
	{
		direction -= front;
	}
	break;
	case Qt::Key_A:
	{
		direction += right;
	}
	break;
	case Qt::Key_D:
	{
		direction -= right;
	}
	break;
	default:
		return true;
	}

	if (glm::length(direction) != 0.0f)
	{
		direction = glm::normalize(direction);
		m_camera->m_position += (direction * m_fMoveSpeed);
	}

	return false;
}

bool GameCameraControl::keyReleaseEvent(QKeyEvent* event)
{
	return false;
}

void GameCameraControl::pitch(float angle)
{
	m_pitch += angle;
	if (m_pitch > 89.0f || m_pitch < -89.0f)
	{
		m_pitch -= angle;
		return;
	}

	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_camera->m_right);
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
}

void GameCameraControl::yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	m_camera->m_right = mat * glm::vec4(m_camera->m_right, 0.0f);
}

TrackBallCameraControl::TrackBallCameraControl()
{

}

void TrackBallCameraControl::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}

bool TrackBallCameraControl::mousePressEvent(QMouseEvent* event)
{
	float xpos = event->pos().x();
	float ypos = event->pos().y();
	m_curX = xpos;
	m_curY = ypos;

	switch (event->button())
	{
	case Qt::LeftButton:
	{
		m_bLeftMousePress = true;
	}
	break;
	case Qt::RightButton:
	{
		m_bRightMousePress = true;
	}
	break;
	case Qt::MiddleButton:
	{
		m_bMiddleMousePress = true;
	}
	break;
	}

	return false;
}

bool TrackBallCameraControl::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
	{
		m_bLeftMousePress = false;
	}
	break;
	case Qt::RightButton:
	{
		m_bRightMousePress = false;
	}
	break;
	case Qt::MiddleButton:
	{
		m_bMiddleMousePress = false;
	}
	break;
	}

	return false;
}

bool TrackBallCameraControl::mouseMoveEvent(QMouseEvent* event)
{
	float xpos = event->pos().x();
	float ypos = event->pos().y();

	if (m_bLeftMousePress)
	{
		//调整相机的各类参数
		//1 计算经线跟纬线旋转的增量角度(正负都有可能）
		float deltaX = (xpos - m_curX) * m_sensitivity;
		float deltaY = (ypos - m_curY) * m_sensitivity;

		//2 分开pitch跟yaw各自计算
		pitch(-deltaY);
		yaw(-deltaX);
	}
	else if (m_bMiddleMousePress)
	{
		float deltaX = (xpos - m_curX) * m_fMoveSpeed;
		float deltaY = (ypos - m_curY) * m_fMoveSpeed;

		m_camera->m_position += m_camera->m_up * deltaY;
		m_camera->m_position -= m_camera->m_right * deltaX;
	}

	m_curX = xpos;
	m_curY = ypos;

	return false;
}

bool TrackBallCameraControl::wheelEvent(QWheelEvent* event)
{
	int wheelDelta = event->delta() / 120;
	m_camera->SetScale(m_sensitivity * wheelDelta);

	return false;
}

bool TrackBallCameraControl::keyPressEvent(QKeyEvent* event)
{
	return true;
}

bool TrackBallCameraControl::keyReleaseEvent(QKeyEvent* event)
{
	return true;
}

void TrackBallCameraControl::pitch(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_camera->m_right);
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	m_camera->m_position = mat * glm::vec4(m_camera->m_position, 1.0f);
}

void TrackBallCameraControl::yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	m_camera->m_right = mat * glm::vec4(m_camera->m_right, 0.0f);
	m_camera->m_position = mat * glm::vec4(m_camera->m_position, 1.0f);
}
