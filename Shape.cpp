#include "Matrix.cpp"

const double EPSILON = 1e-9;

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

class Shape {
public:
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

class Sphere : public Shape {
    Vector center;
    double radius;

public:
    Sphere (const Vector& center, double radius) : center(center), radius(radius) {}

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

class Plane : public Shape {
protected:
    Vector sample;
    Vector n;

public:
    Plane (const Vector& sample, const Vector& normal) : sample(sample), n(unit(normal)) {}
    
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

class Triangle : public Plane {
    Vector hb, hc;

public:
    Triangle (const Vector &a, const Vector &b, const Vector& c) : Plane(a, unit(cross(b - a, c - a))) {
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