#ifndef FLOATVECTOR_H
#define FLOATVECTOR_H

#include <memory>
#include "../Exception/ExceptionNotifier.h"

struct FloatVector {
   std::unique_ptr<float> vector_;
   unsigned int size_;

public:
   FloatVector(unsigned int size, float* data);
   FloatVector(unsigned int size, const std::initializer_list<float>& data);
   FloatVector(const FloatVector& floatVector);
   FloatVector(FloatVector&& floatVector);
   ~FloatVector();

   FloatVector& operator=(const FloatVector& vector);
   FloatVector& operator=(FloatVector&& vector);

   bool operator==(const FloatVector& vector) const;
   bool operator!=(const FloatVector& vector) const;

   FloatVector operator+(const FloatVector& vector) const;
   void operator+=(const FloatVector& vector);
   void operator-=(const FloatVector& vector);
   FloatVector operator-(const FloatVector& vector) const;
   FloatVector operator*(const float& f) const;
   friend FloatVector operator*(const float& f, const FloatVector& vector);
   void operator*=(float f) const;

   const unsigned int& getSize() const;

   const std::unique_ptr<float>& getVector() const;

   std::string toString() const;
};

#endif //FLOATVECTOR_H
