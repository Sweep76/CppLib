#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
* Vector implemented using templates.
* Vector stored by dynamic contiguous array.
* Random access iterator for Vector implemented.
 */
template<typename E>
class Vector
{
    static const int DEFAULT_CAPACITY = 10; // Default capacity of Vector.

    // Raw pointers have all the characteristics of random access iterators.
    using iterator = E*;
    using const_iterator = const E* ;
private:
    int n; // Vector size
    int N; // Vector capacity
    E* pv; // Pointer to Vector elements

    // Expand Vector to specified capacity.
    void reserve(int count);
    // Check if index is valid.
    bool valid(int i) const { return i >= 0 && i < n; }
public:
    explicit Vector(int count = DEFAULT_CAPACITY);
    Vector(const Vector& that);
    Vector(Vector&& that) noexcept;
    ~Vector() { delete[] pv; }

    // Return the number of elements in the Vector
    int size() const { return n; }
    // Return the capacity of the Vector
    int capacity() const { return N; }
    // Check if the Vector is empty
    bool empty() const { return n == 0; }
    // Add an element to the specified position
    void insert(iterator pos, E elem);
    // Add an element to the end of the Vector
    void insert_back(E elem);
    // Remove the element at the specified position
    void remove(iterator pos);
    // Remove the last element of the Vector
    void remove_back();
    // Return a reference to the element at the specified position, with bounds checking
    E& at(int i) { return const_cast<E&>(static_cast<const Vector&>(*this).at(i)); }
    // Return a const reference to the element at the specified position, with bounds checking
    const E& at(int i) const;
    // Return a reference to the first element of the Vector
    E& front() { return const_cast<E&>(static_cast<const Vector&>(*this).front()); }
    // Return a const reference to the first element of the Vector
    const E& front() const;
    // Return a reference to the last element of the Vector
    E& back() { return const_cast<E&>(static_cast<const Vector&>(*this).back()); }
    // Return a const reference to the last element of the Vector
    const E& back() const;
    // Swap two Vector objects
    void swap(Vector& that);
    // Clear all elements in the Vector
    void clear() { n = 0; }

    //  [] operator overloading
    E& operator[](int i) { return const_cast<E&>(static_cast<const Vector&>(*this)[i]); }
    // [] operator overloading
    const E& operator[](int i) const;
    Vector& operator=(Vector that);
    Vector& operator+=(const Vector& that);
    template<typename T>
    friend Vector<T> operator+(Vector<T> lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vector);

    iterator begin() { return pv; }
    iterator end() { return pv + n; }
    const_iterator begin() const { return pv; }
    const_iterator end() const { return pv + n; }
};

/**
 * @param count: 
 */
template<typename E>
Vector<E>::Vector(int count)
{
    n = 0;
    N = count;
    pv = new E[N]();
}

/**
 * @param that:
 */
template<typename E>
Vector<E>::Vector(const Vector& that)
{
    n = that.n;
    N = that.N;
    pv = new E[N];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * @param that: 
 */
template<typename E>
Vector<E>::Vector(Vector&& that) noexcept
{
    n = that.n;
    N = that.N;
    pv = that.pv;
    that.pv = nullptr; 
}

/**
 * @param size: 
 */
template<typename E>
void Vector<E>::reserve(int count)
{
    assert(count >= size());

    Vector tmp(count);

    std::move(begin(), end(), tmp.begin());
    tmp.n = n; 
    swap(tmp); 
}

/**
 * @param i: 
 * @throws 
 */
template<typename E>
void Vector<E>::insert(iterator pos, E elem)
{
    if (i == n)
        insert_back(elem);
    else if (!valid(i))
        throw std::out_of_range("Vector::insert() i out of range.");
    else
    {
        if (n == N) reserve(N * 2);
        std::move_backward(std::next(begin(), i), end(),
                           std::next(begin(), n + 1));
        (*this)[i] = std::move(elem);
        n++;
    }
}

/**
 * @param elem: 
 */
template<typename E>
void Vector<E>::insert_back(E elem)
{
    if (n == N)
        reserve(N * 2);
    (*this)[n++] = std::move(elem);
}

/**
 * @param i: 
 * @throws 
 */
template<typename E>
void Vector<E>::remove(iterator pos)
{
    if (i == n - 1)
        return remove_back();
    if (!valid(i))
        throw std::out_of_range("Vector::remove() i out of range.");
    std::move(std::next(begin(), i + 1), end(),
              std::next(begin(), i));
    n--;
    if (n > 0 && n == N / 4)
        reserve(N / 2);
}

/**
 * @throws 
 */
template<typename E>
void Vector<E>::remove_back()
{
    if (empty())
        throw std::out_of_range("Vector::remove_back");

    if (n > 0 && n == N / 4)
        reserve(N / 2);
}

/**
 * @return 
 * @throws 
 */
template<typename E>
const E& Vector<E>::front() const
{
    if (empty())
        throw std::out_of_range("Vector::front");
    return *begin();
}

/**
 * @return 
 * @throws 
 */
template<typename E>
const E& Vector<E>::back() const
{
    if (empty())
        throw std::out_of_range("Vector::back");
    return *std::prev(end());
}

/**
 
 * @return 
 * @throws 
 */
template<typename E>
const E& Vector<E>::at(int i) const
{
    if (!valid(i))
        throw std::out_of_range("Vector::at");
    return (*this)[i];
}

/**

 * @param that: 
 */
template<typename E>
void Vector<E>::swap(Vector<E>& that)
{
    using std::swap;
    swap(n, that.n);
    swap(N, that.N);
    swap(pv, that.pv);
}

/**
 
 * @param i: 
 * @return 
 */
template<typename E>
const E& Vector<E>::operator[](int i) const
{
    return *std::next(begin(), i);
}

/**
 * @param that: 
 * @return
 */
template<typename E>
Vector<E>& Vector<E>::operator=(Vector<E> that)
{
    swap(that);
    return *this;
}

/**
 * @param that: 
 * @return 
 */
template<typename E>
Vector<E>& Vector<E>::operator+=(const Vector<E>& that)
{
    reserve(N + that.N);
    std::copy(that.begin(), that.end(), end());
    n += that.n;
    return *this;
}

/**
 * @param lhs: 
 *        
 * @return 
 */
template<typename E>
Vector<E> operator+(Vector<E> lhs, const Vector<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * @param lhs: 
 * @return 
 *         
 */
template<typename E>
bool operator==(const Vector<E>& lhs, const Vector<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**

 * @param lhs: 
 *        
 * @return 
 *         
 */
template<typename E>
bool operator!=(const Vector<E>& lhs, const Vector<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 
 * @param os:
 *        
 * @return 
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const Vector<E>& vector)
{
    for (auto i : vector)
        os << i << " ";
    return os;
}

/**
 * @param lhs: 
 *        
 */
template<typename E>
void swap(Vector<E>& lhs, Vector<E>& rhs)
{
    lhs.swap(rhs);
}