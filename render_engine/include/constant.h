#pragma once
#include <cstdint>
#include <QOpenGLFunctions_4_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <QVector3D>
#include <QMatrix4x4>

using byte = unsigned char;
using HashCode = size_t;

using ID = uint32_t;
static ID GenerateID()
{
	static ID g_id = 0;
	g_id++;
	return g_id;
}

//////////////////////////////////////////////////////////////////////////

static QVector3D ToVector3D(const glm::vec3& value)
{
	return QVector3D(value.x, value.y, value.z);
}
static glm::vec3 ToVector3D(const QVector3D& value)
{
	return glm::vec3(value.x(), value.y(), value.z());
}

static QMatrix3x3 ToMatrix3x3(const glm::mat3& value)
{
	QMatrix3x3 qtMat;
	std::memcpy(qtMat.data(), &value[0][0], sizeof(float) * 9);
	return qtMat;
}
static glm::mat3 ToMatrix3x3(const QMatrix3x3& value)
{
	glm::mat3 glmMat;
	std::memcpy(&glmMat[0][0], value.constData(), sizeof(float) * 9);
	return glmMat;
}

static QMatrix4x4 ToMatrix4x4(const glm::mat4& value)
{
	QMatrix4x4 qtMat;
	std::memcpy(qtMat.data(), &value[0][0], sizeof(float) * 16);
	return qtMat;
}
static glm::mat4 ToMatrix4x4(const QMatrix4x4& value)
{
	glm::mat4 glmMat;
	std::memcpy(&glmMat[0][0], value.constData(), sizeof(float) * 16);
	return glmMat;
}

//////////////////////////////////////////////////////////////////////////

enum class EDataType
{
	EDT_UnsignedByte,
	EDT_Byte,
	EDT_Int32,
	EDT_UInt32,
	EDT_Float
};
template<typename T>
static EDataType ToDataType()
{
	if (typeid(T) == typeid(float))
		return EDataType::EDT_Float;
	if (typeid(T) == typeid(int))
		return EDataType::EDT_Int32;
	if (typeid(T) == typeid(char))
		return EDataType::EDT_Byte;
	if (typeid(T) == typeid(unsigned char))
		return EDataType::EDT_UnsignedByte;
	if (typeid(T) == typeid(uint32_t))
		return EDataType::EDT_UInt32;
	return EDataType::EDT_Float;
}
static uint32_t ToGL(const EDataType& value)
{
	switch (value)
	{
	case EDataType::EDT_UnsignedByte:
		return GL_UNSIGNED_BYTE;
	case EDataType::EDT_Float:
		return GL_FLOAT;
	case EDataType::EDT_Byte:
		return GL_BYTE;
	case EDataType::EDT_Int32:
		return GL_INT;
	case EDataType::EDT_UInt32:
		return GL_UNSIGNED_INT;
	default:
		return 0;
	}
}
static size_t ToSize(const EDataType& value)
{
	switch (value)
	{
	case EDataType::EDT_UnsignedByte:
		return sizeof(unsigned char);
	case EDataType::EDT_Float:
		return sizeof(float);
	case EDataType::EDT_Byte:
		return sizeof(char);
	case EDataType::EDT_Int32:
		return sizeof(int);
	case EDataType::EDT_UInt32:
		return sizeof(uint32_t);
	default:
		return 0;
	}
}

enum class EBufferAllocType
{
	EBAT_StaticDraw,
	EBAT_DynamicDraw
};
static uint32_t ToGL(const EBufferAllocType& value)
{
	switch (value)
	{
	case EBufferAllocType::EBAT_StaticDraw:
		return GL_STATIC_DRAW;
	case EBufferAllocType::EBAT_DynamicDraw:
		return GL_DYNAMIC_DRAW;
	default:
		return 0;
	}
}

enum class EDepthFunction
{
	EDF_Less,
	EDF_LessOrEqual,
	EDF_Bigger,
	EDF_BiggerOrEqual
};
static uint32_t ToGL(const EDepthFunction& value)
{
	switch (value) 
	{
	case EDepthFunction::EDF_Less:
		return GL_LESS;
	case EDepthFunction::EDF_Bigger:
		return GL_GREATER;
	case EDepthFunction::EDF_LessOrEqual:
		return GL_LEQUAL;
	case EDepthFunction::EDF_BiggerOrEqual:
		return GL_GEQUAL;
	default:
		return GL_NONE;
	}
}

enum class ETextureType
{
	ETT_Texture2D,
	ETT_TextureCubeMap
};
static GLuint ToGL(const ETextureType& value)
{
	switch (value) 
	{
	case ETextureType::ETT_Texture2D:
		return GL_TEXTURE_2D;
	case ETextureType::ETT_TextureCubeMap:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return GL_NONE;
	}
}

enum class ETextureFormat
{
	ETF_RGB,
	ETF_RGBA
};
static GLuint ToGL(const ETextureFormat& format)
{
	switch (format)
	{
	case ETextureFormat::ETF_RGB:
		return GL_RGB;
	case ETextureFormat::ETF_RGBA:
		return GL_RGBA;
	default:
		return 0;
	}
}

enum class ETextureFilter
{
	ETF_Linear,
	ETF_Nearest
};
static uint32_t ToGL(const ETextureFilter& value)
{
	switch (value) 
	{
	case ETextureFilter::ETF_Linear:
		return GL_LINEAR;
	case ETextureFilter::ETF_Nearest:
		return GL_NEAREST;
	default:
		return 0;
	}
}

enum class ETextureWrapping 
{
	ETW_Repeat,
	ETW_ClampToEdge,
	ETW_ClampToBorder,
	ETW_MirroredRepeat,
};
static uint32_t ToGL(const ETextureWrapping& value)
{
	switch (value) {
	case ETextureWrapping::ETW_Repeat:
		return GL_REPEAT;
	case ETextureWrapping::ETW_ClampToEdge:
		return GL_CLAMP_TO_EDGE;
	case ETextureWrapping::ETW_MirroredRepeat:
		return GL_MIRRORED_REPEAT;
	case ETextureWrapping::ETW_ClampToBorder:
		return GL_CLAMP_TO_BORDER;
	default:
		return 0;
	}
}