#ifndef FLOAT4_H
#define FLOAT4_H

#include "FloatVector.h"
#include "Float3.h"

class Float4 : public FloatVector {
public:
   Float4();
   Float4(const float& x, const float& y, const float& z, const float& w);
   Float4(const Float4& point);
   Float4(const Float3& float3, bool isPoint);
   Float4(Float4&& point);
   Float4(const FloatVector& point);
   Float4(FloatVector&& point);
   ~Float4();

   Float4& operator=(const Float4& point);
   Float4& operator=(Float4&& point);

   Float4 operator+(const Float4& point) const;
   Float4 operator-(const Float4& point) const;
   Float4& operator+=(const Float4& point);
   Float4& operator-=(const Float4& point);

   Float4 operator*(const float& scalar) const;
   Float4& operator*=(const float& scalar);

   float dotProduct(const Float4& point) const;
   //Float4 crossProduct(const Float4& point) const;

   float l2Norm() const;
   void normalize();

   float getX() const;
   float getY() const;
   float getZ() const;
   float getW() const;

   void setX(const float& x);
   void setY(const float& y);
   void setZ(const float& z);
   void setW(const float& w);

   Float3 getFloat3() const;
};

#endif //FLOAT4_H
