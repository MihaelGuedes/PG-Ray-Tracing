#include "Render.cpp"
#include <iostream>

std::ostream& operator<< (std::ostream& os, Vector& v) {
    double t = 1.0;
    for (int i = 0; i < v.size(); i++) {
        t = std::max(t, v[i]);
    }
    v = v*255.999/t;
    for (int i = 0; i < v.size(); i++) {
        os << (char)v[i];
    }
    return os;
}

const double square_side = 0.5;

class Camera {
    Vector eye;
    Vector u, v, w;

public:
    Camera (Vector& eye, Vector& target, Vector& up) : eye(eye) {
        w = unit(eye - target);
        u = unit(cross(up, w));
        v = cross(w, u);
    }

    void render (double d, int vres, int hres, int max_depth) {
        Vector topleft = eye - w*d + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
        std::cout << "P6" << std::endl;
        std::cout << hres << ' ' << vres << std::endl;
        std::cout << 255 << std::endl;
        for (int i = 0; i < vres; i++) {
            for (int j = 0; j < hres; j++) {
                Vector pixelCenter = topleft + (u*j - v*i)*square_side;
                Vector pixelColor = ray_trace(Ray(eye, unit(pixelCenter - eye)), max_depth);
                std::cout << pixelColor;
            }
        }
    }

    void applyMatrix (const Matrix& m) {
        eye = affineTransformation(m, eye, true);
        u = affineTransformation(m, u, false);
        v = affineTransformation(m, v, false);
        w = affineTransformation(m, w, false);
    }
};