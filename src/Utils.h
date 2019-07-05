// Copyright 2019 Cosmin-Razvan Vancea

#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace utils {

// la tema 1 era interzisa utilizarea std::less, deci nu ma risc
template <class T>
struct less {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

// alternativa pentru std::hash<std::string>
// djb2: http://www.cse.yorku.ca/~oz/hash.html
struct string_hash {
    size_t operator()(const std::string& str) const {
        size_t hash = 5381;

        for (auto& c : str) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }
};
}  // namespace utils

#endif  // UTILS_H_
