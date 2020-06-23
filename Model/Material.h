#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Vector/Float3.h"

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

class Material {
   //TODO check if names are correct (Factor =? Coeff)
   float ambientFactor;
   Float3 ambientCoeff;
   float diffuseFactor;
   Float3 diffuseCoeff;
   float specularFactor;
   Float3 specularCoeff;
   float shininess;

   float roughness;

public:
   Material();

   float getLambertBRDF();
   float getPhongBRDF();
   float getBlinnBRDF();
   float getApproxBlinnBRDF();

   float getRoughness() const;

   void setRoughness(float roughness);

   float getAmbientFactor() const;

   void setAmbientFactor(float ambientFactor);

   const Float3 &getAmbientCoeff() const;

   void setAmbientCoeff(const Float3 &ambientCoeff);

   float getDiffuseFactor() const;

   void setDiffuseFactor(float diffuseFactor);

   const Float3 &getDiffuseCoeff() const;

   void setDiffuseCoeff(const Float3 &diffuseCoeff);

   float getSpecularFactor() const;

   void setSpecularFactor(float specularFactor);

   const Float3 &getSpecularCoeff() const;

   void setSpecularCoeff(const Float3 &specularCoeff);

   float getShininess() const;

   void setShininess(float shininess);
};

#endif //MATERIAL_H
