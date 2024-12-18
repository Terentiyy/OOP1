#include "Seven.h"

#include <stdexcept>

// public
Seven::Seven() : len(0), array{new unsigned char{}} {}

Seven::Seven(unsigned int decimal_value) {
    unsigned char temp = decimal_value;

    for (len = 0; temp > 0; len++) {
        temp /= 7;
    }

    array = new unsigned char[len]{};

    for (int i = 0; i < len && decimal_value > 0; i++) {
        array[i] = decimal_value % 7;
        decimal_value /= 7;
    }
}

Seven::Seven(const Seven& obj) noexcept {
    len = obj.len;
    array = new unsigned char[len]{};

    for (int i = 0; i < len; i++) {
        array[i] = obj.array[i];
    }
}

Seven Seven::add(const Seven& obj) const {
    unsigned int res_len = std::max(len, obj.len);
    Seven res{0, res_len + 1};

    unsigned char digit = 0;

    int i;
    for (i = 0; i < std::min(len, obj.len); i++) {
        res.array[i] = (array[i] + obj.array[i] + digit) % 7;
        digit = (array[i] + obj.array[i] + digit) / 7;
    }

    res.array[i + 1] = digit;

    res.strip();

    return res;
}

Seven Seven::substract(const Seven& obj) const {
    if (less_than(obj)) {
        throw std::underflow_error("Answer must not have negative value!");
    }

    unsigned int res_len = std::max(len, obj.len);
    Seven res{0, res_len};

    bool digit = 0;

    int i;
    for (i = 0; i < std::min(len, obj.len); i++) {
        if (array[i] >= obj.array[i] + digit) {
            res.array[i] = array[i] - obj.array[i] - digit;
            digit = 0;
        } else {
            if (i == len - 1) {
                throw std::underflow_error(
                    "Answer must not have negative value!");
            }

            res.array[i] = array[i] + 7 - obj.array[i] - digit;
            digit = 1;
        }
    }

    if (digit == 1) {
        if (res.array[i + 1] >= digit) {
            res.array[i + 1] -= digit;
        } else {
            throw std::underflow_error("Answer must not have negative value!");
        }
    }

    res.strip();

    return res;
}

bool Seven::equals(const Seven& obj) const {
    if (len != obj.len) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        if (array[i] != obj.array[i]) {
            return false;
        }
    }

    return true;
}

bool Seven::greater_than(const Seven& obj) const {
    if (len > obj.len) {
        return true;
    }

    if (len == obj.len) {
        for (int i = len - 1; i > -1; i--) {
            if (array[i] > obj.array[i]) {
                return true;
            }
        }
    }

    return false;
}

bool Seven::less_than(const Seven& obj) const {
    return !greater_than(obj) && !equals(obj);
}

void Seven::copy(const Seven& obj) {
    if (this == &obj) {
        return;
    }

    len = obj.len;

    delete[] array;
    array = new unsigned char[len]{};

    for (int i = 0; i < len; i++) {
        array[i] = obj.array[i];
    }

    return;
}

void Seven::print() const {
    std::cout << "len: " << len << " array: ";
    for (int i = len - 1; i > -1; i--) {
        std::cout << static_cast<int>(array[i]);
    }

    std::cout << std::endl;

    return;
}

Seven::~Seven() noexcept {
    len = 0;
    delete[] array;
    array = nullptr;
}

// private
Seven::Seven(unsigned int decimal_value, unsigned int capacity) {
    len = capacity;
    array = new unsigned char[len]{};

    for (int i = 0; i < len && decimal_value > 0; i++) {
        array[i] = decimal_value % 7;
        decimal_value /= 7;
    }

    if (decimal_value > 0) {
        throw std::out_of_range("Buffer overflow!");
    }
}

void Seven::strip() {
    for (len; array[len - 1] == 0; len--);

    unsigned char* new_array{new unsigned char[len]{}};

    for (int i = 0; i < len; i++) {
        new_array[i] = array[i];
    }

    delete[] array;
    array = new unsigned char[len]{};

    for (int i = 0; i < len; i++) {
        array[i] = new_array[i];
    }

    delete[] new_array;
    new_array = nullptr;

    return;
}