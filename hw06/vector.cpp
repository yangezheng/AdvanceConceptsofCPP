#include "vector.h"

#include <iterator>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <numeric>



namespace linalg {


/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
Vector::Vector(std::size_t n, float val) : data_(n, val) {}

Vector::Vector(std::initializer_list<float> list) : data_(list) {}

auto Vector::operator=(float val) -> Vector & {
    for (auto& element : data_) {
        element = val;
    }
    return *this;
}


auto Vector::assign(float val) -> void {
    for (auto& element : data_) {
        element = val;
    }
}

auto Vector::assign(Vector v) -> void {
    data_ = v.data_;
}

auto Vector::size() const -> std::size_t {
    return data_.size();
}

auto Vector::begin() -> iterator {
    return data_.begin();
}

auto Vector::end() -> iterator {
    return data_.end();
}

auto Vector::begin() const -> const_iterator {
    return data_.cbegin();
}

auto Vector::end() const -> const_iterator {
        return data_.cend();
}

auto Vector::cbegin() const -> const_iterator {
    return data_.cbegin();
}

auto Vector::cend() const -> const_iterator {
    return data_.cend();
}

auto Vector::operator[](int idx) -> float & {
    if (idx < 0) {
        idx += data_.size();
    }
    return data_[idx];
}

auto Vector::operator[](int idx) const -> const float & {
    if (idx < 0) {
        idx += data_.size();
    }
    return data_[idx];
}

auto Vector::coeff(int idx) -> float & {
    if (idx < 0 || static_cast<std::size_t>(idx) >= data_.size()) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[idx];
}

auto Vector::coeff(int idx) const -> const float & {
    if (idx < 0 || static_cast<std::size_t>(idx) >= data_.size()) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[idx];
}

auto Vector::operator+=(float val) -> Vector & {
    for (auto& element : data_) {
        element += val;
    }
    return *this;
}

auto Vector::operator-=(float val) -> Vector & {
    for (auto& element : data_) {
        element -= val;
    }
    return *this;
}

auto Vector::operator*=(float val) -> Vector & {
    for (auto& element : data_) {
        element *= val;
    }
    return *this;
}

auto Vector::operator/=(float val) -> Vector & {
    for (auto& element : data_) {
        element /= val;
    }
    return *this;
}

auto Vector::operator+=(const Vector &y) -> Vector & {
    if (size() != y.size()) {
        throw std::invalid_argument("Vectors must be of the same size for in-place addition");
    }

    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] += y.data_[i];
    }

    return *this;
}

auto Vector::operator-=(const Vector &y) -> Vector & {
    if (size() != y.size()) {
        throw std::invalid_argument("Vectors must be of the same size for in-place subtraction");
    }

    for (std::size_t i = 0; i < size(); ++i) {
        data_[i] -= y.data_[i];
    }

    return *this;
}


auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

// Standalone function to find the minimun value in a Vector
auto min(const Vector &x) -> float {
    if (x.size() == 0) {
        throw std::invalid_argument("Cannot find minimum value in an empty vector");
    }
    return *std::min_element(x.begin(), x.end());
}

// Standalone function to find the maximum value in a Vector
auto max(const Vector &x) -> float {
    if (x.size() == 0) {
        throw std::invalid_argument("Cannot find maximum value in an empty vector");
    }
    return *std::max_element(x.begin(), x.end());
}

// Standalone function to find the index of the minimum value in a Vector
auto argmin(const Vector &x) -> std::size_t {
    if (x.size() == 0) {
        throw std::invalid_argument("Cannot find argmin in an empty vector");
    }
    return std::distance(x.begin(), std::min_element(x.begin(), x.end()));
}

// Standalone function to find the index of the maximum value in a Vector
auto argmax(const Vector &x) -> std::size_t {
    if (x.size() == 0) {
        throw std::invalid_argument("Cannot find argmax in an empty vector");
    }
    return std::distance(x.begin(), std::max_element(x.begin(), x.end()));
}

// Standalone function to count the number of non-zero elements in a Vector
auto non_zeros(const Vector &x) -> std::size_t {
    return std::count_if(x.begin(), x.end(), [](float value) { return value != 0.0f; });
}

// Standalone function to find the sum of coefficients in a Vector
auto sum(const Vector &x) -> float {
    return std::accumulate(x.begin(), x.end(), 0.0f);
}

// Standalone function to find the product of coefficients in a Vector
auto prod(const Vector &x) -> float {
    if (x.size() == 0) {
        return 0.0f; // Product of an empty vector is 0
    }
    return std::accumulate(x.begin(), x.end(), 1.0f, std::multiplies<float>());
}

// Standalone function to find the dot product of two Vectors
auto dot(const Vector &x, const Vector &y) -> float {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors must be of the same size for dot product");
    }
    return std::inner_product(x.begin(), x.end(), y.begin(), 0.0f);
}

// Standalone function to find the Euclidean norm of a Vector
auto norm(const Vector &x) -> float {
    return std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), 0.0f));
}

// Standalone function to normalize the vector in-place
auto normalize(Vector &x) -> void {
    float magnitude = std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), 0.0f));
    if (magnitude == 0.0f) {
        throw std::invalid_argument("Cannot normalize a zero vector");
    }
    std::transform(x.begin(), x.end(), x.begin(), [magnitude](float value) { return value / magnitude; });
}

// Standalone function to return a normalized copy of a Vector
auto normalized(const Vector &x) -> Vector {
    Vector result = x;
    normalize(result);
    return result;
}

// Standalone function to create a vector with floored coefficients
auto floor(const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [](float value) { return std::floor(value); });
    return result;
}

// Standalone function to create a vector with ceiled coefficients
auto ceil(const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [](float value) { return std::ceil(value); });
    return result;
}

// Unary operator+: Returns a copy of x
auto operator+(const Vector &x) -> Vector {
    return x; // Return a copy
}

// Unary operator-: Returns a copy of x with negated values
auto operator-(const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), std::negate<float>());
    return result;
}

// Binary operator+: Returns a vector with coefficients as the sum of the corresponding coefficients
auto operator+(const Vector &x, const Vector &y) -> Vector {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors must be of the same size for addition");
    }
    Vector result = x;
    std::transform(result.begin(), result.end(), y.begin(), result.begin(), std::plus<float>());
    return result;
}

// Binary operator-: Returns a vector with coefficients as the subtraction of the corresponding coefficients
auto operator-(const Vector &x, const Vector &y) -> Vector {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors must be of the same size for subtraction");
    }
    Vector result = x;
    std::transform(result.begin(), result.end(), y.begin(), result.begin(), std::minus<float>());
    return result;
}

// Binary operator+: Returns a vector with coefficients as the sum of the corresponding coefficients and the scalar
auto operator+(const Vector &x, float val) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return value + val; });
    return result;
}

// Binary operator-: Returns a vector with coefficients as the subtraction of the corresponding coefficients and the scalar
auto operator-(const Vector &x, float val) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return value - val; });
    return result;
}

// Binary operator*: Returns a vector with coefficients as the multiplication of the corresponding coefficients and the scalar
auto operator*(const Vector &x, float val) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return value * val; });
    return result;
}

// Binary operator/: Returns a vector with coefficients as the division of the corresponding coefficients and the scalar
auto operator/(const Vector &x, float val) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return value / val; });
    return result;
}

// Binary operator+: Returns a vector with coefficients as the sum of the scalar and the corresponding coefficients
auto operator+(float val, const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return val + value; });
    return result;
}

// Binary operator-: Returns a vector with coefficients as the subtraction of the scalar and the corresponding coefficients
auto operator-(float val, const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return val - value; });
    return result;
}

// Binary operator*: Returns a vector with coefficients as the multiplication of the scalar and the corresponding coefficients
auto operator*(float val, const Vector &x) -> Vector {
    Vector result = x;
    std::transform(result.begin(), result.end(), result.begin(), [val](float value) { return val * value; });
    return result;
}

}

