#include "Material_Lambert.h"

#include "BRDF.h"

Material_Lambert::Material_Lambert(const Elite::RGBColor& diffuseColor, float diffuseReflectance)
    : m_DiffuseColor{diffuseColor}
    , m_DiffuseReflectance{diffuseReflectance}
{
    ( m_DiffuseReflectance > 1 ) ? m_DiffuseReflectance = 1 : ( m_DiffuseReflectance < 0 ) ? m_DiffuseReflectance = 0 : m_DiffuseReflectance;
}

Elite::RGBColor Material_Lambert::Shade(const HitRecord& , const Elite::FVector3& , const Elite::FVector3& ) const
{
    return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}
