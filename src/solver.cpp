// Copyright 2019 Cosmin-Razvan Vancea

#include <fstream>
#include <string>
#include <iomanip>

#include "./DataStructures/ListGraph.h"
#include "./DataStructures/BinarySearchTree.h"
#include "./DriverPool.h"
#include "./Utils.h"
#include "./solver.h"

void solver::task1_solver(std::ifstream& fin, std::ofstream& fout) {
    size_t nNodes, nEdges;
    fin >> nNodes >> nEdges;
    m_City.resize(nNodes);

    std::string nodeLabel;
    for (size_t i = 0; i != nNodes; ++i) {
        fin >> nodeLabel;
        m_City.setNodeLabel(i, nodeLabel);
    }

    std::string start, end;
    for (size_t i = 0; i != nEdges; ++i) {
        fin >> start >> end;
        m_City.addEdge(start, end);
    }

    size_t nQueries;
    fin >> nQueries;

    for (size_t i = 0; i != nQueries; ++i) {
        fin >> start >> end;
        if (m_City.isReachable(start, end)) {
            fout << 'y';
        } else {
            fout << 'n';
        }
        fout << std::endl;
    }
}

void solver::task2_solver(std::ifstream& fin, std::ofstream& fout) {
    size_t nQueries;
    std::string start, end;
    fin >> nQueries;

    for (size_t i = 0; i != nQueries; ++i) {
        fin >> start >> end;
        fout << m_City.getDistance(start, end) << std::endl;
    }
}

void solver::task3_solver(std::ifstream& fin, std::ofstream& fout) {
    char cmd;
    std::string nodeA, nodeB, nodeC;
    size_t param, nQueries;

    fin >> nQueries;
    for (size_t i = 0; i != nQueries; ++i) {
        fin >> cmd >> nodeA >> nodeB >> param;

        if (cmd == 'c') {
            switch (param) {
            case 0:
                m_City.addEdge(nodeA, nodeB);
                break;
            case 1:
                m_City.removeEdge(nodeA, nodeB);
                m_City.removeEdge(nodeB, nodeA);
                break;
            case 2:
                if (!m_City.hasEdge(nodeA, nodeB)) {
                    m_City.addEdge(nodeA, nodeB);
                }

                if (!m_City.hasEdge(nodeB, nodeA)) {
                    m_City.addEdge(nodeB, nodeA);
                }
                break;
            case 3:
                bool edgeAB = m_City.hasEdge(nodeA, nodeB);
                bool edgeBA = m_City.hasEdge(nodeB, nodeA);

                if (edgeAB && !edgeBA) {
                    m_City.removeEdge(nodeA, nodeB);
                    m_City.addEdge(nodeB, nodeA);
                } else if (!edgeAB && edgeBA) {
                    m_City.removeEdge(nodeB, nodeA);
                    m_City.addEdge(nodeA, nodeB);
                }
                break;
            }
        } else if (cmd == 'q') {
            switch (param) {
            case 0:
                if (m_City.isReachable(nodeA, nodeB)) {
                    fout << 'y';
                } else {
                    fout << 'n';
                }
                break;
            case 1:
                fout << m_City.getDistance(nodeA, nodeB);
                break;
            case 2:
                fin >> nodeC;
                int dist = m_City.getDistance(nodeA, nodeC);
                if (dist != -1) {
                    int distCB = m_City.getDistance(nodeC, nodeB);
                    if (distCB != -1) {
                        dist += distCB;
                    } else {
                        dist = -1;
                    }
                }
                fout << dist;
                break;
            }
            fout << std::endl;
        }
    }

    m_City.precomputeDistances();
}

void solver::task4_solver(std::ifstream& fin, std::ofstream& fout) {
    std::string cmd;
    size_t nQueries;

    fout.setf(std::ios::fixed, std::ios::floatfield);
    fout.precision(3);

    fin >> nQueries;
    for (size_t i = 0; i != nQueries; ++i) {
        fin >> cmd;
        if (cmd == "d") {
            std::string name, location;
            fin >> name >> location;

            Driver* driver = m_DriverPool.getDriver(name);
            if (driver == nullptr) {
                m_DriverPool.insertDriver(name, location);
            } else {
                driver->setStatus(true);
                driver->driveTo(location, true);
            }
        } else if (cmd == "b") {
            std::string name;
            fin >> name;

            Driver* driver = m_DriverPool.getDriver(name);
            if (driver != nullptr) {
                driver->setStatus(false);
            }
        } else if (cmd == "r") {
            std::string start, end;
            int rating;
            fin >> start >> end >> rating;

            bool isDestinationReachable = m_City.isReachable(start, end);
            if (!isDestinationReachable) {
                for (auto& neighbor : m_City.getNeighbors(end)) {
                    if (m_City.isReachable(end, neighbor) &&
                        m_City.isReachable(start, neighbor)) {
                        end = neighbor;
                        isDestinationReachable = true;
                        break;
                    }
                }
            }

            Driver* driver = m_DriverPool.getBestDriver(start);
            if (driver == nullptr) {
                fout << "Soferi indisponibili" << std::endl;
            } else {
                if (!isDestinationReachable) {
                    fout << "Destinatie inaccesibila" << std::endl;
                } else {
                    m_DriverPool.getDriversByAvgRating().erase(driver);
                    m_DriverPool.getDriversByTotalDist().erase(driver);
                    m_DriverPool.getDriversByTotalRides().erase(driver);

                    driver->driveTo(start);
                    driver->pickUpRider(rating);
                    driver->driveTo(end);

                    m_DriverPool.getDriversByAvgRating().insert(driver);
                    m_DriverPool.getDriversByTotalDist().insert(driver);
                    m_DriverPool.getDriversByTotalRides().insert(driver);
                }
            }
        } else if (cmd == "top_rating") {
            size_t nDrivers, idx = 0;
            fin >> nDrivers;

            auto avgRating = m_DriverPool.getDriversByAvgRating().inOrderTraverse();
            for (auto& driver : avgRating) {
                if (++idx > nDrivers) {
                    break;
                }
                fout << driver->getName() << ":" << driver->getRating() << " ";
            }
            fout << std::endl;
        } else if (cmd == "top_dist") {
            size_t nDrivers, idx = 0;
            fin >> nDrivers;

            auto totalDist = m_DriverPool.getDriversByTotalDist().inOrderTraverse();
            for (auto& driver : totalDist) {
                if (++idx > nDrivers) {
                    break;
                }
                fout << driver->getName() << ":" << driver->getTotalDistance() << " ";
            }
            fout << std::endl;
        } else if (cmd == "top_rides") {
            size_t nDrivers, idx = 0;
            fin >> nDrivers;

            auto totalRides = m_DriverPool.getDriversByTotalRides().inOrderTraverse();
            for (auto& driver : totalRides) {
                if (++idx > nDrivers) {
                    break;
                }
                fout << driver->getName() << ":" << driver->getTotalRides() << " ";
            }
            fout << std::endl;
        } else if (cmd == "info") {
            std::string name;
            fin >> name;

            Driver* driver = m_DriverPool.getDriver(name);
            if (driver != nullptr) {
                fout << driver->getName() << ": " << driver->getLocation() << " "
                    << driver->getRating() << " " << driver->getTotalRides() << " "
                    << driver->getTotalDistance() << " "
                    << (driver->getStatus() ? "online" : "offline") << std::endl;
            }
        }
    }
}

void solver::task5_solver(std::ifstream& fin, std::ofstream& fout) {
    struct RemainingFuel {
        std::string m_Location;
        int m_Fuel;

        RemainingFuel(const std::string& location, int fuel) :
            m_Location(location), m_Fuel(fuel) { }

        bool operator<(const RemainingFuel& rhs) const {
            if (m_Fuel > rhs.m_Fuel) {
                return 1;
            } else if (m_Fuel == rhs.m_Fuel) {
                return m_Location < rhs.m_Location;
            }
            return 0;
        }
    };

    BinarySearchTree<RemainingFuel, utils::less<RemainingFuel>> allLocations;
    std::string driverName, nextLocation;
    int driverFuel, nNodes;

    fin >> driverFuel >> driverName >> nNodes;
    Driver* driver = m_DriverPool.getDriver(driverName);

    for (int i = 0; i != nNodes; ++i) {
        fin >> nextLocation;

        int dist = driver->getDistanceTo(nextLocation);
        int remainingFuel = driverFuel - dist;

        if (dist != -1 && remainingFuel >= 0) {
            allLocations.insert(RemainingFuel(nextLocation, remainingFuel));
        }
    }

    for (auto& i : allLocations.inOrderTraverse()) {
        fout << i.m_Location << " ";
    }
    fout << std::endl;
}
