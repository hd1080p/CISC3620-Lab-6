#include <vector>

using namespace std;

struct vec3 {
  float x;
  float y;
  float z;
  vec3(float ix, float iy, float iz) : x(ix), y(iy), z(iz) {}
};

vector<vector<vec3> > objects;
