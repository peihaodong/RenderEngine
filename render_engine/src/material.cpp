#include "material.h"
#include "command.h"
#include <taskmanager.h>
#include "texture.h"

Material::Material()
{
	m_strClassName = "Material";
	m_id = GenerateID();
}

Material::~Material()
{
	PTaskParameter parameter = TaskParameter::Create(ETT_MaterialDestroy);
	parameter->m_informer = this;
	TaskManager_Sync* manager = TaskManager_Sync::GetInstance();
	manager->Notify(parameter);
}

std::shared_ptr<Material> Material::New()
{
	return std::make_shared<Material>();
}

ID Material::GetID() const
{
	return m_id;
}

EMaterialType Material::GetMaterialType() const
{
	return m_type;
}

void Material::SetDepthTest(bool enable)
{
	m_bDepthTest = enable;
}

void Material::SetDepthWrite(bool enable)
{
	m_bDepthWrite = enable;
}

void Material::SetDepthFunction(EDepthFunction func)
{
	m_nDepthFunction = func;
}

void Material::SetDepthClearColor(double value)
{
	m_dDepthClearColor = value;
}

bool Material::GetDepthTest() const
{
	return m_bDepthTest;
}

bool Material::GetDepthWrite() const
{
	return m_bDepthWrite;
}

EDepthFunction Material::GetDepthFunction() const
{
	return m_nDepthFunction;
}

double Material::GetDepthClearColor() const
{
	return m_dDepthClearColor;
}

bool Material::IsTransparent() const
{
	return m_bTransparent;
}

void Material::SetDiffuseTexture(const PTexture& texture)
{
	m_textureDiffuse = texture;
}

PTexture Material::GetDiffuseTexture() const
{
	return m_textureDiffuse;
}
