#include "Matrix.cpp"

const double EPSILON = 1e-9;

struct Ray {
    Vector origin, direction;
    
    Ray (const Vector& origin, const Vector& direction) :
        origin(origin),
        direction(direction)
    {}

    Vector at (double t) const {
        return origin + direction*t;
    }
};

struct Shape {
    Vector color;

    Shape(const Vector& color) : color(color) {}

    virtual bool intersect (const Ray& ray, double& t) {
        return {};
    }

    virtual void applyMatrix (const Matrix& m) {
        return;
    }

    virtual Vector normal (const Vector& p) {
        return {};
    }
};

struct Sphere : public Shape {
    Vector center;
    double radius;

    Sphere (const Vector& color, const Vector& center, double radius) :
        Shape(color),
        center(center),
        radius(radius)
    {}

    bool intersect (const Ray& ray, double& t) {
        Vector v = center - ray.origin;
        double tca = dot(v, ray.direction);
        double d2 = dot(v, v) - tca*tca;
        double r2 = radius*radius;
        if (d2 - r2 > EPSILON) {
            return false;
        }
        double thc = std::sqrt(r2 - d2);
        double t0 = tca - thc, t1 = tca + thc;
        t = t0;
        if (t0 < EPSILON) {
            if (t1 < EPSILON) {
                return false;
            }
            t = t1;
        }
        return true;
    }

    void applyMatrix (const Matrix& m) {
        center = affineTransformation(m, center, true);
    }

    Vector normal (const Vector& p) {
        return unit(p - center);
    }
};

struct Plane : public Shape {
    Vector sample;
    Vector n;

    Plane (const Vector& color, const Vector& sample, const Vector& normal) :
        Shape(color),
        sample(sample),
        n(unit(normal))
    {}
    
    bool intersect (const Ray& ray, double& t) {
        double aux = dot(ray.direction, n);
        if (std::abs(aux) < EPSILON) {
            return false;
        } else {
            t = dot(sample - ray.origin, n)/aux;
            return t > EPSILON;
        }
    }

    void applyMatrix (const Matrix& m) {
        sample = affineTransformation(m, sample, true);
        n = affineTransformation(m, n, false);
    }

    Vector normal(const Vector& p) {
        return n;
    }
};

struct Triangle : public Plane {
    Vector hb, hc;

    Triangle(const Vector &color, const Vector &a, const Vector &b, const Vector& c) :
        Plane(color, a, unit(cross(b - a, c - a)))
    {
        Vector u = b - a, v = c - a;
        Vector projuv = v*(dot(u, v)/dot(v, v));
        Vector projvu = u*(dot(u, v)/dot(u, u));
        hb = u - projuv;
        hb = hb/dot(hb, hb);
        hc = v - projvu;
        hc = hc/dot(hc, hc);
    }

    bool intersect(const Ray& ray, double& t) {
        if (!Plane::intersect(ray, t)) {
            return false;
        }
        Vector p = ray.at(t);
        Vector v = p - sample;
        double beta = dot(v, hb), gamma = dot(v, hc), alpha = 1 - (gamma + beta);
        if (alpha < -EPSILON || beta < -EPSILON || gamma < -EPSILON) {
            return false;
        }
        return true;
    }

    void applyMatrix (const Matrix& m) {
        Plane::applyMatrix(m);
        hb = affineTransformation(m, hb, false);
        hc = affineTransformation(m, hc, false);
    }
};