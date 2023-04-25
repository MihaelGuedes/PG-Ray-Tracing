#include "Camera.cpp"

extern Vector ambient_light;
extern std::vector<Light> lights;
extern std::vector<Material> objects;

using namespace std;

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
                double kd, ks, ka;
                int p;
                cin >> c >> r >> o >> kd >> ks >> ka >> p;
                o = o/255.0;
                objects.emplace_back(new Sphere(c, r), o, kd, ks, ka, p);
                //shapes.back()->applyMatrix(translate({150.0, 0.0, 0.0}));
            } break;
            case 'p': {
                Vector p0(3), n(3), o(3);
                double kd, ks, ka;
                int p;
                cin >> p0 >> n >> o >> kd >> ks >> ka >> p;
                o = o/255.0;
                objects.emplace_back(new Plane(p0, n), o, kd, ks, ka, p);
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
                double kd, ks, ka;
                int p;
                cin >> o >> kd >> ks >> ka >> p;
                o = o/255.0;
                for (auto [i, j, k] : faces) {
                    objects.emplace_back(new Triangle(vertices[i], vertices[j], vertices[k]), o, kd, ks, ka, p);
                    //shapes.back()->applyMatrix(rotateX(40.0, true));
                }
                break;
            }
            case 'l': {
                Light light;
                cin >> light.position >> light.intensity;
                light.intensity = light.intensity/255.0;
                lights.push_back(light);
            } break;
            case 'a': {
                cin >> ambient_light;
                ambient_light = ambient_light/255.0;
            } break;
        }
        type = -1;
    }
    //camera->applyMatrix(rotateZ(30, false));
    camera->render(d, vres, hres);
}