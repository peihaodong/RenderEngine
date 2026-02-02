#include "command.h"

UIntData::UIntData(uint32_t value)
	:m_value(value)
{

}

std::shared_ptr<UIntData> UIntData::New(uint32_t value)
{
	return std::make_shared<UIntData>(value);
}
