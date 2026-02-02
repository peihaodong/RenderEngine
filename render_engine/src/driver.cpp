#include "driver.h"
#include "taskmanager.h"
#include "command.h"

Driver::Driver()
{
	m_strClassName = "Driver";
}

DriverAttribute::DriverAttribute()
{
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Register(ETT_AttributeDestroy, this, &DriverAttribute::OnAttributeDestroy);
}

DriverAttribute::~DriverAttribute()
{
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Cancel(ETT_AttributeDestroy, this, &DriverAttribute::OnAttributeDestroy);
}

void DriverAttribute::OnAttributeDestroy(const PTaskParameter& parameter)
{
	std::shared_ptr<UIntData> data = std::dynamic_pointer_cast<UIntData>(parameter->m_data);
	if (!data)
		return;

}
