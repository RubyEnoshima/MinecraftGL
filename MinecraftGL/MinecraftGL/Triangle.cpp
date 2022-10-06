#include "Triangle.h"

Triangle::Triangle() {

}

Triangle::Triangle(float vertices[NVERT]) {
	for (int i = 0; i < NVERT; i++) {
		_vertices[i] = vertices[i];
	}
}