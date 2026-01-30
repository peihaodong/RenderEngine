#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

using ByteArray = std::vector<uint8_t>;
using PByteArray = std::shared_ptr<std::vector<uint8_t>>;

//获取唯一ID
uint64_t GetUniqueID();

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

class DLL_EXPORT Object : public std::enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

protected:
	std::string m_strClassName;
};

class DLL_EXPORT Data : public Object
{
public:
	Data();
};
using PData = std::shared_ptr<Data>;

#pragma warning(pop)  // 恢复之前的警告状态