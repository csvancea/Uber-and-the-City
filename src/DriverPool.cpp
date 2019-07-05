// Copyright 2019 Cosmin-Razvan Vancea

#include <string>
#include <list>
#include <limits>

#include "./DriverPool.h"

void DriverPool::insertDriver(const std::string& name,
                              const std::string& location) {
    m_Drivers.push_back(Driver(name, location, m_City));

    m_TotalDist.insert(&m_Drivers.back());
    m_AvgRating.insert(&m_Drivers.back());
    m_TotalRides.insert(&m_Drivers.back());
}

Driver* DriverPool::getDriver(const std::string& name) {
    auto driver = std::find_if(std::begin(m_Drivers), std::end(m_Drivers),
        [name](const Driver & driver) {
            return (driver.getName() == name);
        });

    if (driver != std::end(m_Drivers)) {
        return &*driver;
    }

    return nullptr;
}

Driver* DriverPool::getBestDriver(const std::string& location) {
    Driver* bestMatch = nullptr;
    int minDist = std::numeric_limits<int>::max();

    auto vecAvgRating = m_AvgRating.inOrderTraverse();
    for (auto& driver : vecAvgRating) {
        if (driver->getStatus() == 0) {
            continue;
        }

        int currDist = driver->getDistanceTo(location);
        if (currDist != -1 && minDist > currDist) {
            bestMatch = driver;
            minDist = currDist;

            if (currDist == 0) {
                return bestMatch;
            }
        }
    }
    return bestMatch;
}

const std::list<Driver>& DriverPool::getDrivers() const {
    return m_Drivers;
}

BinarySearchTree<Driver*, DriverPool::cmp_DriverByTotalDistance>&
DriverPool::getDriversByTotalDist() {
    return m_TotalDist;
}

BinarySearchTree<Driver*, DriverPool::cmp_DriverByTotalRides>&
DriverPool::getDriversByTotalRides() {
    return m_TotalRides;
}

BinarySearchTree<Driver*, DriverPool::cmp_DriverByAvgRating>&
DriverPool::getDriversByAvgRating() {
    return m_AvgRating;
}
