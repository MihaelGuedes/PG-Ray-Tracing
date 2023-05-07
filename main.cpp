#include "Camera.cpp"

extern Vector ambient_light;
extern std::vector<AreaLight> lights;
extern std::vector<Material> objects;

using namespace std;

int main() {
    Camera* camera = nullptr;
    int vres, hres;
    double d;
    int max_depth;
    while (cin) {
        char type;
        cin >> type;
        switch (type) {
            case 'c': {
                Vector up(3), c(3), m(3);
                cin >> hres >> vres >> d >> up >> c >> m >> max_depth;
                camera = new Camera(c, m, up);
            } break;
            case 's': {
                Vector o(3), c(3);
                double r;
                double kd, ks, ka, kr, kt, ior;
                int p;
                cin >> c >> r >> o >> kd >> ks >> ka >> kr >> kt >> p >> ior;
                o = o/255.0;
                objects.emplace_back(new Sphere(c, r), o, kd, ks, ka, kr, kt, p, ior);
            } break;
            case 'p': {
                Vector p0(3), n(3), o(3);
                double kd, ks, ka, kr, kt, ior;
                int p;
                cin >> p0 >> n >> o >> kd >> ks >> ka >> kr >> kt >> p >> ior;
                o = o/255.0;
                objects.emplace_back(new Plane(p0, n), o, kd, ks, ka, kr, kt, p, ior);
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
                double kd, ks, ka, kr, kt, ior;
                int p;
                cin >> o >> kd >> ks >> ka >> kr >> kt >> p >> ior;
                o = o/255.0;
                for (auto [i, j, k] : faces) {
                    objects.emplace_back(new Triangle(vertices[i], vertices[j], vertices[k]), o, kd, ks, ka, kr, kt, p, ior);
                    objects.back().getShape()->applyMatrix(rotateZ(15, false));
                    objects.back().getShape()->applyMatrix(translate({200.0, 400.0, 50.0}));
                }
                break;
            }
            // case 'l': {
            //     Vector color, corner, uvec, vvec;
            //     cin >> light.position >> light.intensity;
            //     light.intensity = light.intensity/255.0;
            //     lights.push_back(light);
            // } break;
            case 'a': {
                cin >> ambient_light;
                ambient_light = ambient_light/255.0;
            } break;
        }
        type = -1;
    }

    lights.emplace_back(
        Vector({1.0, 1.0, 1.0}),
        Vector({110.0, 110.0, 200.0}),
        Vector({10.0, 0.0, 0.0}),
        Vector({0.0, 10.0, 0.0}),
        8,
        8
    );
    //lights.back().applyMatrix(rotateX(30, true));
    //lights.back().applyMatrix(translate({100.0, 100.0, 150.0}));

    camera->applyMatrix(rotateZ(45, false));
    camera->applyMatrix(rotateX(20, true));
    camera->applyMatrix(rotateZ(45, true));
    camera->render(d, vres, hres, max_depth);
}