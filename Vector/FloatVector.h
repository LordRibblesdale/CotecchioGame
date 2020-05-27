#ifndef FLOATVECTOR_H
#define FLOATVECTOR_H

#include <memory>
#include "../Exception/ExceptionNotifier.h"

struct FloatVector {
   std::unique_ptr<float> vector_;
   unsigned int size_;

public:
   FloatVector(unsigned int size, const std::initializer_list<float>& data) {
      vector_ = move(std::unique_ptr<float>(new float[size] {0}));
      FloatVector::size_ = size;

      auto iterator = data.begin();
      for (int i = 0; i < size && i < data.size(); ++i) {
         vector_.get()[i] = *iterator;
         ++iterator;
      }
   }

   FloatVector(const FloatVector& floatVector) {
      vector_ = std::unique_ptr<float>(new float[floatVector.getSize()]);
      FloatVector::size_ = floatVector.getSize();

      for (int i = 0; i < size_; ++i) {
         vector_.get()[i] = floatVector.getVector().get()[i];
      }
   }

   FloatVector(FloatVector&& floatVector) {
      //TODO test const_cast here
      //USED in: Matrix::multiplyVector()
      vector_ = move(std::unique_ptr<float>((const_cast<std::unique_ptr<float>&>(floatVector.getVector())).release()));
      size_ = floatVector.getSize();
   }

   ~FloatVector() {
      vector_.reset();
      size_ = 0;
   }

   //TODO add all operators and functions

   FloatVector& operator=(const FloatVector& vector) {
      vector_ = std::unique_ptr<float>(new float[vector.getSize()]);
      FloatVector::size_ = vector.getSize();

      for (int i = 0; i < size_; ++i) {
         vector_.get()[i] = vector.getVector().get()[i];
      }

      return *this;
   }

   FloatVector& operator=(FloatVector&& vector) {
      //TODO test const_cast here
      //USED in: Matrix::multiplyVector()
      vector_ = move(std::unique_ptr<float>((const_cast<std::unique_ptr<float>&>(vector.getVector())).release()));
      size_ = vector.getSize();

      return *this;
   }

   FloatVector operator+(const FloatVector& vector) {
      if (size_ == vector.getSize()) {
         for (int i = 0; i < size_; ++i) {
            vector_.get()[i] += vector.getVector().get()[i];
         }
      } else {
         std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
         s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

         throw (ExceptionNotifier(std::to_string(size_).c_str()));
      }
   }

   FloatVector operator-(const FloatVector& vector) {
      if (size_ == vector.getSize()) {
         for (int i = 0; i < size_; ++i) {
            vector_.get()[i] -= vector.getVector().get()[i];
         }
      } else {
         std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
         s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

         throw (ExceptionNotifier(std::to_string(size_).c_str()));
      }
   }

   const unsigned int& getSize() const {
      return size_;
   }

   const std::unique_ptr<float>& getVector() const {
      return vector_;
   }

   std::string toString() const {
      std::string s;

      for (auto i = 0; i < getSize(); ++i) {
         s.append("[").append(std::to_string(getVector().get()[i])).append("]\n");
      }

      return s;
   }
};

#endif //FLOATVECTOR_H
