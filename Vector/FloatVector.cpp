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
   FloatVector::size_ = floatVector.getSize();
   vector_.reset(new float[floatVector.getSize()] {0});

   std::copy(floatVector.getVector().get(), floatVector.getVector().get() + size_, vector_.get());
}

FloatVector::FloatVector(FloatVector &&floatVector) {
   vector_ = std::move(floatVector.vector_);
   size_ = floatVector.getSize();
}

FloatVector::~FloatVector() {
   vector_.reset();
   size_ = 0;
}

FloatVector &FloatVector::operator=(const FloatVector &vector) {
   FloatVector::size_ = vector.getSize();
   vector_.reset(new float[vector.getSize()] {0});

   std::copy(vector.getVector().get(), vector.getVector().get() + size_, vector_.get());

   return *this;
}

FloatVector &FloatVector::operator=(FloatVector &&vector) {
   vector_ = std::move(vector.vector_);
   size_ = vector.getSize();

   return *this;
}

bool FloatVector::operator==(const FloatVector &vector) const {
   if (size_ == vector.getSize()) {
      for (unsigned int i = 0; i < vector.getSize(); ++i) {
         if (vector.vector_.get()[i] != vector.getVector().get()[i]) {
            return false;
         }
      }

      return true;
   }

   return false;
}

bool FloatVector::operator!=(const FloatVector &vector) const {
   if (size_ == vector.getSize()) {
      for (unsigned int i = 0; i < vector.getSize(); ++i) {
         if (vector.vector_.get()[i] == vector.getVector().get()[i]) {
            return false;
         }
      }

      return true;
   }

   return true;
}

FloatVector FloatVector::operator+(const FloatVector &vector) const {
   if (size_ == vector.getSize()) {
      FloatVector newData(size_, {});

      for (int i = 0; i < size_; ++i) {
         newData.getVector().get()[i] = vector_.get()[i] + vector.getVector().get()[i];
      }

      return FloatVector(std::move(newData));

   } else {
      std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
      s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

      throw (ExceptionNotifier(std::to_string(size_).c_str()));
   }
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

FloatVector FloatVector::operator-(const FloatVector &vector) const {
   if (size_ == vector.getSize()) {
      FloatVector newData(size_, {});

      for (int i = 0; i < size_; ++i) {
         newData.getVector().get()[i] = vector_.get()[i] - vector.getVector().get()[i];
      }

      return FloatVector(std::move(newData));

   } else {
      std::string s = "Exception VECTOR_SUM: dimensions do not correspond. ";
      s.append("(").append(std::to_string(size_)).append(", ").append(std::to_string(vector.getSize())).append(")\n");

      throw (ExceptionNotifier(std::to_string(size_).c_str()));
   }
}

FloatVector FloatVector::operator*(const float& f) const {
   FloatVector newData(size_, {});

   for (int i = 0; i < size_; ++i) {
      newData.getVector().get()[i] = vector_.get()[i] * f;
   }

   return FloatVector(std::move(newData));
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
