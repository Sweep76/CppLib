#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

template<typename E>
class ArrayQueue {
private:
    static const int DEFAULT_CAPACITY = 10;

    int n;
    int head;
    int tail;
    int capacity;
    E* pq;

    void resize(int size);
public:
    explicit ArrayQueue(int cap = DEFAULT_CAPACITY);
    ArrayQueue(const ArrayQueue& that);
    ArrayQueue(ArrayQueue&& that) noexcept;
    ~ArrayQueue() { delete[] pq; }

    int size() const { return n; }
    bool isEmpty() const { return n == 0; }
    void enqueue(E elem);
    E dequeue();
    E front();
    E back();
    void swap(ArrayQueue& that);
    void clear();

    ArrayQueue& operator=(ArrayQueue that);
    template <typename T>
    friend bool operator==(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayQueue<T>& lhs, const ArrayQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayQueue<T>& queue);

    class iterator : public std::iterator<std::forward_iterator_tag, E> {
    private:
        const ArrayQueue* queue;
        int i;
    public:
        iterator() : queue(nullptr), i(0) {}
        iterator(const ArrayQueue* queue, int i) : queue(queue), i(i) {}
        iterator(const iterator& that) : queue(that.queue), i(that.i) {}
        ~iterator() {}

        E& operator*() const { return queue->pq[(queue->head + i) % queue->capacity]; }
        bool operator==(const iterator& that) const { return queue == that.queue && i == that.i; }
        bool operator!=(const iterator& that) const { return queue != that.queue || i != that.i; }
        iterator& operator++() { i++; return *this; }
        iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
    };

    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

template<typename E>
ArrayQueue<E>::ArrayQueue(int cap) {
    n = 0;
    head = 0;
    tail = 0;
    capacity = cap;
    pq = new E[capacity];
}

template<typename E>
ArrayQueue<E>::ArrayQueue(const ArrayQueue& that) {
    n = that.n;
    head = that.head;
    tail = that.tail;
    capacity = that.capacity;
    pq = new E[capacity];
    std::copy(that.pq, that.pq + n, pq);
}

template<typename E>
ArrayQueue<E>::ArrayQueue(ArrayQueue&& that) noexcept {
    n = that.n;
    head = that.head;
    tail = that.tail;
    capacity = that.capacity;
    pq = that.pq;
    that.pq = nullptr;
}

template<typename E>
void ArrayQueue<E>::resize(int size) {
    assert(size >= n);

    E* pnew = new E[size];

    for (int i = 0; i < n; ++i)
        pnew[i] = std::move(pq[(head + i) % capacity]);

    delete[] pq;
    pq = pnew;
    head = 0;
    tail = n;
    capacity = size;
}

template<typename E>
void ArrayQueue<E>::enqueue(E elem) {
    if (n == capacity) 
        resize(capacity * 2);

    pq[tail++] = std::move(elem); 
    if (tail == capacity) tail = 0;
    n++;
}

template<typename E>
E ArrayQueue<E>::dequeue() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");

    E tmp = pq[head++];
    if (head == capacity) head = 0;
    n--;

    if (n > 0 && n == capacity / 4) 
        resize(capacity / 2);

    return tmp;
}

template<typename E>
E ArrayQueue<E>::front() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return pq[head];
}

template<typename E>
E ArrayQueue<E>::back() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return pq[(tail + capacity - 1) % capacity];
}

template<typename E>
void ArrayQueue<E>::swap(ArrayQueue<E>& that) {
    using std::swap;
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
    swap(capacity, that.capacity);
    swap(pq, that.pq);
}

template<typename E>
void ArrayQueue<E>::clear() {
    n = 0;
    head = 0;
    tail = 0;
}

template<typename E>
ArrayQueue<E>& ArrayQueue<E>::operator=(ArrayQueue<E> that) {
    swap(that);
    return *this;
}

template<typename E>
bool operator==(const ArrayQueue<E>& lhs, const ArrayQueue<E>& rhs) {
    if (&lhs == &rhs) return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename E>
bool operator!=(const ArrayQueue<E>& lhs, const ArrayQueue<E>& rhs) {
    return !(lhs == rhs);
}

template<typename E>
std::ostream& operator<<(std::ostream& os, const ArrayQueue<E>& queue) {
    for (int i = 0; i < queue.n; ++i)
        os << queue.pq[(queue.head + i) % queue.capacity] << " ";
    return os;
}

template<typename E>
void swap(ArrayQueue<E>& lhs, ArrayQueue<E>& rhs) {
    lhs.swap(rhs);
}
