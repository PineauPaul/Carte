#include <math.h>

#ifndef __Math_h__
#define __Math_h__

struct triple {
	double first;
	double second;
	double third;
};


void GPS_to_XY(float lx, float ly,float &x, float &y);
/**
 		* \brief	Permet de déterminer la coordonée x, y dans un repère cartésien
 		* \param	float lx et ly latitude longitude
 		* \param	float &x et &y variable corespondant à la coordonnée du point
 		* \return	les variables x et y modifiées
 		*/


float norm(float x1, float x2);
/**
 		* \brief	Permet de déterminer la norme d'un point
 		* \param	float x1, x2
 		* \return	retourne la norme (float) 
 		*/


float angleVec(triple &vec3, triple &vec4);
/**
 		* \brief	Permet de déterminer l'angle entre 2 vecteurs
 		* \param	2 vecteurs vec3 et vec4 en structures triple 
 		* \return	la valeur de l'angle (float)
 		*/


triple produitVec(triple &vec1, triple &vec2);
/**
 		* \brief	Permet de déterminer le vecteur normal à 2 vecteurs
 		* \param    2 vecteurs vec1 et vec2 en structures triple 
 		* \return	le vecteur normal sous une structure triple
 		*/


#endif