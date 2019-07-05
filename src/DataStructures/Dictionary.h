// Copyright 2019 Cosmin-Razvan Vancea

#ifndef DATASTRUCTURES_DICTIONARY_H_
#define DATASTRUCTURES_DICTIONARY_H_

#include <vector>
#include <list>
#include <algorithm>

template <class Key, class T, class Hash = std::hash<Key>>
class Dictionary {
    struct Node {
        Key key;
        T value;
    };
    std::vector<std::list<Node>> m_Buckets;

    size_t m_TableSize;
    Hash m_HasherFn;
    size_t m_Values;

 public:
    Dictionary() {
        m_TableSize = 0;
        m_Values = 0;
    }

    explicit Dictionary(size_t table_size) {
        m_TableSize = 0;
        m_Values = 0;
        resize(table_size);
    }

    bool resize(size_t new_size) {
        if (m_TableSize == 0) {
            m_TableSize = new_size;
            m_Buckets.resize(new_size);
            return 1;
        }
        return 0;
    }

    void set(const Key& key, const T& value) {
        size_t bucketIdx = m_HasherFn(key) % m_TableSize;
        auto& bucket = m_Buckets[bucketIdx];

        auto node = std::find_if(std::begin(bucket), std::end(bucket),
            [key](const Node& node) {
                return (node.key == key);
            });

        if (node == std::end(bucket)) {
            bucket.push_back({ key, value });
            m_Values++;
        } else {
            node->value = value;
        }
    }

    void remove(const Key& key) {
        size_t bucketIdx = m_HasherFn(key) % m_TableSize;
        auto& bucket = m_Buckets[bucketIdx];

        auto node = std::find_if(std::begin(bucket), std::end(bucket),
            [key](const Node& node) {
                return (node.key == key);
            });

        if (node != std::end(bucket)) {
            bucket.erase(node);
            m_Values--;
        }
    }

    T get(const Key& key) {
        size_t bucketIdx = m_HasherFn(key) % m_TableSize;
        auto& bucket = m_Buckets[bucketIdx];

        auto node = std::find_if(std::begin(bucket), std::end(bucket),
            [key](const Node& node) {
                return (node.key == key);
            });

        if (node != std::end(bucket)) {
            return node->value;
        }

        return T();
    }

    bool keyExists(const Key& key) {
        size_t bucketIdx = m_HasherFn(key) % m_TableSize;
        auto& bucket = m_Buckets[bucketIdx];

        auto node = std::find_if(std::begin(bucket), std::end(bucket),
            [key](const Node& node) {
                return (node.key == key);
            });

        return (node != std::end(bucket));
    }

    // daca exista cheia in dictionar, returneaza o ref la valoarea asociata
    // daca nu exista, se adauga valoarea default si se returneaza o ref
    T& operator[] (const Key& key) {
        size_t bucketIdx = m_HasherFn(key) % m_TableSize;
        auto& bucket = m_Buckets[bucketIdx];

        auto node = std::find_if(std::begin(bucket), std::end(bucket),
            [key](const Node& node) {
                return (node.key == key);
            });

        if (node != std::end(bucket)) {
            return node->value;
        }

        bucket.push_back({ key, T() });
        m_Values++;
        return bucket.back().value;
    }
};

#endif  // DATASTRUCTURES_DICTIONARY_H_
