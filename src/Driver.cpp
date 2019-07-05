// Copyright 2019 Cosmin-Razvan Vancea

#include <string>
#include "./Driver.h"

int Driver::getDistanceTo(const std::string& destination) const {
    return m_City->getDistance(m_Location, destination);
}

bool Driver::driveTo(const std::string& destination, bool teleport) {
    if (!teleport) {
        m_TotalDistance += getDistanceTo(destination);
    }
    m_Location = destination;
    return 1;
}

void Driver::pickUpRider(int rating) {
    m_TotalRating += rating;
    m_TotalRides++;
}

void Driver::setStatus(int status) {
    m_Status = status;
}

int Driver::getStatus() const {
    return m_Status;
}

float Driver::getRating() const {
    if (m_TotalRides == 0) {
        return 0.0f;
    }
    return static_cast<float>(m_TotalRating) / m_TotalRides;
}

int Driver::getTotalDistance() const {
    return m_TotalDistance;
}

int Driver::getTotalRides() const {
    return m_TotalRides;
}

int Driver::getTotalRating() const {
    return m_TotalRating;
}

const std::string& Driver::getName() const {
    return m_Name;
}

const std::string& Driver::getLocation() const {
    return m_Location;
}
