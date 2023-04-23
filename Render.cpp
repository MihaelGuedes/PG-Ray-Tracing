#include "Matrix.cpp"
#include <vector>

struct Ray {
    Vector origin, direction;
    
    Ray (const Vector& origin, const Vector& direction) :
        origin(origin + direction * 1e-5),
        direction(direction)
    {}

    Vector at (double t) const {
        return origin + direction*t;
    }
};

struct Light {
    Vector position, intensity;
    Light() : position(3), intensity(3) {}
};

Vector ambient_light(3);
std::vector<Light> lights;

struct Shape {
    Vector cd;
    double ka, kd, ks;
    int eta;

    Shape (const Vector &cd, double ka, double kd, double ks, int eta) :
        cd(cd), ka(ka), kd(kd), ks(ks), eta(eta)
    {}

    virtual bool intersect (const Ray& ray, double& t) {
        return {};
    }

    virtual void applyMatrix (const Matrix& m) {
        return;
    }

    virtual Vector normal (const Vector& p) {
        return {};
    }

    Vector shade (const Vector &p, const Vector &v, const Vector &n);
};

std::vector<Shape*> shapes;

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
    double t_min;
    Shape* hit = nearest(ray, t_min);
    if (hit) {
        Vector p = ray.at(t_min);
        return hit->shade(p, ray.direction*-1, hit->normal(p));
    } else {
        return Vector(3);
    }
}

Vector Shape::shade (const Vector &p, const Vector &v, const Vector &n) {
    Vector cp = cd*ambient_light*ka;
    for (Light light : lights) {
        Vector l = unit(light.position - p), r = n*2.0*(dot(n, l)) - l;
        double t;
        auto shadow = nearest(Ray(p, l), t);
        if (shadow == nullptr || dot(l, light.position - p) < t) {
            double dotd = dot(n, l);
            if (dotd > 0.0) {
                cp = cp + cd*light.intensity*dotd*kd;
            }
            double dots = dot(r, v);
            if (dots > 0.0) {
                cp = cp + light.intensity*ks*std::pow(dots, eta);
            }
        }
    }
    return cp;
}