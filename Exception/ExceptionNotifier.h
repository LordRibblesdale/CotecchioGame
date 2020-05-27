#ifndef EXCEPTIONNOTIFIER_H
#define EXCEPTIONNOTIFIER_H

#include <exception>
#include <string>

class ExceptionNotifier : public std::exception {
private:
   const char* s_;

public:
   ExceptionNotifier(const char* s) {
      s_ = s;
   }

   const char *what() const noexcept {
      return s_;
   }
};

#endif //EXCEPTIONNOTIFIER_H
