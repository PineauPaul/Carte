#include <math.h>
#include "WGS84toCartesian.hpp"

struct triple {
	double first;
	double second;
	double third;
};


void GPS_to_XY(float lx, float ly,float &x, float &y){
	// Mercator projection
	std::array<double,2> pos{wgs84::toCartesian({48.394031,-4.957625},{lx,ly})};
	x = pos[0]; // Récupère la coordonnée en x
	y = pos[1]; // Récupère la coordonnée en y
}


float norm(float x1, float x2){ // calcul de la norme d'un point
  return sqrt(pow(x1,2) + pow(x2,2));
}


float angleVec(triple &vec3, triple &vec4){ // calcul l'angle entre 2 vecteurs
	double norm_3 = norm(vec3.first, vec3.third);
	double norm_4 = norm(vec4.first, vec4.third);

	double C = (vec3.first*vec4.first + vec3.third*vec4.third)/(norm_3*norm_4);
	double s = vec3.first*vec4.third - vec3.third*vec4.first;

	return (s/abs(s))*acos(C);
}


triple produitVec(triple &vec1, triple &vec2){ // retourne un vecteur normal aux 2 autres
	triple vec3;
	vec3.first = vec1.second*vec2.third - vec1.third*vec2.second;
	vec3.second = vec1.third*vec2.first - vec1.first*vec2.third;
	vec3.third = vec1.first*vec2.second - vec1.second*vec2.first;
	
	return vec3;
}