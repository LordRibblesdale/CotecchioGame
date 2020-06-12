#include "FloatVector.h"

FloatVector::FloatVector(unsigned int size, float *data) {
   FloatVector::size_ = size;
   vector_.reset(data);
}

FloatVector::FloatVector(unsigned int size, const std::initializer_list<float> &data) {
   vector_ = std::move(std::unique_ptr<float>(new float[size] {0}));
   FloatVector::size_ = size;

   auto iterator = data.begin();
   for (int i = 0; i < size && i < data.size(); ++i) {
      vector_.get()[i] = *iterator;
      ++iterator;
   }
}

FloatVector::FloatVector(const FloatVector &floatVector) {
   vector_ = std::move(std::unique_ptr<float>(new float[floatVector.getSize()] {0}));
   FloatVector::size_ = floatVector.getSize();

   for (int i = 0; i < size_; ++i) {
      vector_.get()[i] = floatVector.getVector().get()[i];
   }
}

FloatVector::FloatVector(FloatVector &&floatVector) {
   vector_ = std::move(std::unique_ptr<float>(floatVector.vector_.release()));
   size_ = floatVector.getSize();
}

FloatVector::~FloatVector() {
   vector_.reset();
   size_ = 0;
}

FloatVector &FloatVector::operator=(const FloatVector &vector) {
   vector_ = std::move(std::unique_ptr<float>(new float[vector.getSize()] {0}));
   FloatVector::size_ = vector.getSize();

   for (int i = 0; i < size_; ++i) {
      vector_.get()[i] = vector.getVector().get()[i];
   }

   return *this;
}

FloatVector &FloatVector::operator=(FloatVector &&vector) {
   vector_ = std::move(std::unique_ptr<float>(vector.vector_.release()));
   size_ = vector.getSize();

   return *this;
}

FloatVector FloatVector::operator+(const FloatVector &vector) {
   std::unique_ptr<float> newData(new float[size_] {0});

   if (size_ == vector.getSize()) {
      for (int i = 0; i < size_; ++i) {
         newData.get()[i] = vector_.get()[i] + vector.getVector().get()[i];
      }
   } else {
      std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
      s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

      throw (ExceptionNotifier(std::to_string(size_).c_str()));
   }

   return FloatVector(size_, newData.release());
}

void FloatVector::operator+=(const FloatVector &vector) {
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

void FloatVector::operator-=(const FloatVector &vector) {
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

FloatVector FloatVector::operator-(const FloatVector &vector) {
   std::unique_ptr<float> newData(new float[size_] {0});

   if (size_ == vector.getSize()) {
      for (int i = 0; i < size_; ++i) {
         newData.get()[i] = vector_.get()[i] - vector.getVector().get()[i];
      }
   } else {
      std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
      s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

      throw (ExceptionNotifier(std::to_string(size_).c_str()));
   }

   return FloatVector(size_, newData.release());
}

FloatVector FloatVector::operator*(const float& f) const {
   std::unique_ptr<float> newData(new float[size_] {0});

   for (int i = 0; i < size_; ++i) {
      newData.get()[i] = vector_.get()[i] * f;
   }

   return FloatVector(size_, newData.release());
}

void FloatVector::operator*=(float f) const {
   for (int i = 0; i < size_; ++i) {
      vector_.get()[i] *= f;
   }
}

FloatVector operator*(const float& f, const FloatVector &vector) {
   return vector * f;
}

const unsigned int &FloatVector::getSize() const {
   return size_;
}

const std::unique_ptr<float> &FloatVector::getVector() const {
   return vector_;
}

std::string FloatVector::toString() const {
   std::string s;

   for (auto i = 0; i < getSize(); ++i) {
      s.append("[").append(std::to_string(getVector().get()[i])).append("]\n");
   }

   return s;
}
