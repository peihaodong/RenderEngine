#pragma once
#include <base.h>
#include "constant.h"

class Texture;
using PTexture = std::shared_ptr<Texture>;

enum class EMaterialType
{
	EMT_Material
};

#pragma warning(push)  // 保存当前警告状态
#pragma warning(disable: 4251)  // 禁用 C4251 警告

//材质
class DLL_EXPORT Material : public Object, public std::enable_shared_from_this<Material>
{
public:
	Material();
	~Material();

	static std::shared_ptr<Material> New();

	//获取id
	ID GetID() const;
	//获取材质类型
	EMaterialType GetMaterialType() const;

	//depth set
	void SetDepthTest(bool enable);
	void SetDepthWrite(bool enable);
	void SetDepthFunction(EDepthFunction func);
	void SetDepthClearColor(double value);
	//depth get
	bool GetDepthTest() const;
	bool GetDepthWrite() const;
	EDepthFunction GetDepthFunction() const;
	double GetDepthClearColor() const;

	//blending set

	//blending get
	bool IsTransparent() const;

	//texture set
	void SetDiffuseTexture(const PTexture& texture);
	//texture get
	PTexture GetDiffuseTexture() const;

protected:
	ID m_id = 0;
	EMaterialType m_type = EMaterialType::EMT_Material;

	//depth
	bool m_bDepthTest = true;//是否开启深度检测
	bool m_bDepthWrite = true;//是否开启深度写入
	EDepthFunction m_nDepthFunction = EDepthFunction::EDF_LessOrEqual;
	double m_dDepthClearColor = 1.0;

	//blending
	bool m_bTransparent = false;//是否透明
	float m_fOpacity = 1.0f;//透明度

	//texture
	PTexture m_textureDiffuse;//漫反射纹理
};
using PMaterial = std::shared_ptr<Material>;

#pragma warning(pop)  // 恢复之前的警告状态