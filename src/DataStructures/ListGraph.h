// Copyright 2019 Cosmin-Razvan Vancea

#ifndef DATASTRUCTURES_LISTGRAPH_H_
#define DATASTRUCTURES_LISTGRAPH_H_

#include <vector>
#include <list>
#include <queue>

#include "./Dictionary.h"

constexpr size_t DEFAULT_DICTIONARY_SIZE = 6661;

template <class T, class Hash = std::hash<T>>
class ListGraph {
    struct Node {
        std::list<size_t> neighbors;
        T label;
    };

    bool m_GraphInitialized;
    std::vector<Node> m_Nodes;
    Dictionary<T, size_t, Hash> m_LabelToNodeIdx;

    std::vector<std::vector<int>> m_Distances;
    bool m_DistancesPrecomputed;

 public:
    ListGraph() {
        m_GraphInitialized = false;
        m_DistancesPrecomputed = false;
    }

    explicit ListGraph(size_t nodes) {
        m_GraphInitialized = false;
        m_DistancesPrecomputed = false;

        resize(nodes);
    }

    bool resize(size_t nodes, size_t dictionarySize = DEFAULT_DICTIONARY_SIZE) {
        if (!m_GraphInitialized) {
            m_Nodes.resize(nodes);
            m_Distances.resize(nodes);
            m_LabelToNodeIdx.resize(dictionarySize);

            m_GraphInitialized = true;
            return 1;
        }
        return 0;
    }

    void setNodeLabel(size_t nodeIdx, const T &label) {
        m_LabelToNodeIdx[label] = nodeIdx;
        m_Nodes[nodeIdx].label = label;
    }

    const T& getNodeLabel(size_t nodeIdx) {
        return m_Nodes[nodeIdx].label;
    }

    size_t getNodeIdx(const T& label) {
        return m_LabelToNodeIdx[label];
    }

    bool isReachable(const T& src, const T& dst) {
        return getDistance(src, dst) != -1;
    }

    int getDistance(const T& src, const T& dst) {
        size_t srcIdx = m_LabelToNodeIdx[src];
        size_t dstIdx = m_LabelToNodeIdx[dst];

        if (m_DistancesPrecomputed) {
            return m_Distances[srcIdx][dstIdx];
        }

        std::vector<int> distance = getDistanceToAllNodes(srcIdx);
        return distance[dstIdx];
    }

    void precomputeDistances() {
        m_DistancesPrecomputed = true;

        for (size_t i = 0; i != getSize(); ++i) {
            m_Distances[i] = getDistanceToAllNodes(i);
        }
    }

    bool addEdge(const T& src, const T& dst) {
        size_t srcIdx = m_LabelToNodeIdx[src];
        size_t dstIdx = m_LabelToNodeIdx[dst];

        if (hasEdge(src, dst)) {
            return 0;
        }

        m_Nodes[srcIdx].neighbors.push_back(dstIdx);
        m_DistancesPrecomputed = false;
        return 1;
    }

    bool removeEdge(const T& src, const T& dst) {
        size_t srcIdx = m_LabelToNodeIdx[src];
        size_t dstIdx = m_LabelToNodeIdx[dst];

        if (!hasEdge(src, dst)) {
            return 0;
        }

        m_Nodes[srcIdx].neighbors.erase(
            std::remove(
                std::begin(m_Nodes[srcIdx].neighbors),
                std::end(m_Nodes[srcIdx].neighbors),
                dstIdx),
            std::end(m_Nodes[srcIdx].neighbors));

        m_DistancesPrecomputed = false;
        return 1;
    }

    bool hasEdge(const T& src, const T& dst) {
        size_t srcIdx = m_LabelToNodeIdx[src];
        size_t dstIdx = m_LabelToNodeIdx[dst];

        auto result = std::find(
            std::begin(m_Nodes[srcIdx].neighbors),
            std::end(m_Nodes[srcIdx].neighbors),
            dstIdx);

        return result != std::end(m_Nodes[srcIdx].neighbors);
    }

    std::list<T> getNeighbors(const T& node) {
        size_t nodeIdx = m_LabelToNodeIdx[node];
        const std::list<size_t>& neighborsIdx = getNeighbors(nodeIdx);

        std::list<T> neighbors;
        for (auto& n : neighborsIdx) {
            neighbors.push_back(m_Nodes[n].label);
        }

        return neighbors;
    }

    size_t getSize() {
        return m_Nodes.size();
    }

 private:
    const std::list<size_t>& getNeighbors(size_t nodeIdx) {
        return m_Nodes[nodeIdx].neighbors;
    }

    std::vector<int> getDistanceToAllNodes(size_t srcIdx) {
        std::vector<int> distance(getSize(), -1);
        std::queue<size_t> queue;

        distance[srcIdx] = 0;
        queue.push(srcIdx);

        while (!queue.empty()) {
            size_t parent = queue.front();
            queue.pop();

            for (auto& n : m_Nodes[parent].neighbors) {
                if (distance[n] == -1) {
                    distance[n] = distance[parent] + 1;
                    queue.push(n);
                }
            }
        }
        return distance;
    }
};

#endif  // DATASTRUCTURES_LISTGRAPH_H_

