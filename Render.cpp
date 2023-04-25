#include "Shape.cpp"
#include <vector>

class Material {
    Shape* shape;
    Vector cd;
    double ka, kd, ks;
    int eta;
    double ior;

public:

    double kr, kt;

    Material (Shape* shape, const Vector& cd, double kd, double ks, double ka, double kr, double kt, int eta, double ior) :
        shape(shape), cd(cd), kd(kd), ks(ks), ka(ka), kr(kr), kt(kt), eta(eta), ior(ior)
    {}

    Shape* getShape () {
        return shape;
    }

    Vector shade (const Vector &p, const Vector &v, const Vector& n);

    Vector refract (const Vector& v, const Vector& n) {
        double d = dot(n, v);
        Vector n2 = n;
        double ior2 = ior; 
        if (d < 0.0) {
            n2 = n2*-1;
            ior2 = 1.0/ior2;
            d *= -1;
        }
        double delta = 1.0 - (1.0 - d*d)/(ior2*ior2);
        if (delta < 0.0) {
            throw -1;
        }
        return v/(-ior2) - n2*(sqrt(delta) - d/ior2);
    }
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

Vector ray_trace (const Ray &ray, int ttl) {
    Vector color(3);
    double t_min;
    Material* object = nearest(ray, t_min);
    if (object) {
        Vector p = ray.at(t_min);
        Vector v = ray.direction*-1;
        Vector n = object->getShape()->normal(p);
        color = object->shade(p, v, n);
        if (ttl > 0) {
            Ray reflected(p, n*(2.0*dot(n, v)) - v);
            try {
                if (object->kt > 0.0) {
                    Ray refracted(p, object->refract(v, n));
                    color = color + ray_trace(refracted, ttl - 1)*object->kt;
                }
                if (object->kr > 0.0) {
                    color = color + ray_trace(reflected, ttl - 1)*object->kr;
                }
            } catch (int e) {
                color = color + ray_trace(reflected, ttl - 1);
            }
        }
    }
    return color;
}

struct Light {
    Vector position, intensity;
    Light() : position(3), intensity(3) {}
};

Vector ambient_light(3);
std::vector<Light> lights;

Vector Material::shade (const Vector &p, const Vector &v, const Vector& n) {
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