#include "base.h"
#include <atomic>

uint64_t GetUniqueID()
{
	static std::atomic<uint64_t> g_ID = 0;
	g_ID++;
	return g_ID;
}

Object::Object()
	:m_strClassName("Object")
{

}

Object::~Object()
{

}

Data::Data()
{
	m_strClassName = "Data";
}
