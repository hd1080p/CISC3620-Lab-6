#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct vec3 {
  float x;
  float y;
  float z;
  vec3(float ix, float iy, float iz) : x(ix), y(iy), z(iz) {}
	vec3() {}
	friend ostream& operator<< (ostream& stream, const vec3& v) {
		stream << v.x << " " << v.y << " " << v.z;
		return stream;
	}
};

struct polygon {
	vector<vec3> vertices;
	friend ostream& operator<< (ostream& s, const polygon& p) {
		for (vec3 v : p.vertices) {
			s << v << endl;
		}
		return s;
	}
};

struct ray {
	vec3 e;
	vec3 d;
	friend ostream& operator<< (ostream& s, const ray& r) {
		s << r.e << " " << r.d;
		return s;
	}
};

void loadPolygons(const string& fname, vector<polygon>* polygons) {
	ifstream f(fname);
	string line;
	polygon o;
	istringstream iss(line);
	while (getline(f, line)) {
		if (line != "begin") {
			cerr << "malformed polygons file: missing polygon begin\n";
		}
		o = polygon();
		while (getline(f, line) && line != "end") {
			float a, b, c;
			iss.str(line); iss.clear();
			iss >> a >> b >> c;
			o.vertices.push_back(vec3(a, b, c));
		}
		if (line != "end") {		// reached eof without closing polygon
			cerr << "malformed polygons file: missing polygon end\n";
		}
		if (o.vertices.size() < 3) {
			cerr << "malformed polygons file: fewer than three vertices in polygon\n";
		}
		polygons->push_back(o);
	}
	f.close();
}

void loadRays(const string& fname, vector<ray>* rays) {
	ifstream f(fname);
	string line;
	ray r;
	istringstream iss(line);
	float a, b, c;
	while (getline(f, line)) {
		r = ray();
		iss.str(line); iss.clear();
		iss >> a >> b >> c;
		r.e = vec3(a, b, c);
		getline(f, line);
		iss.str(line); iss.clear();
		iss >> a >> b >> c;
		r.d = vec3(a, b, c);
		rays->push_back(r);
	}
}

// Lab 6: replace with code to find intersection
pair<bool, float> rayPolygonIntersection(const ray& r, const polygon& p) {
	return make_pair(true, 1.0);
}

// Lab 6: replace loop over rays and polygons to only return closest
// intersection
void checkIntersections(const vector<polygon>& polygons, const vector<ray>& rays, const string& fname) {
	ofstream f;
	f.open(fname);
	for (ray r : rays) {
		f << "Ray " << r << endl;
		bool intersected = false;
		for (polygon p : polygons) {
			pair<bool, float> intersectionResult = rayPolygonIntersection(r, p);
			if (intersectionResult.first) {
				intersected = true;
				f << "intersects with polygon\n" << p << "at t=" 
					<< intersectionResult.second << endl;
			}
		}
		if (!intersected) {
			cout << "does not ever intersect\n";
		}
	}
	f.close();
}

int main() {
	vector<polygon> polygons;
	vector<ray> rays;
	loadPolygons("polygons.out", &polygons);
	loadRays("rays.out", &rays);
	checkIntersections(polygons, rays, "intersections.out");
}
