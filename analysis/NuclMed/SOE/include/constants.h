
#pragma once

#ifndef _SOE_CONSTANTS_H__
#define _SOE_CONSTANTS_H__

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

const double kPI = 3.14159265358979323846;

const double mass_electron = 510.99907; 	// keV

const double kMILLION = 1E6;

bool doubleEquals(double left, double right, double epsilon);

void WriteString(std::ostream& io_os, const std::string& in_string);
void ReadString(std::istream& in_is, std::string& io_string);

void bubbleSort( int size, double* x );

int solveQuadraticEquation(const double& in_a, const double& in_b, const double& in_c
		, double& out_x1, double& out_x2);

#endif
