// Find clusters of 1 in a binary matrix
// A binary matrix has only 0 and 1 as elements

#include <iostream>

template <typename T>
class Matrix {
public:
	Matrix(int rows, int cols) {
		mRows = rows;
		mCols = cols;
		mMatrix = new T[rows*cols];
	}

	void set(int r, int c, T const& v) {
		if (r < 0 || r >= mRows || c < 0 || c >= mCols) {
			throw std::runtime_error("Invalid index");
		}

		mMatrix[r*mCols + c] = v;
	}

	T get(int r, int c) const {
		if (r < 0 || r >= mRows || c < 0 || c >= mCols) {
			throw std::runtime_error("Invalid index");
		}

		return mMatrix[r*mCols + c];
	}

	void print() const {
		for (int i = 0; i < mRows*mCols; ++i) {
			std::cout << "Matrix[" << i/mCols << "][" << i % mCols << "] = " << mMatrix[i] << std::endl;
			if ((i + 1) % mCols == 0) {
				std::cout << std::endl;
			}
		}
	}

	int getRows() const {
		return mRows;
	}

	int getCols() const {
		return mCols;
	}

private:
	int mRows;
	int mCols;
	T   *mMatrix;
};

void visitCluster(Matrix<bool> const& inMat, int i, int j, Matrix<bool> visited) {

	if (i > 0 && !visited.get(i - 1, j) && inMat.get(i - 1, j)) {
		visited.set(i - 1, j, true);
		visitCluster(inMat, i - 1, j, visited);
	}
	if (j > 0 && !visited.get(i, j - 1) && inMat.get(i, j - 1)) {
		visited.set(i, j - 1, true);
		visitCluster(inMat, i, j - 1, visited);
	}
	if (i < (inMat.getRows() - 1) && !visited.get(i + 1, j) && inMat.get(i + 1, j)) {
		visited.set(i + 1, j, true);
		visitCluster(inMat, i + 1, j, visited);
	}
	if (j < (inMat.getCols() - 1) && !visited.get(i, j + 1) && inMat.get(i, j + 1)) {
		visited.set(i, j + 1, true);
		visitCluster(inMat, i, j + 1, visited);
	}
}

int countClustersOfOnes(Matrix<bool> const& inMat) {
	int count = 0;

	int rows = inMat.getRows();
	int cols = inMat.getCols();
	Matrix<bool> visited(rows, cols);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (!visited.get(i, j) && inMat.get(i, j)) {
				count++;
				visited.set(i, j, true);
				visitCluster(inMat, i, j, visited);
			}
		}
	}

	return count;
}

int main(int argc, char** argv) {
	Matrix<bool> intMat(3, 4);

	intMat.set(0, 0, true);
	intMat.set(0, 1, true);
	intMat.set(0, 2, false);
	intMat.set(0, 3, true);

	intMat.set(1, 0, false);
	intMat.set(1, 1, true);
	intMat.set(1, 2, false);
	intMat.set(1, 3, true);

	intMat.set(2, 0, false);
	intMat.set(2, 1, false);
	intMat.set(2, 2, true);
	intMat.set(2, 3, false);

	intMat.print();

	std::cout << "Count of clusters of 1: " << countClustersOfOnes(intMat) << "\n" << std::endl;

	Matrix<std::string> strMat(1, 2);
	strMat.set(0, 0, "foo");
	strMat.set(0, 1, "bar");

	strMat.print();

	return 0;
}
