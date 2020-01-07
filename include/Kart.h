#include <math.h>
#include "Math.h"
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <unistd.h>
using namespace std ;
#include <map>
#include <string.h>
#include <stdio.h>
#include <vector>




struct triangle { // permet de transporter les 3 coordonées du triangle plus facilement

	pair<double,double> coord0;
	pair<double,double> coord1;
	pair<double,double> coord2;
};

void read_datas(string name, vector<double> &coords, map <pair<double,double>,float> &mape, vector<double> &extreMome, vector<double> &values);
/**
 		* \brief	Permet de traiter les données 
 		* \details	Va lire le dossier et y extraire les données afin de pouvoir construire la carte
 		* \param	name un string renseignant du nom du fichier de donnée à lire
 		* \param	&coords un vecteur des points
 		* \param	&mape un disctionnaire avec les coordonnées des points sous forme de pair et leur valeur en z
 		* \param	&extreMome un vecteur renseignant des valeurs limites des coordonnées (xmax,xmin,ymax,ymin)
 		* \param	&values un vecteur renseignant des valeurs limites des points (zmin, zmax)
 		* \return	Remplie les différents vecteurs
 		*/


double sign(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y);
/**
 		* \brief	Permet de déterminer les signes entre de la distance entre 2 poins
 		* \details	p3x et p3y étant le point d'origine
 		* \param	double p1x, p1y, p2x, p2y, p3x, p3y	des coordonées des points
 		* \return	Double
 		*/



double extrait_point(triangle trg, double &x0, double &y0, double &x1, double &y1, double &x2, double &y2);
/**
 		* \brief	Permet d'extraire les valeurs stockées dans la structure triangle 
 		* \param	trg une structure tiangle
 		* \param	double x0, y0, x1, y1, x2, y2 
 		* \return	retourne les variables modifées
 		*/



double extrait_valeur_lim(vector<double> &extreMome, vector<double> &values, double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax);
/**
 		* \brief	Permet d'extraire les valeurs stockées dans le vecteur extreMome et values
 		* \param	vecteur extreMome et vecteur values
 		* \param	double xmin, ymin, xmax, ymax, zmin, zmax 
 		* \return	retourne les variables modifées
 		*/


bool in_triangle(double x, double y, triangle trg);
/**
 		* \brief	Permet de déterminer si un point est dans un triangle
 		* \param	double x et y (coordonée d'un point)
 		* \param	triangle trg (coordonnées de 3 points formant un triangle)
 		* \return	retourne True ou False
 		*/


float shaddow(triangle trg,map <pair<double,double>,float> &mape,vector<double> &extreMome, float pixel_x);
/**
 		* \brief	Permet de déterminer l'ombrage d'un point
 		* \param	triangle trg (coordonnées de 3 points formant un triangle)
 		* \param	&mape un disctionnaire avec les coordonnées des points sous forme de pair et leur valeur en z
 		* \param	&extreMome un vecteur renseignant des valeurs limites des coordonnées (xmax,xmin,ymax,ymin)
 		* \param	float pixel_x coordonée en x du point concerné
 		* \return	retourne un float représentant le niveau d'ombrage
 		*/


pair<int,int> find_zone(double x, double y, vector<double> extreMome, int n );
/**
 		* \brief	Permet de déterminer la zone d'un point 
 		* \param	double x et y (coordonée d'un point)
 		* \param	&extreMome un vecteur renseignant des valeurs limites des coordonnées (xmax,xmin,ymax,ymin)
 		* \param	int n qui nous donne le nombre de lignes et colonnes du quadrillage de la zone
 		* \return	retourne un pair indiquant la case de la zone 
 		*/


void segmentation_triangle( vector<double> &coords, vector<double> extreMome, int n, map <pair<int,int>,vector<triangle>> &triangle_sorted);
/**
 		* \brief	Permet de trier les triangles dans des zones plus petites
 		* \details	Nécessaire afin de gagner en rapidité 
 		* \param	coords un vecteur des points
 		* \param	&extreMome un vecteur renseignant des valeurs limites des coordonnées (xmax,xmin,ymax,ymin)
 		* \param	&triangle_sorted un dictionnaire de triangles assignés à une zone
 		* \return	retourne triangle_sorted remplie 
 		*/

