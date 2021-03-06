/*
uint256_t.h
An unsigned 256 bit integer library for C++

Copyright (c) 2013 - 2017 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

With much help from Auston Sterling

Thanks to Fran�ois Dessenne for convincing me
to do a general rewrite of this class.
*/

#ifndef __UINT256_T__
#define __UINT256_T__

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "uint128_t.h"

class uint256_t{
    private:
        uint128_t UPPER, LOWER;

    public:
        // Constructors
        uint256_t();
        uint256_t(const uint256_t & rhs);
        uint256_t(uint256_t && rhs);

        template <typename T> uint256_t(const T & rhs)
            : UPPER(uint128_0), LOWER(rhs)
        {
            static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
        }

        template <typename S, typename T> uint256_t(const S & upper_rhs, const T & lower_rhs)
            : UPPER(upper_rhs), LOWER(lower_rhs)
        {
            static_assert(std::is_integral <S>::value &&
                          std::is_integral <T>::value
                          , "Input argument types must be integers.");
        }

        template <typename R, typename S, typename T, typename U>
        uint256_t(const R & upper_lhs, const S & lower_lhs, const T & upper_rhs, const U & lower_rhs)
            : UPPER(upper_lhs, lower_lhs), LOWER(upper_rhs, lower_rhs)
        {
            static_assert(std::is_integral <R>::value &&
                          std::is_integral <S>::value &&
                          std::is_integral <T>::value &&
                          std::is_integral <U>::value
                          , "Input argument types must be integers.");
        }

        //  RHS input args only

        // Assignment Operator
        uint256_t operator=(const uint256_t & rhs);
        uint256_t operator=(uint256_t && rhs);

        template <typename T> uint256_t operator=(const T & rhs){
            static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
            UPPER = uint128_0;
            LOWER = rhs;
            return *this;
        }

        // Typecast Operators
        operator bool      () const;
        operator uint8_t   () const;
        operator uint16_t  () const;
        operator uint32_t  () const;
        operator uint64_t  () const;
        operator uint128_t () const;

        // Bitwise Operators
        uint256_t operator&(const uint128_t & rhs) const;
        uint256_t operator&(const uint256_t & rhs) const;

        template <typename T> uint256_t operator&(const T & rhs) const{
            return uint256_t(uint128_0, LOWER & (uint128_t) rhs);
        }

        uint256_t & operator&=(const uint128_t & rhs);
        uint256_t & operator&=(const uint256_t & rhs);

        template <typename T> uint256_t & operator&=(const T & rhs){
            UPPER = uint128_0;
            LOWER &= rhs;
            return *this;
        }

        uint256_t operator|(const uint128_t & rhs) const;
        uint256_t operator|(const uint256_t & rhs) const;

        template <typename T> uint256_t operator|(const T & rhs) const{
            return uint256_t(UPPER, LOWER | uint128_t(rhs));
        }

        uint256_t & operator|=(const uint128_t & rhs);
        uint256_t & operator|=(const uint256_t & rhs);

        template <typename T> uint256_t & operator|=(const T & rhs){
            LOWER |= (uint128_t) rhs;
            return *this;
        }

        uint256_t operator^(const uint128_t & rhs) const;
        uint256_t operator^(const uint256_t & rhs) const;

        template <typename T> uint256_t operator^(const T & rhs) const{
            return uint256_t(UPPER, LOWER ^ (uint128_t) rhs);
        }

        uint256_t & operator^=(const uint128_t & rhs);
        uint256_t & operator^=(const uint256_t & rhs);

        template <typename T> uint256_t & operator^=(const T & rhs){
            LOWER ^= (uint128_t) rhs;
            return *this;
        }

        uint256_t operator~() const;

        // Bit Shift Operators
        uint256_t operator<<(const uint128_t & shift) const;
        uint256_t operator<<(const uint256_t & shift) const;

        template <typename T> uint256_t operator<<(const T & rhs) const{
            return *this << uint256_t(rhs);
        }

        uint256_t & operator<<=(const uint128_t & shift);
        uint256_t & operator<<=(const uint256_t & shift);

        template <typename T> uint256_t & operator<<=(const T & rhs){
            *this = *this << uint256_t(rhs);
            return *this;
        }

        uint256_t operator>>(const uint128_t & shift) const;
        uint256_t operator>>(const uint256_t & shift) const;

        template <typename T> uint256_t operator>>(const T & rhs) const{
            return *this >> uint256_t(rhs);
        }

        uint256_t & operator>>=(const uint128_t & shift);
        uint256_t & operator>>=(const uint256_t & shift);

        template <typename T> uint256_t & operator>>=(const T & rhs){
            *this = *this >> uint256_t(rhs);
            return *this;
        }

        // Logical Operators
        bool operator!() const;

        bool operator&&(const uint128_t & rhs) const;
        bool operator&&(const uint256_t & rhs) const;

        template <typename T> bool operator&&(const T & rhs) const{
            return ((bool) *this && rhs);
        }

        bool operator||(const uint128_t & rhs) const;
        bool operator||(const uint256_t & rhs) const;

        template <typename T> bool operator||(const T & rhs) const{
            return ((bool) *this || rhs);
        }

        // Comparison Operators
        bool operator==(const uint128_t & rhs) const;
        bool operator==(const uint256_t & rhs) const;

        template <typename T> bool operator==(const T & rhs) const{
            return (!UPPER && (LOWER == uint128_t(rhs)));
        }

        bool operator!=(const uint128_t & rhs) const;
        bool operator!=(const uint256_t & rhs) const;

        template <typename T> bool operator!=(const T & rhs) const{
            return ((bool) UPPER | (LOWER != uint128_t(rhs)));
        }

        bool operator>(const uint128_t & rhs) const;
        bool operator>(const uint256_t & rhs) const;

        template <typename T> bool operator>(const T & rhs) const{
            return ((bool) UPPER | (LOWER > uint128_t(rhs)));
        }

        bool operator<(const uint128_t & rhs) const;
        bool operator<(const uint256_t & rhs) const;

        template <typename T> bool operator<(const T & rhs) const{
            return (!UPPER)?(LOWER < uint128_t(rhs)):false;
        }

        bool operator>=(const uint128_t & rhs) const;
        bool operator>=(const uint256_t & rhs) const;

        template <typename T> bool operator>=(const T & rhs) const{
            return ((*this > rhs) | (*this == rhs));
        }

        bool operator<=(const uint128_t & rhs) const;
        bool operator<=(const uint256_t & rhs) const;

        template <typename T> bool operator<=(const T & rhs) const{
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        uint256_t operator+(const uint128_t & rhs) const;
        uint256_t operator+(const uint256_t & rhs) const;

        template <typename T> uint256_t operator+(const T & rhs) const{
            return uint256_t(UPPER + ((LOWER + (uint128_t) rhs) < LOWER), LOWER + (uint128_t) rhs);
        }

        uint256_t & operator+=(const uint128_t & rhs);
        uint256_t & operator+=(const uint256_t & rhs);

        template <typename T> uint256_t & operator+=(const T & rhs){
            UPPER = UPPER + ((LOWER + rhs) < LOWER);
            LOWER = LOWER + rhs;
            return *this;
        }

        uint256_t operator-(const uint128_t & rhs) const;
        uint256_t operator-(const uint256_t & rhs) const;

        template <typename T> uint256_t operator-(const T & rhs) const{
            return uint256_t(UPPER - ((LOWER - rhs) > LOWER), LOWER - rhs);
        }

        uint256_t & operator-=(const uint128_t & rhs);
        uint256_t & operator-=(const uint256_t & rhs);

        template <typename T> uint256_t & operator-=(const T & rhs){
            *this = *this - rhs;
            return *this;
        }

        uint256_t operator*(const uint128_t & rhs) const;
        uint256_t operator*(const uint256_t & rhs) const;

        template <typename T> uint256_t operator*(const T & rhs) const{
            return *this * uint256_t(rhs);
        }

        uint256_t & operator*=(const uint128_t & rhs);
        uint256_t & operator*=(const uint256_t & rhs);

        template <typename T> uint256_t & operator*=(const T & rhs){
            *this = *this * uint256_t(rhs);
            return *this;
        }

    private:
        std::pair <uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs) const;

    public:
        uint256_t operator/(const uint128_t & rhs) const;
        uint256_t operator/(const uint256_t & rhs) const;

        template <typename T> uint256_t operator/(const T & rhs) const{
            return *this / uint256_t(rhs);
        }

        uint256_t & operator/=(const uint128_t & rhs);
        uint256_t & operator/=(const uint256_t & rhs);

        template <typename T> uint256_t & operator/=(const T & rhs){
            *this = *this / uint256_t(rhs);
            return *this;
        }

        uint256_t operator%(const uint128_t & rhs) const;
        uint256_t operator%(const uint256_t & rhs) const;

        template <typename T> uint256_t operator%(const T & rhs) const{
            return *this % uint256_t(rhs);
        }

        uint256_t & operator%=(const uint128_t & rhs);
        uint256_t & operator%=(const uint256_t & rhs);

        template <typename T> uint256_t & operator%=(const T & rhs){
            *this = *this % uint256_t(rhs);
            return *this;
        }

        // Increment Operators
        uint256_t & operator++();
        uint256_t operator++(int);

        // Decrement Operators
        uint256_t & operator--();
        uint256_t operator--(int);

        // Nothing done since promotion doesn't work here
        uint256_t operator+() const;

        // two's complement
        uint256_t operator-() const;

        // Get private values
        const uint128_t & upper() const;
        const uint128_t & lower() const;

        // Get bitsize of value
        uint16_t bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
};

// Give uint256_t type traits
namespace std {  // This is probably not a good idea
    template <> struct is_arithmetic <uint256_t> : std::true_type {};
    template <> struct is_integral   <uint256_t> : std::true_type {};
    template <> struct is_unsigned   <uint256_t> : std::true_type {};
};

extern const uint128_t uint128_64;
extern const uint128_t uint128_128;
extern const uint128_t uint128_256;
extern const uint256_t uint256_0;
extern const uint256_t uint256_1;

// Bitwise Operators
uint256_t operator&(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator&(const T & lhs, const uint256_t & rhs){
    return rhs & lhs;
}

uint128_t & operator&=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator&=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs & lhs);
}

uint256_t operator|(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator|(const T & lhs, const uint256_t & rhs){
    return rhs | lhs;
}

uint128_t & operator|=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator|=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs | lhs);
}

uint256_t operator^(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator^(const T & lhs, const uint256_t & rhs){
    return rhs ^ lhs;
}

uint128_t & operator^=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator^=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs ^ lhs);
}

// Bitshift operators
uint256_t operator<<(const bool      & lhs, const uint256_t & rhs);
uint256_t operator<<(const uint8_t   & lhs, const uint256_t & rhs);
uint256_t operator<<(const uint16_t  & lhs, const uint256_t & rhs);
uint256_t operator<<(const uint32_t  & lhs, const uint256_t & rhs);
uint256_t operator<<(const uint64_t  & lhs, const uint256_t & rhs);
uint256_t operator<<(const uint128_t & lhs, const uint256_t & rhs);
uint256_t operator<<(const int8_t    & lhs, const uint256_t & rhs);
uint256_t operator<<(const int16_t   & lhs, const uint256_t & rhs);
uint256_t operator<<(const int32_t   & lhs, const uint256_t & rhs);
uint256_t operator<<(const int64_t   & lhs, const uint256_t & rhs);

uint128_t & operator<<=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator<<=(T & lhs, const uint256_t & rhs){
    lhs = static_cast <T> (uint256_t(lhs) << rhs);
    return lhs;
}

uint256_t operator>>(const bool      & lhs, const uint256_t & rhs);
uint256_t operator>>(const uint8_t   & lhs, const uint256_t & rhs);
uint256_t operator>>(const uint16_t  & lhs, const uint256_t & rhs);
uint256_t operator>>(const uint32_t  & lhs, const uint256_t & rhs);
uint256_t operator>>(const uint64_t  & lhs, const uint256_t & rhs);
uint256_t operator>>(const uint128_t & lhs, const uint256_t & rhs);
uint256_t operator>>(const int8_t    & lhs, const uint256_t & rhs);
uint256_t operator>>(const int16_t   & lhs, const uint256_t & rhs);
uint256_t operator>>(const int32_t   & lhs, const uint256_t & rhs);
uint256_t operator>>(const int64_t   & lhs, const uint256_t & rhs);

uint128_t & operator>>=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator>>=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) >> rhs);
}

// Comparison Operators
bool operator==(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator==(const T & lhs, const uint256_t & rhs){
    return (!rhs.upper() && ((uint64_t) lhs == rhs.lower()));
}

bool operator!=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator!=(const T & lhs, const uint256_t & rhs){
    return (rhs.upper() | ((uint64_t) lhs != rhs.lower()));
}

bool operator>(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator>(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs > rhs.lower());
}

bool operator<(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator<(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?true:((uint128_t) lhs < rhs.lower());
}

bool operator>=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator>=(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs >= rhs.lower());
}

bool operator<=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> bool operator<=(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?true:((uint128_t) lhs <= rhs.lower());
}

// Arithmetic Operators
uint256_t operator+(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator+(const T & lhs, const uint256_t & rhs){
    return rhs + lhs;
}

uint128_t & operator+=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator+=(T & lhs, const uint256_t & rhs){
    lhs = static_cast <T> (rhs + lhs);
    return lhs;
}

uint256_t operator-(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator-(const T & lhs, const uint256_t & rhs){
    return -(rhs - lhs);
}

uint128_t & operator-=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator-=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (-(rhs - lhs));
}

uint256_t operator*(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator*(const T & lhs, const uint256_t & rhs){
    return rhs * lhs;
}

uint128_t & operator*=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator*=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs * lhs);
}

uint256_t operator/(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator/(const T & lhs, const uint256_t & rhs){
    return uint256_t(lhs) / rhs;
}

uint128_t & operator/=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator/=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) / rhs);
}

uint256_t operator%(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> uint256_t operator%(const T & lhs, const uint256_t & rhs){
    return uint256_t(lhs) % rhs;
}

uint128_t & operator%=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> T & operator%=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) % rhs);
}

// IO Operator
std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs);
#endif
