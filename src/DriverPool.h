// Copyright 2019 Cosmin-Razvan Vancea

#ifndef DRIVERPOOL_H_
#define DRIVERPOOL_H_

#include <string>
#include <list>

#include "./DataStructures/ListGraph.h"
#include "./DataStructures/BinarySearchTree.h"
#include "./Driver.h"
#include "./Utils.h"

class DriverPool {
    struct cmp_DriverByTotalDistance {
        bool operator()(const Driver *lhs, const Driver *rhs) const {
            if (lhs->getTotalDistance() > rhs->getTotalDistance()) {
                return 1;
            } else if (lhs->getTotalDistance() == rhs->getTotalDistance()) {
                return lhs->getName() < rhs->getName();
            }
            return 0;
        }
    };

    struct cmp_DriverByTotalRides {
        bool operator()(const Driver *lhs, const Driver *rhs) const {
            if (lhs->getTotalRides() > rhs->getTotalRides()) {
                return 1;
            } else if (lhs->getTotalRides() == rhs->getTotalRides()) {
                return lhs->getName() < rhs->getName();
            }
            return 0;
        }
    };

    struct cmp_DriverByAvgRating {
        bool operator()(const Driver* lhs, const Driver* rhs) const {
            // aduc la acelasi numitor si compar ca integer, nu float

            // rezultatele difera in functie de arhitectura procesorului
            // (si/sau alte motive): fail pe x86, pass pe x86-64

            // ar fi fost ok sa fie pe undeva postate specificatiile
            // hardware + software ale masinilor pe care ruleaza vmchecker

            int lhsTotalRating = lhs->getTotalRating();
            int lhsTotalRides = lhs->getTotalRides();

            int rhsTotalRating = rhs->getTotalRating();
            int rhsTotalRides = rhs->getTotalRides();

            int l = lhsTotalRating * rhsTotalRides;
            int r = rhsTotalRating * lhsTotalRides;

            // se trateaza separat cazul in care numitorul e 0
            if (lhsTotalRides == 0) {
                l = 0;
                if (rhsTotalRides != 0) {
                    r = 1;
                }
            }
            if (rhsTotalRides == 0) {
                r = 0;
                if (lhsTotalRides != 0) {
                    l = 1;
                }
            }

            if (l > r) {
                return 1;
            } else if (l == r) {
                return lhs->getName() < rhs->getName();
            }
            return 0;
        }
    };

    std::list<Driver> m_Drivers;
    ListGraph<std::string, utils::string_hash> *m_City;
    BinarySearchTree<Driver*, cmp_DriverByTotalDistance> m_TotalDist;
    BinarySearchTree<Driver*, cmp_DriverByTotalRides> m_TotalRides;
    BinarySearchTree<Driver*, cmp_DriverByAvgRating> m_AvgRating;

 public:
    explicit DriverPool(ListGraph<std::string, utils::string_hash> *city) :
        m_City(city) {
    }

    DriverPool(const DriverPool&) = delete;
    void operator= (const DriverPool&) = delete;

    void insertDriver(const std::string& name, const std::string& location);

    Driver* getDriver(const std::string& name);
    Driver* getBestDriver(const std::string& location);
    const std::list<Driver>& getDrivers() const;

    BinarySearchTree<Driver*, cmp_DriverByTotalDistance>& getDriversByTotalDist();
    BinarySearchTree<Driver*, cmp_DriverByTotalRides>& getDriversByTotalRides();
    BinarySearchTree<Driver*, cmp_DriverByAvgRating>& getDriversByAvgRating();
};

#endif  // DRIVERPOOL_H_
