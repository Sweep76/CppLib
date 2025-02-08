#pragma once
#include <iostream>
#include <iterator>

template<typename E>
class LinkedQueue {
private:
    struct Node {
        E elem;
        Node* next;
        Node(E elem) : elem(std::move(elem)), next(nullptr) {}
    };
    int n;
    Node* head;
    Node* tail;
public:
    LinkedQueue();
    LinkedQueue(const LinkedQueue& that);
    LinkedQueue(LinkedQueue&& that) noexcept;
    ~LinkedQueue();

    int size() const { return n; }
    bool isEmpty() const { return n == 0; }
    void enqueue(E elem);
    E dequeue();
    E front();
    E back();
    void swap(LinkedQueue& that);
    void clear();
    
    LinkedQueue& operator=(LinkedQueue that);
    template <typename T>
    friend bool operator==(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend bool operator!=(const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& queue);

    class iterator : public std::iterator<std::forward_iterator_tag, E> {
    private:
        Node* i;
    public:
        iterator() : i(nullptr) {}
        iterator(Node* x) : i(x) {}
        iterator(const iterator& that) : i(that.i) {}
        ~iterator() {}

        E& operator*() const { return i->elem; }
        bool operator==(const iterator& that) const { return i == that.i; }
        bool operator!=(const iterator& that) const { return i != that.i; }
        iterator& operator++() { i = i->next; return *this; }
        iterator operator++(int) { iterator tmp(*this); i = i->next; return tmp; }
    };
    
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(tail); }
};

template<typename E>
LinkedQueue<E>::LinkedQueue() {
    n = 0;
    head = nullptr;
    tail = nullptr;
}

template<typename E>
LinkedQueue<E>::LinkedQueue(const LinkedQueue& that) {
    n = 0;
    head = nullptr;
    tail = nullptr;
    for (Node* i = that.head; i != nullptr; i = i->next)
        enqueue(i->elem);
}

template<typename E>
LinkedQueue<E>::LinkedQueue(LinkedQueue&& that) noexcept {
    n = that.n;
    head = that.head;
    tail = that.tail;
    that.head = nullptr;
    that.tail = nullptr;
}

template<typename E>
LinkedQueue<E>::~LinkedQueue() {
    Node* aux = nullptr;
    while (head != nullptr) {
        aux = head;
        head = head->next;
        delete aux;
    }
}

template<typename E>
void LinkedQueue<E>::enqueue(E elem) {
    Node* pold = tail;
    tail = new Node(std::move(elem));
    if (isEmpty()) head = tail;
    else pold->next = tail;
    n++;
}

template<typename E>
E LinkedQueue<E>::dequeue() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");

    Node* pold = head;
    E tmp = head->elem;
    head = head->next;
    delete pold;
    n--;
    return tmp;
}

template<typename E>
E LinkedQueue<E>::front() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return head->elem;
}

template<typename E>
E LinkedQueue<E>::back() {
    if (isEmpty()) 
        throw std::out_of_range("Queue underflow.");
    return tail->elem;
}

template<typename E>
void LinkedQueue<E>::swap(LinkedQueue<E>& that) {
    using std::swap;
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
}

template<typename E>
void LinkedQueue<E>::clear() {
    Node* aux = nullptr;
    while (head != nullptr) {
        aux = head;
        head = head->next;
        delete aux;
    }
    tail = nullptr;
    n = 0;
}

template<typename E>
LinkedQueue<E>& LinkedQueue<E>::operator=(LinkedQueue<E> that) {
    swap(that);
    return *this;
}

template<typename E>
bool operator==(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs) {
    if (&lhs == &rhs) return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename E>
bool operator!=(const LinkedQueue<E>& lhs, const LinkedQueue<E>& rhs) {
    return !(lhs == rhs);
}

template<typename E>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<E>& queue) {
    using Node = typename LinkedQueue<E>::Node;
    for (Node* i = queue.head; i != nullptr; i = i->next)
        os << i->elem << " ";
    return os;
}

template<typename E>
void swap(LinkedQueue<E>& lhs, LinkedQueue<E>& rhs) {
    lhs.swap(rhs);
}
