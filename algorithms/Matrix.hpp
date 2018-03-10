#pragma once

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>


template<class elementType>
class Matrix {
public:
	Matrix(unsigned long dataRows, unsigned long dataCols, const std::vector<elementType>& data) :
		dim(dataRows, dataCols),
		elements(data) {}

	elementType operator()(unsigned long row, unsigned long col) const {
		return elements[dim.first*row + col];
	}

	static Matrix<elementType> load(const std::string& filename);

	unsigned long nRows() {
		return dim.first;
	}

	unsigned long nCols() {
		return dim.second;
	}

private:
	std::vector<elementType> elements;
	std::pair<unsigned long, unsigned long> dim;
};

template<class elementType>
Matrix<elementType> Matrix<elementType>::load(const std::string& filename) {
	unsigned long nRows, nCols;
	std::ifstream matrixFile(filename);

	matrixFile >> nRows >> nCols;

	std::vector<elementType> data(nRows*nCols);

	for (unsigned long i = 0; i < data.size(); i++) {
		matrixFile >> data[i];
	}

	return Matrix<elementType>(nRows, nCols, data);
}

template<class elementType>
std::ostream& operator<<(std::ostream& os, Matrix<elementType> matrix) {
	for (unsigned long row = 0; row < matrix.nRows(); ++row) {
		os << "[";
		for (unsigned long col = 0; col < matrix.nCols(); ++col) {
			os << matrix(row, col);
			os << (col == matrix.nCols() - 1 ? "]" : ", ");
		}
		os << "\n";
	}

	return os;
}
