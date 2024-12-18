#pragma once

#include <iostream>

class Seven {
   public:
    Seven();
    Seven(unsigned int decimal_value);
    Seven(const Seven& obj) noexcept;

    Seven add(const Seven& obj) const;
    Seven substract(const Seven& obj) const;

    bool equals(const Seven& obj) const;
    bool greater_than(const Seven& obj) const;
    bool less_than(const Seven& obj) const;

    void copy(const Seven& obj);
    void print() const;

    ~Seven() noexcept;

   private:
    Seven(unsigned int decimal_value, unsigned int len);
    void strip();

   private:
    unsigned int len;
    unsigned char* array;
};