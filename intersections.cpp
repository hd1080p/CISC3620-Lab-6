#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

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

	vec3 operator-(vec3& rhs){
		vec3 temp;
		temp.x = this->x - rhs.x;
		temp.y = this->y - rhs.y;
		temp.z = this->z - rhs.z;
		return temp;
	}
	//Can only perform scalar division on vectors
	vec3 operator/(float val){
		vec3 temp;
		temp.x = this->x/val;
		temp.y = this->y/val;
		temp.z = this->z/val;
		return temp;
	}
	//Scalar multiplication
	vec3 operator*(float val){
		vec3 temp;
		temp.x = this->x * val;
		temp.y = this->y * val;
		temp.z = this->z * val;
		return temp;
	}

	vec3 operator+(vec3& rhs){
		vec3 temp;
		temp.x = this->x + rhs.x;
		temp.y = this->y + rhs.y;
		temp.z = this->z + rhs.z;
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

vec3 crossProduct(vec3& v1, vec3& v2);
vec3 normalize(vec3& v);
float dot(vec3& v1, vec3& v2);


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

vec3 crossProduct(vec3& v1, vec3& v2){
	vec3 cross;
	cross.x = (v1.y * v2.z) - (v1.z * v2.y);
	cross.y = (v1.z * v2.x) - (v1.x * v2.z);
	cross.z = (v1.x * v2.y) - (v1.y * v2.x);
	return cross;
}

vec3 normalize(vec3& v){
	float magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	vec3 normal = v / magnitude;
	return normal;
}

float dot(vec3& v1, vec3& v2){
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// Lab 6: replace with code to find intersection
//To find intersection you need to:
//First find the normal to the polygon
//Second find the intersection value (t)
//t = (p1 - e) dot n / d dot n
//Third: Plug in to point equation p(t) = e + td
pair<bool, float> rayPolygonIntersection(ray& r, polygon& p) {
	//Find the normal:
	vec3 v1= p.vertices[1] - p.vertices[0];
	vec3 v2 = p.vertices[2] - p.vertices[0];
	vec3 xProduct = crossProduct(v1, v2);
	vec3 normal = normalize(xProduct);

	//find T value
	vec3 p1 = p.vertices[0];
	vec3 num = p1 - r.e;
	float t = (dot(num, normal)) / (dot(r.d, normal));
	//Check to see if t iis negative; if it is then it means it is behind the starting point  and there is no intersection.
	if(t < 0)
		return make_pair(false, 0.0);

	//plug into p(t) i.e. the point of intersection
	vec3 wut = r.d * t;
	vec3 p_t = r.e + wut;

	return make_pair(true, t);
}



// Lab 6: replace loop over rays and polygons to only return closest
// intersection
void checkIntersections(vector<polygon>& polygons, vector<ray>& rays, const string& fname) {
	ofstream f;
	f.open(fname);
	pair<bool, float> closestIntersection = rayPolygonIntersection(rays[0], polygons[0]);
	for (ray r : rays) {
		f << "Ray " << r << endl;
		bool intersected = false;
		for (polygon p : polygons) {
			pair<bool, float> intersectionResult = rayPolygonIntersection(r, p);
			
			if (intersectionResult.first) {
				intersected = true;
				f << "Polygon: " << p << endl;
				if(intersectionResult.second < closestIntersection.second)
					closestIntersection = intersectionResult;
			}
		}
		if (!intersected) {
			cout << "does not ever intersect\n";
		}
		f << "Closest intersection for ray: " << closestIntersection.second << endl << endl;
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
