#include "camera.h"
#include "constant.h"

Camera::Camera()
{
	m_strClassName = "Camera";
}

void Camera::SetWindowWidthHeight(int nWidth, int nHeight)
{
	m_fWindowWidth = nWidth;
	m_fWindowHeight = nHeight;

	UpdateProjectionMatrix();
}

glm::mat4 Camera::UpdateWorldMatrix(bool bUpdateParent /*= false*/, bool bUpdateChildren /*= false*/)
{
	__super::UpdateWorldMatrix(bUpdateParent, bUpdateChildren);

	m_matrixWorldInverse = glm::inverse(m_matrixWorld);
	
	return m_matrixWorldInverse;
}

glm::mat4 Camera::GetWorldInverseMatrix() const
{
	return m_matrixWorldInverse;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_matrixProjection;
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	:m_left(left), m_right(right), m_bottom(bottom), m_top(top), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
	m_strClassName = "OrthographicCamera";

	UpdateProjectionMatrix();
}

glm::mat4 OrthographicCamera::UpdateProjectionMatrix()
{
	m_matrixProjection = glm::ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
	return m_matrixProjection;
}

PerspectiveCamera::PerspectiveCamera(float verticalAngle, float nearPlane, float farPlane)
	:m_verticalAngle(verticalAngle), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
	m_strClassName = "PerspectiveCamera";

	UpdateProjectionMatrix();
}

glm::mat4 PerspectiveCamera::UpdateProjectionMatrix()
{
	m_aspectRatio = m_fWindowWidth / m_fWindowHeight;
	m_matrixProjection = glm::perspective(glm::radians(m_verticalAngle), m_aspectRatio, m_nearPlane, m_farPlane);
	return m_matrixProjection;
}

CameraControl::CameraControl()
{
	m_strClassName = "CameraControl";
}

void CameraControl::SetCamera(const PCamera& camera)
{
	m_camera = camera;
}

PCamera CameraControl::GetCamera() const
{
	return m_camera.lock();
}

void CameraControl::SetWindowWidthHeight(int nWidth, int nHeight)
{
	auto camera = m_camera.lock();
	if (camera)
		camera->SetWindowWidthHeight(nWidth, nHeight);
}

GameCameraControl::GameCameraControl()
{
	m_strClassName = "GameCameraControl";
}

void GameCameraControl::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

void GameCameraControl::SetSensitivity(float sensitivity)
{
	m_fSensitivity = sensitivity;
}

bool GameCameraControl::mousePressEvent(QMouseEvent* event)
{
	m_curMouseX = event->pos().x();
	m_curMouseY = event->pos().y();

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

	if (m_bRightMousePress)
	{
		float deltaX = (xpos - m_curMouseX) * m_fSensitivity;
		float deltaY = (m_curMouseX - ypos) * m_fSensitivity;

		m_fPitchAngle += deltaY;
		m_fYawAngle += deltaX;

		//不能仰面翻过去，也不能前滚翻
		if (m_fPitchAngle >= 90.0f)
		{
			m_fPitchAngle = 89.0f;
		}
		if (m_fPitchAngle <= -90.0f) 
		{
			m_fPitchAngle = -89.0f;
		}

		m_front.y = sin(glm::radians(m_fPitchAngle));
		m_front.x = cos(glm::radians(m_fYawAngle)) * cos(glm::radians(m_fPitchAngle));
		m_front.z = sin(glm::radians(m_fYawAngle)) * cos(glm::radians(m_fPitchAngle));

		//一定要注意传进去的是看向哪个点，而不是直接把方向传进去
		auto camera = m_camera.lock();
		if (camera)
		{
			camera->LookAt(m_front + camera->GetLocalPosition(), glm::vec3(0.0, 1.0, 0.0));
		}
	}

	m_curMouseX = xpos;
	m_curMouseY = ypos;

	return false;
}

bool GameCameraControl::wheelEvent(QWheelEvent* event)
{
	return true;
}

bool GameCameraControl::keyPressEvent(QKeyEvent* event)
{
	glm::vec3 direction;

	auto camera = m_camera.lock();
	if (!camera)
		return true;

	glm::vec3 front = camera->GetLocalDirection();
	glm::vec3 right = camera->GetRight();
	glm::vec3 position = camera->GetLocalPosition();

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
		direction -= right;
	}
	break;
	case Qt::Key_D:
	{
		direction += right;
	}
	break;
	default:
		return true;
	}

	if (direction.x == 0 && direction.y == 0 && direction.z == 0)
		return true;

	direction = glm::normalize(direction);
	position += direction * m_fSpeed;
	camera->SetPosition(position);

	return false;
}

bool GameCameraControl::keyReleaseEvent(QKeyEvent* event)
{
	return true;
}
