#include "Object.h"

Object::Object(unsigned int preallocSize)  {
   vertices.reserve(preallocSize);

   xTranslation = 0;
   yTranslation = 0;
   zTranslation = 0;

   xRotation = 0;
   yRotation = 0;
   zRotation = 0;

   xScale = 0;
   yScale = 0;
   zScale = 0;
   //TODO fix here
   /*
   vertices.reserve(triangles.size());
   colors.reserve(colors_.size());
   textureUnwrap.reserve(unwrap.size());

   //TODO check here
   vertices.insert(vertices.end(), triangles.begin(), triangles.end());
   colors.insert(colors.end(), colors_.begin(), colors_.end());
   textureUnwrap.insert(textureUnwrap.end(), unwrap.begin(), unwrap.end());
    */
}

void Object::addTriangle(Triangle triangle) {
   vertices.emplace_back(std::move(triangle));
}

/*
float* Object::getWorldCoordinates() {
   // x y z r g b u v
   std::unique_ptr<float> vctArray(new float[vertices.size() + colors.size() + textureUnwrap.size()] {0});

   SquareMatrix translation(std::move(Transform::translateMatrix4(xTranslation, yTranslation, zTranslation)));
   //TODO check how to rotate with quaternions (check which axis to use)
   //SquareMatrix rotation(std::move(Rotation::rotationByQuaternion(Float4(1, 0, 0, 0), degree2Radiants(xRotation))));
   SquareMatrix xRotoMatrix(std::move(Rotation::rotationXAxisMatrix(xRotation)));
   SquareMatrix yRotoMatrix(std::move(Rotation::rotationXAxisMatrix(yRotation)));
   SquareMatrix zRotoMatrix(std::move(Rotation::rotationXAxisMatrix(zRotation)));
   SquareMatrix scale(Transform::scaleMatrix4(xScale, yScale, zScale));
   //TODO check apply here
   SquareMatrix transform(translation * zRotoMatrix * yRotoMatrix * xRotoMatrix * scale);

   for (int i = 0; i < vertices.size(); ++i) {
      //TODO fix repetition
      for (int j = 0; j < 3; ++j) {
         vctArray.get()[24*i + j*i] = transform.multiplyVector(vertices.at(i).getPoints().get()[j]).getVector().get()[0];
         vctArray.get()[24*i + j*i +1] = transform.multiplyVector(vertices.at(i).getPoints().get()[j]).getVector().get()[1];
         vctArray.get()[24*i + j*i +2] = transform.multiplyVector(vertices.at(i).getPoints().get()[j]).getVector().get()[2];
         vctArray.get()[24*i + j*i +3] = colors.at(j).getRed();
         vctArray.get()[24*i + j*i +4] = colors.at(j).getGreen();
         vctArray.get()[24*i + j*i +5] = colors.at(j).getBlue();
         vctArray.get()[24*i + j*i +6] = textureUnwrap.at(j).getX();
         vctArray.get()[24*i + j*i +7] = textureUnwrap.at(j).getY();
      }
   }

   return vctArray.release();
}
*/

const float& Object::getXTranslation() const {
   return xTranslation;
}

void Object::setXTranslation(const float& xTranslation) {
   Object::xTranslation = xTranslation;
}

const float& Object::getYTranslation() const {
   return yTranslation;
}

void Object::setYTranslation(const float& yTranslation) {
   Object::yTranslation = yTranslation;
}

const float& Object::getZTranslation() const {
   return zTranslation;
}

void Object::setZTranslation(const float& zTranslation) {
   Object::zTranslation = zTranslation;
}

const float& Object::getXRotation() const {
   return xRotation;
}

void Object::setXRotation(const float& xRotation) {
   Object::xRotation = xRotation;
}

const float& Object::getYRotation() const {
   return yRotation;
}

void Object::setYRotation(const float& yRotation) {
   Object::yRotation = yRotation;
}

const float& Object::getZRotation() const {
   return zRotation;
}

void Object::setZRotation(const float& zRotation) {
   Object::zRotation = zRotation;
}

const float& Object::getXScale() const {
   return xScale;
}

void Object::setXScale(const float& xScale) {
   Object::xScale = xScale;
}

const float& Object::getYScale() const {
   return yScale;
}

void Object::setYScale(const float& yScale) {
   Object::yScale = yScale;
}

const float& Object::getZScale() const {
   return zScale;
}

void Object::setZScale(const float& zScale) {
   Object::zScale = zScale;
}

const vector<Triangle> &Object::getVertices() const {
   return vertices;
}
