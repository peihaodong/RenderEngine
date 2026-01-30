#include "taskcommon.h"

TaskParameter::TaskParameter(uint32_t nTaskType)
	:m_nTaskType(nTaskType)
{

}

TaskParameter::~TaskParameter()
{

}

std::shared_ptr<TaskParameter> TaskParameter::Create(uint32_t nTaskType)
{
	return std::make_shared<TaskParameter>(nTaskType);
}