#pragma once

#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <inttypes.h>
#include <string_view>
#include <string>
#include <limits>
#include "fmt/format.h"

namespace trida
{
    template<int S = 1000000>
    struct fixed_point
    {
        int64_t value;
        static constexpr int64_t scale_factor = S;

    public:
        fixed_point() {}

        fixed_point(const fixed_point & fp):value(fp.value) {}

        fixed_point(fixed_point && fp):value(fp.value) {}

        fixed_point(double number) : value(static_cast<int64_t>(number * scale_factor)) {}

        fixed_point(const std::string_view number) : value(static_cast<int64_t>(std::stod(number.data()) * scale_factor)) {}

        int64_t int_part() 
        {
            return value / scale_factor;
        }

        double to_double() const {
            return static_cast<double>(value) / scale_factor;
        }

        // Operator Overloads
        fixed_point operator+(const fixed_point& other) const {
            return fixed_point(static_cast<double>(value + other.value) / scale_factor);
        }

        fixed_point operator-(const fixed_point& other) const {
            return fixed_point(static_cast<double>(value - other.value) / scale_factor);
        }

        fixed_point operator*(const fixed_point& other) const {
            return fixed_point(static_cast<double>(value * other.value) / (scale_factor * scale_factor));
        }

        fixed_point operator/(const fixed_point& other) const {
            return fixed_point((value * scale_factor) / other.value);
        }

        fixed_point& operator=(const fixed_point& other) {
            value = other.value;
            return *this;
        }

        fixed_point& operator=(fixed_point&& other) {
            value = other.value;
            return *this;
        }

        fixed_point& operator+=(const fixed_point& other) {
            value += other.value;
            return *this;
        }

        fixed_point& operator-=(const fixed_point& other) {
            value -= other.value;
            return *this;
        }

        fixed_point& operator*=(const fixed_point& other) {
            value = (value * other.value) / scale_factor;
            return *this;
        }

        fixed_point& operator/=(const fixed_point& other) {
            value = (value * scale_factor) / other.value;
            return *this;
        }

        std::string to_string() const
        {
            return fmt::format("{:.{}f}", to_double(), 6);
        }

        // Stream Insertion Operator
        friend std::ostream& operator<<(std::ostream& os, const fixed_point& fp) {
            os << fp.to_string();
            return os;
        }

        bool operator < (const fixed_point & other) const
        {
            return value < other.value;
        }

        bool operator <= (const fixed_point & other) const
        {
            return value <= other.value;
        }

        bool operator > (const fixed_point & other) const
        {
            return value > other.value;
        }

        bool operator >= (const fixed_point & other) const
        {
            return value >= other.value;
        }

        bool operator == (const fixed_point & other) const
        {
            return value == other.value;
        }

        fixed_point& operator = (const double number)
        {
            value = static_cast<int64_t>(number * scale_factor);
            return *this;
        }

        fixed_point& operator = (const int64_t number)
        {
            value = number * scale_factor;
            return *this;
        }

        fixed_point& operator = (const int number)
        {
            value = static_cast<int64_t>(number * scale_factor);
            return *this;
        }

        fixed_point & operator = (const std::string_view number)
        {
            value = static_cast<int64_t>(std::stod(number.data()) * scale_factor);
            return *this;
        }

        static fixed_point max_limit()
        {
            return fixed_point(std::numeric_limits<int64_t>::max() / scale_factor);
        }

        static fixed_point min_limit()
        {
            return fixed_point(std::numeric_limits<int64_t>::min() / scale_factor);
        }
    };

    // Define a hash_value function for fixed_point
    template<int S>
    std::size_t hash_value(const fixed_point<S>& fp) 
    {
        return std::hash<int64_t>{}(fp.value);
    }
}

namespace std {
    template<int S>
    struct hash<trida::fixed_point<S>> 
    {
        std::size_t operator()(const trida::fixed_point<S>& fp) const noexcept 
        {
            // Simple example: hash the internal value directly
            return std::hash<int64_t>{}(fp.value);
        }
    };
}

#endif