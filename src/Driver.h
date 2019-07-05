// Copyright 2019 Cosmin-Razvan Vancea

#ifndef DRIVER_H_
#define DRIVER_H_

#include <string>

#include "./DataStructures/ListGraph.h"
#include "./Utils.h"

class Driver {
    ListGraph<std::string, utils::string_hash>* m_City;
    std::string m_Name;
    std::string m_Location;

    int m_Status;
    int m_TotalRating;
    int m_TotalRides;
    int m_TotalDistance;

 public:
    Driver(const std::string& name, const std::string& location,
        ListGraph<std::string, utils::string_hash> *city) :
        m_City(city), m_Name(name), m_Location(location),
        m_Status(1), m_TotalRating(0), m_TotalRides(0),
        m_TotalDistance(0) {
    }

    int getDistanceTo(const std::string& destination) const;
    bool driveTo(const std::string& destination, bool teleport = false);
    void pickUpRider(int rating);

    void setStatus(int status);
    int getStatus() const;

    float getRating() const;
    int getTotalDistance() const;
    int getTotalRides() const;
    int getTotalRating() const;

    const std::string& getName() const;
    const std::string& getLocation() const;
};

#endif  // DRIVER_H_
