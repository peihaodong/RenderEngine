#pragma once
#include <base.h>
#include "constant.h"

//材质
class DLL_EXPORT Material : public Object, public std::enable_shared_from_this<Material>
{
public:
	Material();
	~Material();

	static std::shared_ptr<Material> New();

	//获取id
	ID GetID() const;

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

protected:
	ID m_id = 0;

	//depth
	bool m_bDepthTest = true;//是否开启深度检测
	bool m_bDepthWrite = true;//是否开启深度写入
	EDepthFunction m_nDepthFunction = EDepthFunction::EDF_LessOrEqual;
	double m_dDepthClearColor = 1.0;

	//blending
	bool m_bTransparent = false;//是否透明
	float m_fOpacity = 1.0f;//透明度
};
using PMaterial = std::shared_ptr<Material>;