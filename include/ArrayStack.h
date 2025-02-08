#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

template<typename E>
class ArrayStack {
private:
    static const int DEFAULT_CAPACITY = 10;

    int n;
    int capacity;
    E* ps;

    void resize(int size);
public:
    explicit ArrayStack(int cap = DEFAULT_CAPACITY);
    ArrayStack(const ArrayStack& that);
    ArrayStack(ArrayStack&& that) noexcept;
    ~ArrayStack() { delete[] ps; }

    int size() const { return n; }
    bool isEmpty() const { return n == 0; }
    void push(E elem);
    E pop();
    E top();
    void swap(ArrayStack& that);
    void clear() { n = 0; }

    ArrayStack& operator=(ArrayStack that);
    template <typename T>
    friend bool operator==(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayStack<T>& stack);

    class iterator : public std::iterator<std::forward_iterator_tag, E> {
    private:
        const ArrayStack* stack;
        int i;
    public:
        iterator() : stack(nullptr), i(0) {}
        iterator(const ArrayStack* stack, int i) : stack(stack), i(i) {}
        iterator(const iterator& that) : stack(that.stack), i(that.i) {}
        ~iterator() {}

        E& operator*() const { return stack->ps[i]; }
        bool operator==(const iterator& that) const { return stack == that.stack && i == that.i; }
        bool operator!=(const iterator& that) const { return stack != that.stack || i != that.i; }
        iterator& operator++() { i++; return *this; }
        iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
    };

    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

template<typename E>
ArrayStack<E>::ArrayStack(int cap) {
    n = 0;
    capacity = cap;
    ps = new E[capacity];
}

template<typename E>
ArrayStack<E>::ArrayStack(const ArrayStack& that) {
    n = that.n;
    capacity = that.capacity;
    ps = new E[capacity];
    for (int i = 0; i < n; ++i)
        ps[i] = that.ps[i];
}

template<typename E>
ArrayStack<E>::ArrayStack(ArrayStack&& that) noexcept {
    n = that.n;
    capacity = that.capacity;
    ps = that.ps;
    that.ps = nullptr;
}

template<typename E>
void ArrayStack<E>::resize(int size) {
    assert(size >= n);
    E* pnew = new E[size];
    std::move(ps, ps + n, pnew);
    delete[] ps;
    ps = pnew;
    capacity = size;
}

template<typename E>
void ArrayStack<E>::push(E elem) {
    if (n == capacity) 
        resize(capacity * 2);
    ps[n++] = std::move(elem);
}

template<typename E>
E ArrayStack<E>::pop() {
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");
    E tmp = ps[--n];
    if (n > 0 && n == capacity / 4) 
        resize(capacity / 2);
    return tmp;
}

template<typename E>
E ArrayStack<E>::top() {
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");
    return ps[n - 1];
}

template<typename E>
void ArrayStack<E>::swap(ArrayStack<E>& that) {
    using std::swap;
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(ps, that.ps);
}

template<typename E>
ArrayStack<E>& ArrayStack<E>::operator=(ArrayStack that) {
    swap(that);
    return *this;
}

template<typename E>
bool operator==(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs) {
    if (&lhs == &rhs) return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename E>
bool operator!=(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs) {
    return !(lhs == rhs);
}

template<typename E>
std::ostream& operator<<(std::ostream& os, const ArrayStack<E>& stack) {
    for (int i = 0; i < stack.n; ++i)
        os << stack.ps[i] << " ";
    return os;
}

template<typename E>
void swap(ArrayStack<E>& lhs, ArrayStack<E>& rhs) {
    lhs.swap(rhs);
}
