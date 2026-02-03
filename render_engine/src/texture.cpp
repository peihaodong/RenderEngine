#include "texture.h"
#include <QImage>
#include <type_traits>

std::shared_ptr<STextureData> STextureData::New()
{
	return std::make_shared<STextureData>();
}

TextureCache* TextureCache::GetInstance()
{
	static TextureCache inst;
	return &inst;
}


PSTextureData TextureCache::GetTextureData(const std::string& path)
{
	std::hash<std::string> hasher;
	HashCode nHashCode = hasher(path);
	auto iter = m_mapTextureData.find(nHashCode);
	if (iter != m_mapTextureData.end())
		return iter->second;

	QImage image;
	if (!image.load(QString::fromStdString(path)))
		return nullptr;
	QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

	PSTextureData data;
	data->m_nHashCode = nHashCode;
	data->m_width = glImage.width();
	data->m_height = glImage.height();
	auto nDataCount = glImage.byteCount();
	data->m_datas.resize(nDataCount);
	memcpy(data->m_datas.data(), glImage.bits(), nDataCount);

	m_mapTextureData[nHashCode] = data;

	return data;
}

TextureCache::TextureCache()
{
	m_strClassName = "TextureCache";
}

Texture::Texture()
{
	m_strClassName = "Texture";
	m_id = GenerateID();
}

std::shared_ptr<Texture> Texture::LoadTexture(const std::string& path)
{
	TextureCache* cache = TextureCache::GetInstance();
	PSTextureData texture_data = cache->GetTextureData(path);
	if (!texture_data)
		return nullptr;

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->SetWidth(texture_data->m_width);
	texture->SetHeight(texture_data->m_height);
	texture->SetData(texture_data);

	return texture;
}

void Texture::SetWidth(uint32_t width)
{
	m_width = width;
}

void Texture::SetHeight(uint32_t height)
{
	m_height = height;
}

void Texture::SetDataType(EDataType nDataType)
{
	m_nDataType = nDataType;
}

void Texture::SetData(const PSTextureData& data)
{
	m_texture_data = data;
}

void Texture::SetTextureType(ETextureType nTextureType)
{
	m_nTextureType = nTextureType;
}

void Texture::SetFormat(ETextureFormat format)
{
	m_nFormat = format;
}

void Texture::SetInternalFormat(ETextureFormat format)
{
	m_nInternalFormat = format;
}

void Texture::SetMinFilter(ETextureFilter filter)
{
	m_nMinFilter = filter;
}

void Texture::SetMagFilter(ETextureFilter filter)
{
	m_nMagFilter = filter;
}

void Texture::SetWrapS(ETextureWrapping wrap)
{
	m_nWrapS = wrap;
}

void Texture::SetWrapT(ETextureWrapping wrap)
{
	m_nWrapT = wrap;
}

void Texture::SetWrapR(ETextureWrapping wrap)
{
	m_nWrapR = wrap;
}

ID Texture::GetID() const
{
	return m_id;
}

uint32_t Texture::GetWidth() const
{
	return m_width;
}

uint32_t Texture::GetHeight() const
{
	return m_height;
}

EDataType Texture::GetDataType() const
{
	return m_nDataType;
}

PSTextureData Texture::GetData() const
{
	return m_texture_data;
}

ETextureType Texture::GetTextureType() const
{
	return m_nTextureType;
}

ETextureFormat Texture::GetFormat() const
{
	return m_nFormat;
}

ETextureFormat Texture::GetInternalFormat() const
{
	return m_nInternalFormat;
}

ETextureFilter Texture::GetMinFilter() const
{
	return m_nMinFilter;
}

ETextureFilter Texture::GetMagFilter() const
{
	return m_nMagFilter;
}

ETextureWrapping Texture::GetWrapS() const
{
	return m_nWrapS;
}

ETextureWrapping Texture::GetWrapT() const
{
	return m_nWrapT;
}

ETextureWrapping Texture::GetWrapR() const
{
	return m_nWrapR;
}
