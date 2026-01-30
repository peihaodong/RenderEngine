#include "cameratest.h"

CameraTest::CameraTest()
{}

CameraTest::~CameraTest()
{}

void CameraTest::SetPosition(const QVector3D& eye, const QVector3D& right, const QVector3D& up)
{
	m_eye = eye;
	m_right = right;
	m_up = up;
}

QMatrix4x4 CameraTest::GetViewMatrix() const
{
	QVector3D front = QVector3D::crossProduct(m_up, m_right);
	QVector3D center = m_eye + front;
	return LookAt(m_eye, center, m_up);
}

QMatrix4x4 CameraTest::LookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up) const
{
	// 步骤1：计算相机的三个正交轴（和原逻辑一致，无修改）
	QVector3D f = (center - eye).normalized(); // 相机前向向量（从eye指向center）
	QVector3D s = QVector3D::crossProduct(f, up).normalized(); // 相机右向向量
	QVector3D u = QVector3D::crossProduct(s, f); // 相机真实上方向（修正后正交）

	// 步骤2：构建旋转矩阵数据（列主序，和原数据一致）
	float rotMatData[16] = {
		s.x(),    u.x(),    -f.x(),   0.0f,
		s.y(),    u.y(),    -f.y(),   0.0f,
		s.z(),    u.z(),    -f.z(),   0.0f,
		-QVector3D::dotProduct(s, eye), -QVector3D::dotProduct(u, eye), QVector3D::dotProduct(f, eye), 1.0f
	};

	// 步骤3：正确加载旋转矩阵数据
	QMatrix4x4 rotMatrix(rotMatData, 4, 4);

	return rotMatrix;
}

OrthoCameraTest::OrthoCameraTest(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	:m_left(left), m_right(right), m_bottom(bottom), m_top(top), m_nearPlane(nearPlane), m_farPlane(farPlane)
{

}

QMatrix4x4 OrthoCameraTest::GetProjectionMatrix() const
{
	float scale = std::pow(2.0f, m_scale);
	return Ortho(m_left * scale, m_right * scale, m_bottom * scale, m_top * scale, m_nearPlane, m_farPlane);
}

void OrthoCameraTest::SetScale(float deltaScale)
{
	m_scale += deltaScale;
}

QMatrix4x4 OrthoCameraTest::Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) const
{
	QMatrix4x4 m;
	m.ortho(left, right, bottom, top, nearPlane, farPlane);
	return m;
}

PerspectiveCameraTest::PerspectiveCameraTest(float verticalAngle, float aspectRatio, float nearPlane, float farPlane)
	:m_verticalAngle(verticalAngle), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane)
{

}

QMatrix4x4 PerspectiveCameraTest::GetProjectionMatrix() const
{
	return Perspective(m_verticalAngle, m_aspectRatio, m_nearPlane, m_farPlane);
}

void PerspectiveCameraTest::SetScale(float deltaScale)
{
	QVector3D front = QVector3D::crossProduct(m_up, m_right);
	m_eye += (front * deltaScale);
}

QMatrix4x4 PerspectiveCameraTest::Perspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane) const
{
	QMatrix4x4 m;
	m.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
	return m;
}

CameraControlTest::CameraControlTest()
{

}

CameraControlTest::~CameraControlTest()
{

}

void CameraControlTest::SetCamera(const PCameraTest& camera)
{
	m_camera = camera;
}

void CameraControlTest::SetSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

QMatrix4x4 CameraControlTest::GetViewMatrix() const
{
	return m_camera->GetViewMatrix();
}

QMatrix4x4 CameraControlTest::GetProjectionMatrix() const
{
	return m_camera->GetProjectionMatrix();
}

GameCameraControlTest::GameCameraControlTest()
{

}

void GameCameraControlTest::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}

void GameCameraControlTest::mousePressEvent(QMouseEvent* event)
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
}

void GameCameraControlTest::mouseReleaseEvent(QMouseEvent* event)
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
}

void GameCameraControlTest::mouseMoveEvent(QMouseEvent* event)
{
	float xpos = event->pos().x();
	float ypos = event->pos().y();
	float curX = m_curMousePos.x();
	float curY = m_curMousePos.y();

	float deltaX = (xpos - curX) * m_sensitivity;
	float deltaY = (ypos - curY) * m_sensitivity;

	if (m_bRightMousePress)
	{
		pitch(-deltaY);
		yaw(-deltaX);
	}

	m_curMousePos = event->pos();
}

void GameCameraControlTest::wheelEvent(QWheelEvent* event)
{

}

void GameCameraControlTest::keyPressEvent(QKeyEvent* event)
{
	QVector3D direction;

	QVector3D front = QVector3D::crossProduct(m_camera->m_up, m_camera->m_right);
	QVector3D right = m_camera->m_right;

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
	}

	if (direction.length() != 0.0f)
	{
		direction = direction.normalized();
		m_camera->m_eye += (direction * m_fMoveSpeed);
	}
}

void GameCameraControlTest::keyReleaseEvent(QKeyEvent* event)
{

}

void GameCameraControlTest::pitch(float angle)
{
	m_pitch += angle;
	if (m_pitch > 89.0f || m_pitch < -89.0f) 
	{
		m_pitch -= angle;
		return;
	}

	QMatrix4x4 mat;
	mat.rotate(angle, m_camera->m_right);

	m_camera->m_up = (mat * QVector4D(m_camera->m_up, 0.0f)).toVector3D();
}

void GameCameraControlTest::yaw(float angle)
{
	QMatrix4x4 mat;
	mat.rotate(angle, QVector3D(0.0f, 1.0f, 0.0f));

	m_camera->m_up = (mat * QVector4D(m_camera->m_up, 0.0f)).toVector3D();
	m_camera->m_right = (mat * QVector4D(m_camera->m_right, 0.0f)).toVector3D();
}

TrackBallCameraControlTest::TrackBallCameraControlTest()
{

}

void TrackBallCameraControlTest::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}

void TrackBallCameraControlTest::mousePressEvent(QMouseEvent* event)
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
}

void TrackBallCameraControlTest::mouseReleaseEvent(QMouseEvent* event)
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
}

void TrackBallCameraControlTest::mouseMoveEvent(QMouseEvent* event)
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

		m_camera->m_eye += m_camera->m_up * deltaY;
		m_camera->m_eye -= m_camera->m_right * deltaX;
	}

	m_curX = xpos;
	m_curY = ypos;
}

void TrackBallCameraControlTest::wheelEvent(QWheelEvent* event)
{
	int wheelDelta = event->delta() / 120;
	m_camera->SetScale(m_sensitivity * wheelDelta);
}

void TrackBallCameraControlTest::keyPressEvent(QKeyEvent* event)
{

}

void TrackBallCameraControlTest::keyReleaseEvent(QKeyEvent* event)
{

}

void TrackBallCameraControlTest::pitch(float angle)
{
	QMatrix4x4 mat;
	mat.rotate(angle, m_camera->m_right);

	m_camera->m_up = (mat * QVector4D(m_camera->m_up, 0.0f)).toVector3D();
	m_camera->m_eye = (mat * QVector4D(m_camera->m_eye, 1.0f)).toVector3D();
}

void TrackBallCameraControlTest::yaw(float angle)
{
	QMatrix4x4 mat;
	mat.rotate(angle, QVector3D(0.0f, 1.0f, 0.0f));

	m_camera->m_up = (mat * QVector4D(m_camera->m_up, 0.0f)).toVector3D();
	m_camera->m_right = (mat * QVector4D(m_camera->m_right, 0.0f)).toVector3D();
	m_camera->m_eye = (mat * QVector4D(m_camera->m_eye, 1.0f)).toVector3D();
}
