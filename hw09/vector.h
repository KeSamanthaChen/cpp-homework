#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

template <typename T>
class Vector {
public:
    Vector() = default;

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_t n, const T& default_val) {
        _size = n ;
        _capacity = calculate_capacity(n);
        _data = std::make_unique<T[]>(_capacity);
        std::fill(_data.get(), _data.get()+n, default_val);
    }

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<T> l) {
        _size = l.size();
        _capacity = calculate_capacity(l.size());
        _data = std::make_unique<T[]>(_capacity); 
        std::copy(l.begin(), l.end(), _data.get()); // or _data.get()
    }

    Vector(const Vector& copy) : _size(copy._size), _capacity(copy._capacity) {
        _data = std::make_unique<T[]>(copy._capacity);
        std::copy(copy._data.get(), copy._data.get()+copy._size, _data.get());
    }

    Vector(Vector&& move) : _size(move._size), _capacity(move._capacity) { // TODO:
        _data = std::move(move._data);
        move._capacity = 0;
        move._size = 0;
    }

    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(const Vector& copy) {
        if (&copy == this) return *this;
        _size = copy._size;
        if (_capacity != copy._capacity) {
            _capacity = copy._capacity;
            _data = std::make_unique<T[]>(copy._capacity);
        }
        std::copy(copy._data.get(), copy._data.get()+copy._size, _data.get());
        return *this;
    }


    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(Vector&& move) noexcept {
        if (&move == this) return *this;
        _size = std::move(move._size);
        _capacity = std::move(move._capacity);
        _data = std::move(move._data);
        move._size = 0;
        move._capacity = 0;
        return *this;
    }

    size_t size() const noexcept { return _size; }

    size_t capacity() const noexcept { return _capacity; }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const T& value) {
        if (_capacity > _size) { // capa 4, size 3
            _data[_size] = value; // size 2, 0, 1, 2
            _size++; // new size now
        }
        if (_capacity == _size) resize(calculate_capacity(_size+1)); // capacity == size == 0
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value) {
        if (_capacity > _size) {
            _data[_size] = std::move(value);
            _size++;
        }
        if (_capacity == _size) resize(calculate_capacity(_size+1));
    }

    /**
     * Removes the last element of the vector.
     */
    void pop_back() {
        // _data[_size-1] = nullptr;
        _size--;
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) const {
        if (pos >= _size) {
            throw std::out_of_range{"access out of range"};
        }
        return *(_data.get()+pos); // need const or not?
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range{"access out of range"};
        }
        return *(_data.get()+pos);
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) const {
        return *(_data.get()+index);
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) {
        return *(_data.get()+index);
    }

    friend std::ostream& operator<<(std::ostream& o, Vector<T> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_t i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_t growth_factor = 2;
    size_t _size;
    size_t _capacity;

    // Holds vector's data.
    std::unique_ptr<T[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_t calculate_capacity(size_t new_size) {
        if (_capacity == 0) return new_size;
        if (_capacity <= new_size) _capacity *= growth_factor;
        if (_capacity > new_size) return _capacity;
    } // the capacity must be bigger than size

    /**
    * Resizes the vector to new_capacity.
    * If the current capacity is less than new_capacity
    * the vector moves all elements to a new array.
    */
    void resize(size_t new_capacity) {
        if (_capacity < new_capacity) {
            std::unique_ptr<T[]> _new_data = std::make_unique<T[]>(new_capacity);
            std::copy(_data.get(), _data.get()+_size, _new_data.get());
            _data = std::move(_new_data); // original _data is deleted
            _capacity = new_capacity;
            // size remains the same
        }
    }
};

