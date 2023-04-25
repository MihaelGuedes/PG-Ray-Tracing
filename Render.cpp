#include "Shape.cpp"
#include <vector>

class Material {
    Shape* shape;
    Vector cd;
    double ka, kd, ks;
    int eta;

public:
    Material (Shape* shape, const Vector& cd, double kd, double ks, double ka, int eta) :
        shape(shape), cd(cd), kd(kd), ks(ks), ka(ka), eta(eta)
    {}

    Shape* getShape () {
        return shape;
    }

    Vector shade (const Vector &p, const Vector &v);
};

std::vector<Material> objects;

Material* nearest (const Ray& ray, double& t_min) {
    Material* hit = nullptr;
    for (Material& object : objects) {
        double t;
        if (object.getShape()->intersect(ray, t) && (!hit || t < t_min)) {
            t_min = t;
            hit = &object;
        }
    }
    return hit;
}

Vector ray_cast (const Ray& ray) {
    double t_min;
    Material* hit = nearest(ray, t_min);
    if (hit) {
        Vector p = ray.at(t_min);
        return hit->shade(p, ray.direction*-1);
    } else {
        return Vector(3);
    }
}

struct Light {
    Vector position, intensity;
    Light() : position(3), intensity(3) {}
};

Vector ambient_light(3);
std::vector<Light> lights;

Vector Material::shade (const Vector &p, const Vector &v) {
    Vector n = shape->normal(p);
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