#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Vector/Float3.h"

struct Material {
   float ambientFactor;
   Float3 ambientCoeff;
   float diffuseFactor;
   Float3 diffuseCoeff;
   float specularFactor;
   Float3 specularCoeff;
   float shininess;

   float roughness;

   float getLambertBRDF();
   float getPhongBRDF();
   float getBlinnBRDF();
   float getApproxBlinnBRDF();
};

#endif //MATERIAL_H
