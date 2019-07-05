// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_

#include <fstream>
#include <string>

#include "./DataStructures/ListGraph.h"
#include "./DriverPool.h"
#include "./Utils.h"

class solver {
    ListGraph<std::string, utils::string_hash> m_City;
    DriverPool m_DriverPool{ &m_City };

 public:
     void task1_solver(std::ifstream& fin, std::ofstream& fout);
     void task2_solver(std::ifstream& fin, std::ofstream& fout);
     void task3_solver(std::ifstream& fin, std::ofstream& fout);
     void task4_solver(std::ifstream& fin, std::ofstream& fout);
     void task5_solver(std::ifstream& fin, std::ofstream& fout);
};

#endif  // SOLVER_H_
