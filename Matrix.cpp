#include "Vector.cpp"

class Matrix {

    Vector* rows;
    int numRows;
    int numCols;

public:
    // Constructor with initializer list
    Matrix(std::initializer_list<std::initializer_list<double>> values) {
        numRows = values.size();
        numCols = values.begin()->size();
        rows = new Vector[numRows];
        int i = 0;
        for (auto row : values) {
            if (row.size() != numCols) {
                throw std::invalid_argument("All rows must have the same number of columns.");
            }
            Vector aux(row);
            rows[i++] = aux;
        }
    }

    // n x m zero matrix
    Matrix(int n, int m) {
        numRows = n;
        numCols = m;
        rows = new Vector[n];
        for (int i = 0; i < n; i++) {
            Vector aux(m);
            rows[i] = aux;
        }
    }

    // Default constructor
    Matrix () {
        rows = nullptr;
        numRows = numCols = 0;
    }

    Vector& operator[] (int i) {
        return rows[i];
    }

    // Method to return the number of rows
    int getNumRows() const {
        return numRows;
    }

    // Method to return the number of columns
    int getNumCols() const {
        return numCols;
    }

    // Overloaded multiplication operator
    Matrix operator*(const Matrix& other) const {
        if (numCols != other.numRows) {
            throw std::invalid_argument("The number of columns of the first matrix must be equal to the number of rows of the second matrix.");
        }
        Matrix result(numRows, other.numCols);

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < other.numCols; j++) {
                for (int k = 0; k < numCols; k++) {
                    result[i][j] += rows[i][k] * other.rows[k][j];
                }
            }
        }
        return result;
    }

    // Matrix vector multiplication
    Vector operator* (const Vector& v) const {
        if (numCols != v.size()) {
            throw std::invalid_argument("The number of columns of the matrix must be equal to the number of components of the vector.");
        }

        Vector result(numCols);

        for (int i = 0; i < numRows; i++) {
            result[i] = dot(rows[i], v);
        }
        return result;
    }
};

Vector affineTransformation (const Matrix& m, const Vector& v, bool homogeneous) {
    Vector vt = m*Vector({v[0], v[1], v[2], homogeneous ? 1.0 : 0.0});
    return {vt[0], vt[1], vt[2]};
}

Matrix translate (const Vector& t) {
    return {
        {1.0, 0.0, 0.0, t[0]},
        {0.0, 1.0, 0.0, t[1]},
        {0.0, 0.0, 1.0, t[2]},
        {0.0, 0.0, 0.0, 1.0}
    };
}

Matrix rotateX (double theta, bool clockwise, bool conversion=true) {
    if (conversion) {
        theta *= std::acos(-1)/180.0;
    }
    double c = std::cos(theta), s = std::sin(theta);
    if (clockwise) {
        s *= -1.0;
    }
    return {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, c, -s, 0.0},
        {0.0, s, c, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
}

Matrix rotateY (double theta, bool clockwise, bool conversion=true) {
    if (conversion) {
        theta *= std::acos(-1)/180.0;
    }
    double c = std::cos(theta), s = std::sin(theta);
    if (clockwise) {
        s *= -1.0;
    }
    return {
        {c, 0.0, s, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {-s, 0.0, c, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
}

Matrix rotateZ (double theta, bool clockwise, bool conversion=true) {
    if (conversion) {
        theta *= std::acos(-1)/180.0;
    }
    double c = std::cos(theta), s = std::sin(theta);
    if (clockwise) {
        s *= -1.0;
    }
    return {
        {c, -s, 0.0, 0.0},
        {s, c, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
}