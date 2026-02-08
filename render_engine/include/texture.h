#pragma once
#include <base.h>
#include "constant.h"
#include <unordered_map>
#include <vector>

/*
可能出现多次加载纹理情况，所以将加载的纹理数据放入到TextureCache中统一取用
*/

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//纹理数据
struct DLL_EXPORT STextureData
{
	static std::shared_ptr<STextureData> New();

	HashCode m_nHashCode = 0;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<byte> m_datas;
};
using PSTextureData = std::shared_ptr<STextureData>;

//纹理缓存
class DLL_EXPORT TextureCache : public Object
{
public:
	static TextureCache* GetInstance();

	//获取纹理数据
	PSTextureData GetTextureData(const std::string& path);

protected:
	TextureCache();

protected:
	std::unordered_map<HashCode, PSTextureData> m_mapTextureData;
};

//纹理
class DLL_EXPORT Texture : public Object, public std::enable_shared_from_this<Texture>
{
public:
	explicit Texture(uint32_t uint);

	static std::shared_ptr<Texture> LoadTexture(const std::string& path, uint32_t uint);

	//set
	void SetWidth(uint32_t width);
	void SetHeight(uint32_t height);
	void SetDataType(EDataType nDataType);
	void SetData(const PSTextureData& data);
	void SetTextureType(ETextureType nTextureType);
	void SetFormat(ETextureFormat format);
	void SetInternalFormat(ETextureFormat format);
	void SetMinFilter(ETextureFilter filter);
	void SetMagFilter(ETextureFilter filter);
	void SetWrapS(ETextureWrapping wrap);
	void SetWrapT(ETextureWrapping wrap);
	void SetWrapR(ETextureWrapping wrap);

	//get
	ID GetID() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	EDataType GetDataType() const;
	PSTextureData GetData() const;
	ETextureType GetTextureType() const;
	ETextureFormat GetFormat() const;
	ETextureFormat GetInternalFormat() const;
	ETextureFilter GetMinFilter() const;
	ETextureFilter GetMagFilter() const;
	ETextureWrapping GetWrapS() const;
	ETextureWrapping GetWrapT() const;
	ETextureWrapping GetWrapR() const;
	uint32_t GetTextureUint() const;

protected:
	ID m_id = 0;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	EDataType m_nDataType = EDataType::EDT_UnsignedByte;
	PSTextureData m_texture_data;
	uint32_t m_nTextureUint = 0;//纹理单元

	ETextureType m_nTextureType = ETextureType::ETT_Texture2D;
	ETextureFormat m_nFormat = ETextureFormat::ETF_RGBA;//输入的图片数据像素格式
	ETextureFormat m_nInternalFormat = ETextureFormat::ETF_RGBA;//在shader采样时，内存中存放的格式
	ETextureFilter m_nMinFilter = ETextureFilter::ETF_Linear;
	ETextureFilter m_nMagFilter = ETextureFilter::ETF_Linear;
	ETextureWrapping m_nWrapS = ETextureWrapping::ETW_Repeat;
	ETextureWrapping m_nWrapT = ETextureWrapping::ETW_Repeat;
	ETextureWrapping m_nWrapR = ETextureWrapping::ETW_Repeat;
};
using PTexture = std::shared_ptr<Texture>;

#pragma warning(pop)  // 恢复之前的警告状态