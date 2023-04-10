#include <bits/stdc++.h>
#include "Shape.cpp"

using namespace std;

vector<Shape*> shapes;

Shape* nearest (const Ray& ray, double& t_min) {
    Shape* hit = nullptr;
    for (Shape* shape : shapes) {
        double t;
        if (shape->intersect(ray, t) && (!hit || t < t_min)) {
            t_min = t;
            hit = shape;
        }
    }
    return hit;
}

Vector ray_cast (const Ray& ray) {
    double t_min = numeric_limits<double>::max();
    Shape* hit = nearest(ray, t_min);
    if (hit) {
        return hit->color;
    } else {
        return Vector(3);
    }
}

std::ostream& operator<< (std::ostream& os, Vector& v) {
    for (int i = 0; i < v.size(); i++) {
        os << (char)v[i];
    }
    return os;
}

const double square_side = 0.1;

class Camera {
    Vector eye;
    Vector u, v, w;

public:
    Camera (Vector& eye, Vector& target, Vector& up) : eye(eye) {
        w = unit(eye - target);
        u = unit(cross(up, w));
        v = cross(w, u);
    }

    void render (double d, int vres, int hres) {
        Vector topleft = eye - w*d + (v*(vres - 1) - u*(hres - 1))*square_side/2.0;
        std::cout << "P6" << std::endl;
        std::cout << hres << ' ' << vres << std::endl;
        std::cout << 255 << std::endl;
        for (int i = 0; i < vres; i++) {
            for (int j = 0; j < hres; j++) {
                Vector pixelCenter = topleft + (u*j - v*i)*square_side;
                Vector pixelColor = ray_cast(Ray(eye, unit(pixelCenter - eye)));
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

int main() {
    Camera* camera = nullptr;
    int vres, hres;
    double d;
    while (cin) {
        char type;
        cin >> type;
        switch (type) {
            case 'c': {
                Vector up(3), c(3), m(3);
                cin >> hres >> vres >> d >> up >> c >> m;
                camera = new Camera(c, m, up);
            } break;
            case 's': {
                Vector o(3), c(3);
                double r;
                cin >> c >> r >> o;
                shapes.push_back(new Sphere(o, c, r));
                //shapes.back()->applyMatrix(translate({150.0, 0.0, 0.0}));
            } break;
            case 'p': {
                Vector p0(3), n(3), o(3);
                cin >> p0 >> n >> o;
                shapes.push_back(new Plane(o, p0, n));
                //shapes.back()->applyMatrix(rotateX(30.0, true));
            } break;
            case 't': {
                int numFaces, numVertices;
                cin >> numFaces >> numVertices;
                vector<Vector> vertices;
                while (numVertices--) {
                    Vector p(3);
                    cin >> p;
                    vertices.push_back(p);
                }
                vector<tuple<int, int, int>> faces;
                while (numFaces--) {
                    int i, j, k;
                    cin >> i >> j >> k;
                    i--;
                    j--;
                    k--;
                    faces.emplace_back(i, j, k);
                }
                Vector o(3);
                cin >> o;
                for (auto [i, j, k] : faces) {
                    shapes.push_back(new Triangle(o, vertices[i], vertices[j], vertices[k]));
                    //shapes.back()->applyMatrix(rotateX(40.0, true));
                }
                break;
            }
        }
        type = -1;
    }
    //camera->applyMatrix(rotateZ(30, false));
    camera->render(d, vres, hres);
}