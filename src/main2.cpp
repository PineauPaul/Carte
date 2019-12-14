#include <math.h>
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
#include <map>
#include <fstream>
#include "delaunator.hpp"
#include "WGS84toCartesian.hpp"
#include "colourmanager.h"


struct triangle { // permet de transporter les 3 coordonées du triangle plus facilement

	pair<double,double> coord0;
	pair<double,double> coord1;
	pair<double,double> coord2;
};



void create_pgm_BW(const string& file_name)
//Fonction test pour creer une image noir et blanc
{
ofstream f(file_name); // tentative d’ouverture (constructeur)

if(!f.is_open())

	cout << "Erreur d’ouverture de " << file_name << endl;

else{


f << "P2" << endl;
f << 3 << ' ' << 3 << endl;
f << 255 << endl;
f << 255 << ' '<< 0 << ' ' << 255 << endl;
f << 0 << ' '<< 255 << ' ' << 0 << endl;
f << 0 << ' '<< 255 << ' ' << 0 << endl;

f.close(); // fermeture du fichier
}

}



void GPS_to_XY(float lx, float ly,float &x, float &y){
	// Mercator projection
	std::array<double,2> pos{wgs84::toCartesian({48.394031,-4.957625},{lx,ly})};
	x = pos[0]; // Récupère la coordonnée en x
	y = pos[1]; // Récupère la coordonnée en y
	
	
}

void read_datas(string name , vector<double> &v, map <pair<double,double>,float> &mape,vector<double> &extreMome,vector<double> &values){

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
	    GPS_to_XY( lx, ly, x, y); // donne les coord
	    v.push_back(x);
	    liste_x.push_back(x);

	    v.push_back(y);
	    liste_y.push_back(y);

	    liste_z.push_back(z);

	    mape[pair<float,float>(x,y)] = z;

	}
	

	extreMome.push_back(*std::min_element(liste_x.begin(), liste_x.end()));
	extreMome.push_back(*std::max_element(liste_x.begin(), liste_x.end()));
	extreMome.push_back(*std::min_element(liste_y.begin(), liste_y.end()));
	extreMome.push_back(*std::max_element(liste_y.begin(), liste_y.end()));

	values.push_back(*std::min_element(liste_z.begin(), liste_z.end()));
	values.push_back(*std::max_element(liste_z.begin(), liste_z.end()));

}

f.close(); 
}

double sign(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y)
{
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

double extrait_point(triangle trg, double &x0, double &y0, double &x1, double &y1, double &x2, double &y2){
	
	x0 = trg.coord0.first;
	x1 = trg.coord1.first;
	x2 = trg.coord2.first;
	y0 = trg.coord0.second;
	y1 = trg.coord1.second;
	y2 = trg.coord2.second;

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



void segmentation_triangle(delaunator::Delaunator d, vector<double> extreMome, int n, map <pair<int,int>,vector<triangle>> &triangle_sorted){
	

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
		double carte_x_max,carte_x_min,carte_y_min,carte_y_max,pas_x,pas_y;
		carte_x_max = extreMome[1];
		carte_x_min = extreMome[0];
		carte_y_min = extreMome[2];
		carte_y_max = extreMome[3];
		
		pas_x = (carte_x_max - carte_x_min ) / n;
		pas_y = (carte_y_max - carte_y_min) / n;
		


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




int main(int argc,char *argv[]){
	int res = 0;
	string name;
	if (argc >= 3 ) {
		res = atoi(argv[1]);
		name = argv[2];
		
	}
	else{
		cout << "Error, Arguments manquants (nom fichier, taille)"<< endl;
	}
	

	//ColourManager gere les couleurs de la carte
	ColourManager::Init_ColourManager();

	string file_name = "carte.pgm";
	map <pair<double,double>,float> mape; // dico des points 
	vector<double> coords; // liste des points
	vector<double> extreMome; // liste de min, max
	vector<double> values; // valeurs max et min des mesures

	map <pair<int,int>,vector<triangle>> triangle_sorted;
	vector<triangle> triangle_de_la_zone;
	
	read_datas(name,coords,mape,extreMome,values);
    delaunator::Delaunator d(coords);
    cout << d.triangles.size()/6 << endl;
	
    int grid = (int)res/2; // nombe de lignes et de colonnes de la découpe en zone

    double resx = res;
    double resy = res;

    vector<vector<vector<int>>> *im = new vector<vector<vector<int>>> (resx, vector<vector<int>> (resy,vector<int> (3,255))); // vector contenant pour chaque case les valeurs RGB

    double xmin,xmax,ymin,ymax,zmin,zmax ; 
    xmin = extreMome[0];
    xmax = extreMome[1];
    ymin = extreMome[2];
    ymax = extreMome[3];
    zmin = values[0];
    zmax = values[1];


    double pasx = (xmax - xmin) / resx ;
    double pasy = (ymax - ymin) / resy;


    segmentation_triangle(d,extreMome,grid,triangle_sorted);

    double pixel_x;
    double pixel_y;
	double moy = 0;

	//On gere les couleurs 

	ColourManager manager(zmin,zmax);
	ColourMap cmap("cmap");
	cmap.addColour(142,68,173,1.0);
	cmap.addColour(41,128,185,1.0);
	cmap.addColour(46,204,113,1.0);
	cmap.addColour(241,196,15,1.0);
	cmap.addColour(230,126,34,1.0);
	cmap.addColour(235,59,90,1.0);
	ColourManager::setCurrentColourMap(cmap);
	Colour c ;
    
    ofstream f(file_name); // tentative d’ouverture (constructeur)

	if(!f.is_open())

		cout << "Erreur d’ouverture de " << file_name << endl;

	else{
	f << "P6" << endl;
	f << resx << ' ' << resy << endl;
	f << 255 << endl;

	
	pair<int,int> num_zone; 
	

    for(int i = 0; i < resx ;i ++ ){
    	
    	for(int j = 0 ; j<resy ; j++ ) {

    		pixel_x = pasx*j + xmin + pasx/2; 
    		pixel_y = pasy*i + ymin + pasy/2; 

    		num_zone = find_zone(pixel_x, pixel_y , extreMome, grid);
    		triangle_de_la_zone = triangle_sorted[num_zone];
    		
    		for(std::size_t k = 0; k < triangle_de_la_zone.size(); k++) {
			
    			if (in_triangle(pixel_x , pixel_y, triangle_de_la_zone[k])){
    				//determination couleur	

					double h0,h1,h2;
					h0 = mape[triangle_de_la_zone[k].coord0];
					h1 = mape[triangle_de_la_zone[k].coord1];
					h2 = mape[triangle_de_la_zone[k].coord2];

					moy = (h0+h1+h2)/3;
					
					c = manager.getInterpolatedColour((float)moy);

    				(*im)[i][j][0] = (c.getIntR());
					(*im)[i][j][1] = (c.getIntG());
					(*im)[i][j][2] = (c.getIntB());
    				
    		
    			} 
        
    		}
			
    	}
    	
    }
	 //on ecrit dans le fichier carte.pgm
     for(int i = 0; i < resx ;i ++ ){
    	
    	for(int j = 0 ; j<resy ; j++ ) {
			unsigned char r = (*im)[i][j][0];
			unsigned char g = (*im)[i][j][1];
			unsigned char b = (*im)[i][j][2];

    		f << r  << g << b;

    	}
    
    }

    f.close();
   }
}