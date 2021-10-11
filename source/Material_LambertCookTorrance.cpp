#include "Material_LambertCookTorrance.h"
#include "BRDF.h"

Material_LambertCookTorrance::Material_LambertCookTorrance(const Elite::RGBColor& diffuseColor, const Elite::RGBColor& albido, float roughness, bool metalness, float fresnelIntensity)
    : m_DiffuseColor{ diffuseColor }
    , m_AlbidoColor{ albido }
    , m_Roughness{ roughness }
    , m_FresnelIntensity{fresnelIntensity}
    , m_IsMetal{ metalness }

{
}

Elite::RGBColor Material_LambertCookTorrance::Shade(const HitRecord& hitRecord, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection) const
{
    Elite::FVector3 halfVector = ( viewDirection - lightDirection ) / ( Elite::Magnitude(viewDirection - lightDirection) );

    Elite::RGBColor specular = BRDF::CookTorrance(m_AlbidoColor, m_Roughness, m_FresnelIntensity, hitRecord.normal, viewDirection, lightDirection, halfVector, m_IsMetal);

    if(!m_IsMetal )
    {
        Elite::RGBColor schlick = BRDF::Schlick(m_AlbidoColor, m_FresnelIntensity, viewDirection, halfVector, m_IsMetal);
        Elite::RGBColor kd = Elite::RGBColor(1.0f, 1.0f, 1.0f) - schlick;
        Elite::RGBColor diffuse = BRDF::Lambert(kd.r, m_DiffuseColor);
        return diffuse + specular;
    }

    return specular;
}
