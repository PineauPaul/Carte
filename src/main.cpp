#include <math.h>
#include "Kart.h"
#include <list>
#include <cstdlib> 
#include <iostream> 
#include <vector>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
#include <map>
#include <fstream>
#include "colourmanager.h"




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


	// definition des variables:
	map <pair<double,double>,float> mape; // mape un disctionnaire avec les coordonnées des points sous forme de pair et leur valeur en z
	vector<double> extreMome; // extreMome un vecteur renseignant des valeurs limites des coordonnées (xmax,xmin,ymax,ymin)
	vector<double> values; // values un vecteur renseignant des valeurs limites des points (zmin, zmax)

	vector<double> coords; // un vecteur des points
	map <pair<int,int>,vector<triangle>> triangle_sorted; // triangle_sorted un dictionnaire de triangles assignés à une zone
	vector<triangle> triangle_de_la_zone;


	//ColourManager gere les couleurs de la carte
	ColourManager::Init_ColourManager();

	string file_name = "carte.pgm";
	read_datas(name,coords,mape,extreMome,values); // on lit les données

    //delaunator::Delaunator d(coords); // on recupère les triangles
	
    int grid = (int)res/2; // nombe de lignes et de colonnes de la découpe en zone
    double resx = res;
    double resy = res;
    float ombrage;

    vector<vector<vector<int>>> *im = new vector<vector<vector<int>>> (resx, vector<vector<int>> (resy,vector<int> (3,255))); // vector contenant pour chaque case les valeurs RGB

    double xmin,xmax,ymin,ymax,zmin,zmax ; 
    extrait_valeur_lim(extreMome, values, xmin, xmax, ymin, ymax, zmin, zmax); 

    double pasx = (xmax - xmin) / resx ; // on définit le pas en x
    double pasy = (ymax - ymin) / resy; // on définit le pas en y

    segmentation_triangle(coords, extreMome,grid,triangle_sorted); // on sépare les triangles dans des zones

    double pixel_x; 
    double pixel_y;
	double moy = 0;

	//On gere les couleurs 

	ColourManager manager(zmin,zmax);
	ColourMap cmap("cmap");

	cmap.addColour(166,206,227,1.0f);
	cmap.addColour(31,120,180,1.0f);
	cmap.addColour(178,223,138,1.0f);
	cmap.addColour(51,160,44,1.0f);
	cmap.addColour(251,154,153,1.0f);
	//cmap.addColour(227,26,28,1.0f);
	cmap.addColour(253,191,111,1.0f);
	cmap.addColour(255,127,0,1.0f);
	//cmap.addColour(202,178,214,1.0f);
	cmap.addColour(106,61,154,1.0f);
	cmap.addColour(255,255,153,1.0f);
    cmap.addColour(177,89,40,1.0f);



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

    		num_zone = find_zone(pixel_x, pixel_y , extreMome, grid); // on récupère le numéro de la zone ou est notre pixel
    		triangle_de_la_zone = triangle_sorted[num_zone]; // on récupère ensuite les triangles concercés (qui sont dans cette zone)
    		
    		for(std::size_t k = 0; k < triangle_de_la_zone.size(); k++) {
			
    			if (in_triangle(pixel_x , pixel_y, triangle_de_la_zone[k])){
    				//determination couleur	

					double h0,h1,h2;
					h0 = mape[triangle_de_la_zone[k].coord0];
					h1 = mape[triangle_de_la_zone[k].coord1];
					h2 = mape[triangle_de_la_zone[k].coord2];

					ombrage = shaddow(triangle_de_la_zone[k], mape, extreMome, pixel_x); // prend en compte l'ombrage

					moy = (h0+h1+h2) * ombrage /3;
					//moy = (h0+h1+h2)/3;
					
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
