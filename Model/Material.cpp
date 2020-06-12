#include <cmath>

#include "Material.h"

float Material::getLambertBRDF() {
   return diffuseFactor * M_1_PI;
}

float Material::getPhongBRDF() {
   return 0.5f * M_1_PI * (shininess + 1);
}

float Material::getBlinnBRDF() {
   return ((shininess + 2)*(shininess + 4) * 0.125f * M_1_PI) / (powf(2, -shininess*0.5f) + shininess) ;
}

float Material::getApproxBlinnBRDF() {
   return (shininess + 8) * 0.125f * M_1_PI;
}


