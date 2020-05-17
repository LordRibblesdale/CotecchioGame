#include "Color.h"

Color::Color() : FloatVector(3, {}) {}

Color::Color(float r, float g, float b) : FloatVector(3, {r, g, b}) {}

Color::Color(float &r, float& g, float& b) : FloatVector(3, {r, g, b}) {}

Color::Color(const Color &color) : FloatVector(3, {color.getRed(), color.getGreen(), color.getBlue()}) {}

Color::Color(Color &&color) : FloatVector(3, {color.getRed(), color.getGreen(), color.getBlue()}) {
   color.setRed(0);
   color.setGreen(0);
   color.setBlue(0);
}

Color::~Color() {
   setRed(0);
   setGreen(0);
   setBlue(0);
}

Color &Color::operator=(const Color &color) {
   setRed(color.getRed());
   setGreen(color.getGreen());
   setBlue(color.getBlue());

   return *this;
}

Color &Color::operator=(Color &&color) {
   setRed(color.getRed());
   setGreen(color.getGreen());
   setBlue(color.getBlue());

   color.setRed(0);
   color.setGreen(0);
   color.setBlue(0);

   return *this;
}


const float &Color::getRed() const {
   return vector_.get()[0];
}

const float &Color::getGreen() const {
   return vector_.get()[1];
}

const float &Color::getBlue() const {
   return vector_.get()[2];
}

void Color::setRed(const float &red) {
   vector_.get()[0] = red;
}

void Color::setGreen(const float &green) {
   vector_.get()[1] = green;
}

void Color::setBlue(const float &blue) {
   vector_.get()[2] = blue;
}

void Color::clamp() {

}

int Color::toInt8Bit(const float &f) {
   return 255*f;
}
