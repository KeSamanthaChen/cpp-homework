#pragma once

#include <array>

template <typename T, typename U, typename V, typename W> struct Quadruple {
public:
    Quadruple(T first, U second, V third, W fourth) : first(first), second(second), third(third), fourth(fourth) {}

    T get_first() const { return first; }
    U get_second() const { return second; }
    V get_third() const { return third; }
    W get_fourth() const { return fourth; }

private:
    T first;
    U second;
    V third;
    W fourth;
};

template <> struct Quadruple<int, int, int, int> {
    public:
        Quadruple(int first, int second, int third, int fourth) : members{first, second, third, fourth} {}

        int get_first() const { return members[0]; }
        int get_second() const { return members[1]; }
        int get_third() const { return members[2]; }
        int get_fourth() const { return members[3]; }

        std::array<int, 4> members;
};

template <typename T> struct Quadruple<T, T, T, T> {
    public:
        Quadruple(T first, T second, T third, T fourth) : members{first, second, third, fourth} {}

        T get_first() const { return members[0]; }
        T get_second() const { return members[1]; }
        T get_third() const { return members[2]; }
        T get_fourth() const { return members[3]; }

        std::array<T, 4> members;
};
