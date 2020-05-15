#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
   //TODO check if diffuse/reflective color is needed here
   float diffuseCoeff;     // Coefficiente di diffusione
   float specularCoeff;    // Coefficiente di specularità TODO check if necessary here
   float shininess;        // Grado di specularità
   float slope;            // Indice di rugosità

public:
   Material();

   float getLambertBRDF();
   float getPhongBRDF();
   float getBlinnBRDF();
   float getApproxBlinnBRDF();
};

#endif //MATERIAL_H
