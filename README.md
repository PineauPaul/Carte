# Projet Modèle Numérique de Terrain 

Robotique/UE 3.1  -  Projet C++ -  Décembre 2019

Supports de cours disponibles sur
www.simon-rohou.fr/cours/c++

# Objectif du projet:

L’objectif  de  ce  projet  est  de  gagner  en  autonomie  et  de  proposer  une  solution  pour  la  génération  d’une image 2D representant, par niveaux de couleurs, les différentes altitudes d’un terrain 2.5D. Un Modèle Numérique de Terrain (MNT) du lac de Guerlédan vous est fourni pour ce projet.

# Modélisation

Le  modèle  numérique  de  terrain  est  dit  en "2,5  dimen-sions"puisqu’il  associe  une  seule  altitude  à  une  position  horizontale donnée.

Le Maillage:

	_ Un rendu de MNT est une image pour laquelle chaque pixel correspond à une zone carrée dans la projec-tion choisie. Pour cela on utilise un algorithme de triangulation de Delaunay.

Format d’image:

Le  rendu  est  généré  par  votre  exécutable  avec  l’ ́ecriture  d’un  fichier  de  données.

# Compilation

executer le script build.sh
bash ./build.sh
