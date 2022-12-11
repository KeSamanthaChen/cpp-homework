#include "vector.h"

#include <iterator>
#include <numeric>
#include <cmath>


namespace linalg {
Vector::Vector(std::size_t n) : data_(n) {}

Vector::Vector(std::size_t n, float val) : data_(n, val) {}

Vector::Vector(std::initializer_list<float> list) : data_(list) {}

auto Vector::operator=(float val) -> Vector & { 
    Vector::assign(val);
    return *this;
}

auto Vector::assign(float val) -> void {
    std::fill(data_.begin(), data_.end(), val);
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
    return data_.begin();
}

auto Vector::end() const -> const_iterator {
    return data_.end();
}

auto Vector::cbegin() const -> const_iterator {
    return data_.begin();
}

auto Vector::cend() const -> const_iterator {
    return data_.end();
}

auto Vector::operator[](int idx) -> float & {
    if (idx < 0) {
        return data_[data_.size() + static_cast<unsigned long>(idx)];
    }
    return data_[static_cast<unsigned long>(idx)];
}

auto Vector::operator[](int idx) const -> const float & {
    if (idx < 0) {
        return data_[data_.size() + static_cast<unsigned long>(idx)];
    }
    return data_[static_cast<unsigned long>(idx)];
}

auto Vector::coeff(int idx) -> float & {
    if (idx < -1 || static_cast<unsigned long>(idx) >= data_.size()) {
        throw std::out_of_range{"Index out of range for coeff"};
    }
    return data_[static_cast<unsigned long>(idx)];
}

auto Vector::coeff(int idx) const -> const float & {
    if (idx < -1 || static_cast<unsigned long>(idx) >= data_.size()) {
        throw std::out_of_range{"Index out of range for coeff const"};
    }
    return data_[static_cast<unsigned long>(idx)];
}

auto Vector::operator+=(float val) -> Vector & {
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] += val;
    return *this;
}

auto Vector::operator-=(float val) -> Vector & {
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] -= val;
    return *this;
}

auto Vector::operator*=(float val) -> Vector & {
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] *= val;
    return *this;
}

auto Vector::operator/=(float val) -> Vector & {
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] /= val;
    return *this;
}

auto Vector::operator+=(const Vector &y) -> Vector & {
    if (data_.size() != y.data_.size())
        throw std::invalid_argument{"Vector size not equal"};        
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] += y.data_[i];
    return *this;
}

auto Vector::operator-=(const Vector &y) -> Vector & {
    if (data_.size() != y.data_.size())
        throw std::invalid_argument{"Vector size not equal"};
    for (unsigned long i = 0; i < data_.size(); i++)
        data_[i] -= y.data_[i];
    return *this;
}

auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

auto min(const Vector &x) -> float {
    if (x.size() ==  0)
        throw std::invalid_argument{"vector empty"};
    return *std::min_element(x.begin(), x.end());
}

auto max(const Vector &x) -> float {
    if (x.size() ==  0)
        throw std::invalid_argument{"vector empty"};
    return *std::max_element(x.begin(), x.end());
}

auto argmin(const Vector &x) -> std::size_t {
    if (x.size() ==  0)
        throw std::invalid_argument{"vector empty"};
    return std::min_element(x.begin(), x.end()) - x.begin();
}

auto argmax(const Vector &x) -> std::size_t {
    if (x.size() ==  0)
        throw std::invalid_argument{"vector empty"};
    return std::max_element(x.begin(), x.end()) - x.begin();
}

auto non_zeros(const Vector &x) -> std::size_t {
    return x.size() - std::count(x.begin(), x.end(), 0);
}

auto sum(const Vector &x) -> float {
    // return std::reduce(x.begin(), x.end());
    return std::accumulate(x.begin(), x.end(), 0.0f);
}

auto prod(const Vector &x) -> float {
    return std::accumulate(x.begin(), x.end(), 1, std::multiplies<float>());
}

auto dot(const Vector &x, const Vector &y) -> float {
    if (x.size() != y.size())
        throw std::invalid_argument{"Vector size not equal"};
    Vector new_vec(x.size());
    for (int i=0; i < static_cast<int>(x.size()); i++)
        new_vec[i] = x[i]*y[i];
    return sum(new_vec);
}

auto norm(const Vector &x) -> float {
    float result = 0.0f;
    for (int i=0; i < static_cast<int>(x.size()); i++) {
        result += x[i]*x[i];
    }
    return std::sqrt(result);
}

auto normalize(Vector &x) -> void {
    x /= norm(x);
}

auto normalized(const Vector &x) -> Vector {
    Vector new_vec = x;
    new_vec /= norm(new_vec);
    return new_vec;
}

auto floor(const Vector &x) -> Vector {
    Vector new_vec = x;
    for (int i=0; i < static_cast<int>(new_vec.size()); i++) {
        new_vec[i] = std::floor(new_vec[i]);
    }
    return new_vec;
}

auto ceil(const Vector &x) -> Vector {
    Vector new_vec = x;
    for (int i=0; i<static_cast<int>(new_vec.size()); i++) {
        new_vec[i] = std::ceil(new_vec[i]);
    }
    return new_vec;
}

auto operator+(const Vector &x) -> Vector {
    Vector new_vec = x;
    return new_vec;
}

auto operator-(const Vector &x) -> Vector {
    Vector new_vec = x;
    new_vec *= -1;
    return new_vec;
}

auto operator+(const Vector &x, const Vector &y) -> Vector {
    Vector new_vec = x;
    new_vec += y;
    return new_vec;
}

auto operator-(const Vector &x, const Vector &y) -> Vector {
    Vector new_vec = x;
    new_vec -= y;
    return new_vec;
}

auto operator+(const Vector &x, float val) -> Vector {
    Vector new_vec = x;
    new_vec += val;
    return new_vec;
}

auto operator-(const Vector &x, float val) -> Vector {
    Vector new_vec = x;
    new_vec -= val;
    return new_vec;
}

auto operator*(const Vector &x, float val) -> Vector {
    Vector new_vec = x;
    new_vec *= val;
    return new_vec;
}

auto operator/(const Vector &x, float val) -> Vector {
    Vector new_vec = x;
    new_vec /= val;
    return new_vec;
}

auto operator+(float val, const Vector &x) -> Vector {
    Vector new_vec = x;
    new_vec += val;
    return new_vec;   
}

auto operator-(float val, const Vector &x) -> Vector {
    Vector new_vec = x;
    new_vec -= val;
    return -new_vec;
}

auto operator*(float val, const Vector &x) -> Vector {
    Vector new_vec = x;
    new_vec *= val;
    return new_vec;
}
}

/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
