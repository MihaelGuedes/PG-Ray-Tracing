#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <cmath>

class Vector {

    double* components;
    int dimension;

public:

    // Constructor with initializer list
    Vector(std::initializer_list<double> values) {
        dimension = values.size();
        components = new double[dimension];
        int i = 0;
        for (double value : values) {
            components[i++] = value;
        }
    }

    // n-dimensional zero vector
    Vector (int n) {
        dimension = n;
        components = new double[n];
        for (int i = 0; i < n; i++) {
            components[i] = 0.0;
        }
    }

    // Default constructor
    Vector() {
        components = nullptr;
        dimension = 0;
    }

    int size () const {
        return dimension;
    }

    double& operator[] (int i) {
        return components[i];
    }

    double& operator[] (int i) const {
        return components[i];
    }

    // Overloaded addition operator
    Vector operator+(const Vector& other) const {
        Vector result(dimension);
        for (int i = 0; i < dimension; i++) {
            result.components[i] = components[i] + other.components[i];
        }
        return result;
    }

    // Overloaded subtraction operator
    Vector operator-(const Vector& other) const {
        Vector result(dimension);
        for (int i = 0; i < dimension; i++) {
            result.components[i] = components[i] - other.components[i];
        }
        return result;
    }
    
    // Overloaded multiplication by scalar operator
    Vector operator*(const double& scalar) const {
        Vector result(dimension);
        for (int i = 0; i < dimension; i++) {
            result.components[i] = components[i] * scalar;
        }
        return result;
    }

    // Overloaded multiplication by scalar operator
    Vector operator/(const double& scalar) const {
        Vector result(dimension);
        for (int i = 0; i < dimension; i++) {
            result.components[i] = components[i]/scalar;
        }
        return result;
    }

    // Haddamard product (component-wise)
    Vector operator*(const Vector& other) const {
        Vector result(dimension);
        for (int i = 0; i < dimension; i++) {
            result.components[i] = components[i] * other.components[i];
        }
        return result;
    }

    friend std::istream& operator>> (std::istream& is, Vector& v) {
        for (int i = 0; i < v.dimension; i++) {
            is >> v[i];
        }
        return is;
    }
    
    std::string debug () const {
        std::stringstream ss;
        ss << std::setprecision(2) << std::fixed;
        for (int i = 0; i < dimension; i++) {
            ss << components[i] << ' ';
        }
        return ss.str();
    }

};

Vector cross (const Vector& u, const Vector& v) {
    return {u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]};
}

double dot (const Vector& u, const Vector& v) {
    double result = 0.0;
    for (int i = 0; i < u.size(); i++) {
        result += u[i]*v[i];
    }
    return result;
}

Vector unit (const Vector& v) {
    double norm = sqrt(dot(v, v));
    return v/norm;
}