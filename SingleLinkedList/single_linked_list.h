#pragma once
#include <algorithm>
#include <cassert>


template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }

        Type value{};
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {
        }

    public:

        using iterator_category = std::forward_iterator_tag;

        using value_type = Type;

        using difference_type = std::ptrdiff_t;

        using pointer = ValueType*;

        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept :node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }


        BasicIterator& operator++() noexcept {
            assert(&(this->node_) != nullptr);
            this->node_ = (*this->node_).next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old = *this;
            this->operator++();
            return old;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(&(this->node_) != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(&(this->node_) != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;

    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {

        for (auto el : values) {
            PushBack(el);
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp;

        for (auto el : other) {
            tmp.PushBack(el);
        }

        swap(tmp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {

        if (this->head_.next_node == rhs.head_.next_node) {
            return *this;
        }

        if (rhs.IsEmpty()) {
            Clear();
            return *this;
        }

        SingleLinkedList tmp = { rhs };

        swap(tmp);

        return *this;
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return {};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return {};
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() {
        assert(!IsEmpty());

        Node* next_ = (*head_.next_node).next_node;
        delete head_.next_node;
        head_.next_node = next_;
        --size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        (*pos.node_).next_node = new Node(value, (*pos.node_).next_node);

        ++size_;

        return Iterator((*pos.node_).next_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {

        Node* node_to_del = (*pos.node_).next_node;
        assert(node_to_del != nullptr);
        Node* next_ = node_to_del->next_node;
        delete node_to_del;

        (*pos.node_).next_node = next_;

        --size_;

        return Iterator(next_);
    }


    void PushBack(const Type& value) {
        if (IsEmpty()) {
            head_.next_node = new Node(value, nullptr);
        }
        else {
            Node* current = &head_;
            while (current->next_node != nullptr) {
                current = current->next_node;
            }

            current->next_node = new Node(value, nullptr);
        }
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* ptr_in_next_node = head_.next_node;
            head_.next_node = ptr_in_next_node->next_node;
            delete ptr_in_next_node;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

private:

    Node head_;
    size_t size_ = 0;

};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (&lhs == &rhs)  // Оптимизация сравнения списка с собой
        || (lhs.GetSize() == rhs.GetSize()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin()));  // может бросить исключение
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);  // может бросить исключение
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());  // может бросить исключение
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);  // Может бросить исключение
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);  // Может бросить исключение
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);  // Может бросить исключение
}