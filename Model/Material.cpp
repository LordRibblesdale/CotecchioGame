#include <cmath>

#include "Material.h"

Material::Material() {

}

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

float Material::getAmbientFactor() const {
   return ambientFactor;
}

void Material::setAmbientFactor(float ambientFactor) {
   Material::ambientFactor = ambientFactor;
}

const Float3 &Material::getAmbientCoeff() const {
   return ambientCoeff;
}

void Material::setAmbientCoeff(const Float3 &ambientCoeff) {
   Material::ambientCoeff = ambientCoeff;
}

float Material::getDiffuseFactor() const {
   return diffuseFactor;
}

void Material::setDiffuseFactor(float diffuseFactor) {
   Material::diffuseFactor = diffuseFactor;
}

const Float3 &Material::getDiffuseCoeff() const {
   return diffuseCoeff;
}

void Material::setDiffuseCoeff(const Float3 &diffuseCoeff) {
   Material::diffuseCoeff = diffuseCoeff;
}

float Material::getSpecularFactor() const {
   return specularFactor;
}

void Material::setSpecularFactor(float specularFactor) {
   Material::specularFactor = specularFactor;
}

const Float3 &Material::getSpecularCoeff() const {
   return specularCoeff;
}

void Material::setSpecularCoeff(const Float3 &specularCoeff) {
   Material::specularCoeff = specularCoeff;
}

float Material::getShininess() const {
   return shininess;
}

void Material::setShininess(float shininess) {
   Material::shininess = shininess;
}

float Material::getRoughness() const {
   return roughness;
}

void Material::setRoughness(float roughness) {
   Material::roughness = roughness;
}


