#pragma once
#include <base.h>
#include <QOpenGLFunctions_4_3_Core>
#include <taskcommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//驱动
class Driver : public Object, public QOpenGLFunctions_4_3_Core, public std::enable_shared_from_this<Driver>
{
public:
	Driver();
};
using PDriver = std::shared_ptr<Driver>;

//驱动-状态
class DriverState : public Driver
{
public:

protected:
	glm::vec4 mCurrentViewport;
};
using PDriverState = std::shared_ptr<DriverState>;

//驱动-属性
class DriverAttribute : public Driver
{
public:
	DriverAttribute();
	~DriverAttribute();

protected:
	void OnAttributeDestroy(const PTaskParameter& parameter);
};