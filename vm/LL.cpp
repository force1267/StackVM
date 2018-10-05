#ifndef LL
#define LL


template<typename T> class LinkedList {
    public:
    
    class Node {
        public:
        Node* next;
        Node* prev;
        T data;
        
        Node(T data = 0, Node* next = 0, Node* prev = 0) {
            this->next = next;
            if(next) {
                next->prev = this;
            }
            this->prev = prev;
            if(prev) {
                prev->next = this;
            }
            this->data = data;
        }
        Node* add_next(T& data) {
            Node* next = new Node(data, this->next, this);
            return next;
        }
        Node* add_prev(T& data) {
            Node* prev = new Node(data, this, this->prev);
            return prev;
        }
        Node* remove_next(T& data) {
            Node* old = this->next;
            this->next = old->next;
            if(old) {
                if(old->next)
                    old->next->prev = this;
                data = old->data;
                delete old;
                return this;
            } else return 0;
        }
        Node* remove_prev(T& data) {
            Node* old = this->prev;
            this->prev = old->prev;
            if(old) {
                if(old->prev)
                    old->prev->next = this;
                data = old->data;
                delete old;
                return old;
            } else return 0;
        }
        Node* remove_this(T& data) {
            data = this->data;
            if(this->prev)
                this->prev->next = this->next;
            if(this->next)
                this->next->prev = this->prev;
            delete this;
            return this;
        }
    };
    class callback {
        public: virtual void fn(T, Node*) = 0;
    };
    Node *first, *last;
    LinkedList() {
        first = new Node();
        last = new Node(0, 0, first);
        first->next = last;
    }
    void push(T data) {
        last->add_prev(data);
    }
    bool pop(T& data) {
        if(last->prev == first)
            return false;
        return last->remove_prev(data);
    }
    void pull(T data) {
        first->add_next(data);
    }
    bool shift(T& data) {
        if(first->next == last)
            return false;
            
        return first->remove_next(data);
    }
    Node* find(T& data) {
        Node* now = first->next;
        while(now != last) {
            if(now->data == data)
                return now;
            now = now->next;
        }
        return 0;
    }
    Node* max() {
        Node* now = first->next;
        Node* max = first;
        while(now != last) {
            if(now->data >= max->data)
                max = now;
            now = now->next;
        }
        if(max != first)
            return max;
        return 0;
    }
    Node* min() {
        Node* now = first->next;
        Node* min = first;
        while(now != last) {
            if(now->data <= min->data)
                min = now;
            now = now->next;
        }
        if(min != first)
            return min;
        return 0;
    }
    bool isEmpty() {
        return first->next == last;
    }
    void forEach(callback* cb) {
        Node* now = first->next;
        while(now != last) {
            if(now)
                cb->fn(now->data, now);
            now = now->next;
        }
    }
};


#endif