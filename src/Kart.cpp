#include <math.h>
#include "Kart.h"
#include "Math.h"
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <unistd.h>
#include <map>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include "delaunator.hpp"




void read_datas(string name, vector<double> &coords, map <pair<double,double>,float> &mape, vector<double> &extreMome, vector<double> &values){

string filename = "../Datas/" + name;

ifstream f(filename); // tentative d’ouverture (constructeur)

if(!f.is_open())
	cout << "Erreur d’ouverture de " << filename << endl;

else{
	
	vector<double> liste_x; // liste des positions en x
	vector<double> liste_y; // liste des positions en y
	vector<double> liste_z; // liste des valeurs

	string ligne; //Une variable pour stocker les lignes lues

	
	while(getline(f, ligne)){

		const char *seps = " "; // on sépare en fonction de tout les espaces
	    char *token = strtok(&ligne[0],seps);
	   
	    float lx = std::stof(token);
	    token = strtok(NULL,seps);
	    float ly = std::stof(token);
	    token = strtok(NULL,seps);
	    float z = std::stof(token);
		float x = 0.; // initialise les coord
		float y = 0.;
	    GPS_to_XY(lx, ly, x, y); // donne les coord
	    coords.push_back(x);
	    liste_x.push_back(x);

	    coords.push_back(y);
	    liste_y.push_back(y);

	    liste_z.push_back(z);

	    mape[pair<float,float>(x,y)] = z;

	}
	

	extreMome.push_back(*std::min_element(liste_x.begin(), liste_x.end())); // on extrait xmin
	extreMome.push_back(*std::max_element(liste_x.begin(), liste_x.end())); // on extrait xmax
	extreMome.push_back(*std::min_element(liste_y.begin(), liste_y.end())); // on extrait ymin
	extreMome.push_back(*std::max_element(liste_y.begin(), liste_y.end())); // on extrait ymax

	values.push_back(*std::min_element(liste_z.begin(), liste_z.end())); // on extrait zmin
	values.push_back(*std::max_element(liste_z.begin(), liste_z.end())); // on extrait zmax

}

f.close(); 
}

double sign(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y) // afin de savoir le signe entre 2 points 
{
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

double extrait_point(triangle trg, double &x0, double &y0, double &x1, double &y1, double &x2, double &y2){ // récupère les données de la structure triangle
	
	x0 = trg.coord0.first;
	x1 = trg.coord1.first;
	x2 = trg.coord2.first;
	y0 = trg.coord0.second;
	y1 = trg.coord1.second;
	y2 = trg.coord2.second;

}

double extrait_valeur_lim(vector<double> &extreMome, vector<double> &values, double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax){
// récupère les données de extreMome et values
	xmin = extreMome[0];
    xmax = extreMome[1];
    ymin = extreMome[2];
    ymax = extreMome[3];
    zmin = values[0];
    zmax = values[1];

}



bool in_triangle(double x, double y, triangle trg){

	/*verifie si le poit (x,y est dans le triangle d.triangle[i]*/
	double x0,y0,x1,y1,x2,y2;

	extrait_point(trg,x0,y0,x1,y1,x2,y2);

	bool has_neg, has_pos;
	double d1,d2,d3;
	d1 = sign(x,y,x0,y0,x1,y1);
	d2 = sign(x,y,x1,y1,x2,y2);
	d3 = sign(x,y,x2,y2,x0,y0);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);

}

float shaddow(triangle trg,map <pair<double,double>,float> &mape,vector<double> &extreMome, float pixel_x){ // fonction pour déterminer l'ombrage
// on récupère 2 vecteurs définissant notre triangle
	triple vec1;
	vec1.first = trg.coord1.first - trg.coord0.first;
	vec1.second = trg.coord1.second - trg.coord0.second;
	vec1.third = mape[trg.coord1] - mape[trg.coord0];
	
	triple vec2;
	vec2.first = trg.coord2.first - trg.coord0.first;
	vec2.second = trg.coord2.second - trg.coord0.second;
	vec2.third = mape[trg.coord2] - mape[trg.coord0];


// on constuit le vecteur normal au triangle
// on utilise le produit vectoriel
	triple vec3;
	vec3 = produitVec(vec1, vec2);

// on définit mtn le vecteur du soleil
	float x = extreMome[0] - 100; //xmin - 100
	float z = 200;

	triple vec4;
	vec4.first = pixel_x - x;
	vec4.second = 0; // ne nous intérèsse pas 
	vec4.third = (mape[trg.coord0] + mape[trg.coord1] + mape[trg.coord2])/3 - z; 


// on calcule l'angle entre les 2 vecteurs
	float angle = angleVec(vec3, vec4)*180/M_PI;
	//cout << "on trouve un angle de :" << angle << " degrés" << endl;

	float i = 1;

	if (angle < 90 and angle > -90){
		i = 1 + 0.7*(abs(angle) - 90)/90 ; // 0.3 la valeur minimum
	}	// extremum en 0 degré
	

	return i;
}


pair<int,int> find_zone(double x, double y, vector<double> extreMome, int n ){

	double xmin,xmax,ymin,ymax ; // on récupère la grandeur de notre image
    xmin = extreMome[0];
    xmax = extreMome[1];
    ymin = extreMome[2];
    ymax = extreMome[3];

	double pas_x = (xmax - xmin)/n;
	double pas_y = (ymax - ymin)/n;


	int k = (x-xmin)/pas_x; //on récupère son numéro de zone
	int l = (y-ymin)/pas_y;

return pair<int,int> (k,l);
}


void segmentation_triangle( vector<double> &coords, vector<double> extreMome, int n, map <pair<int,int>,vector<triangle>> &triangle_sorted){
// fonction qui permet de séparer les triangles en petit groupe grace à des zones dans le but de limiter les itérations et gagner en temps de calcul

delaunator::Delaunator d(coords); // on recupère les triangles


    triangle triag; 

    pair<int,int> num_zone1;
    pair<int,int> num_zone3;
    pair<int,int> num_zone2;

	
	// on regarde si le triangle est dans la zone
	for(std::size_t m = 0; m < d.triangles.size(); m+=3) { // on parcours la liste des triangles	
		double x0,y0,x1,y1,x2,y2;

		x0 = d.coords[2 * d.triangles[m]]; // on extrait un point
		y0 = d.coords[2 * d.triangles[m] + 1];
		x1 = d.coords[2 * d.triangles[m + 1]];
		y1 = d.coords[2 * d.triangles[m + 1] + 1];
		x2 = d.coords[2 * d.triangles[m + 2]];
		y2 = d.coords[2 * d.triangles[m + 2] + 1];

		triag.coord0 = pair<double,double> (x0,y0);
		triag.coord1 = pair<double,double> (x1,y1);
		triag.coord2 = pair<double,double> (x2,y2);

		num_zone1 = find_zone(x0, y0 , extreMome, n);
		num_zone2 = find_zone(x1, y1 , extreMome, n);
		num_zone3 = find_zone(x2, y2 , extreMome, n);

		int zx1,zx2,zx3,zy1,zy2,zy3,ymin,ymax,xmin,xmax;

		zx1 = num_zone1.first;
		zx2 = num_zone2.first;
		zx3 = num_zone3.first;
		zy1 = num_zone1.second;
		zy2 = num_zone2.second;
		zy3 = num_zone3.second;
		

		//Delaunator crée une enveloppe convexe, on recherche donc les faux triangles
		double carte_x_max, carte_x_min, carte_y_min, carte_y_max, pas_x, pas_y;
		carte_x_max = extreMome[1];
		carte_x_min = extreMome[0];
		carte_y_min = extreMome[2];
		carte_y_max = extreMome[3];
		
		pas_x = (carte_x_max - carte_x_min)/500;
		pas_y = (carte_y_max - carte_y_min)/500;
		


		ymin = min(zy1,min(zy2,zy3));
		xmin = min(zx1,min(zx2,zx3));
		xmax = max(zx1,max(zx2,zx3));
		ymax = max(zy1,max(zy2,zy3));

		for(int i = xmin; i <= xmax ; i ++){
			for(int j = ymin; j <= ymax;j++){
				//si le triangle est trop grand c'est que c'est un faux
				if ((abs(x0-x1)<pas_x) and (abs(x0-x2)<pas_x) and (abs(x2-x1)<pas_x) and (abs(y0-y1)<pas_y) and (abs(y2-y1)<pas_y) and (abs(y0-y2)<pas_y) ){
					triangle_sorted[pair<int,int>(i,j)].push_back(triag); // on ajoute les triangles trouvées et la liste des bornes
				}
				
			}
		}

		
	}	

}
