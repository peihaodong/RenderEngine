#pragma once

#include <base.h>
#include <memory>
#include <string>
#include <functional>

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

struct DLL_EXPORT TaskParameter
{
public:
	explicit TaskParameter(uint32_t nTaskType);
	~TaskParameter();

	static std::shared_ptr<TaskParameter> Create(uint32_t nTaskType);

	uint32_t m_nTaskType = 0;	//任务类型
	PData m_data;				//任务所携带的数据
	void* m_informer = nullptr;	//任务的通知者this指针
};
using PTaskParameter = std::shared_ptr<TaskParameter>;

//类函数指针
template<typename T>
using TaskCallback = void(T::*)(const PTaskParameter&);

#pragma warning(pop)  // 恢复之前的警告状态