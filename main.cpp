#include <iostream>
//#include "stdafx.h"
#include "main.h"
#include <windef.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream> // permet d'écrire dans des fichiers textes
#include <iomanip>

using namespace std;


//                4------------------5                           Z
//                |\                 |\                          |
//                | \                | \                      X  |
//                |  7------------------6                      \ |
//                |  |               |  |                       \|
//                |  |               |  |               Y -------0
//                |  |               |  |
//                |  |               |  |
//                0--|---------------1  |
//                 \ |                \ |
//                  \|                 \|
//                   3------------------2
//




// ---------------------------------------------------
//        VARIABLES ET STRUCTURES DE DONNEES
// ---------------------------------------------------
int LOOKUP_table[256][13];

bool index_reference[256];

struct Sphere{
    int rayon;
    int centre_x;
    int centre_y;
    int centre_z;
} sphere;

struct Point{
    float x, y, z;
};

unsigned int grille; // dimension de la grille (cubique)



// ============================================
//              FUNCTIONS
// ============================================


/**
 * fonction pour retourner l'index d'une configuration en fonction des sommets allumés
 * @param array d'une ligne de 8 cases
 * @return l'index de la configuration dans la LOOKUP_table
 */
int indexation(bool tab[8]) {
    int index = 0;
    if (tab[0] == true) index += 1;
    if (tab[1] == true) index += 2;
    if (tab[2] == true) index += 4;
    if (tab[3] == true) index += 8;
    if (tab[4] == true) index += 16;
    if (tab[5] == true) index += 32;
    if (tab[6] == true) index += 64;
    if (tab[7] == true) index += 128;

    return index;
}

/**
 * fonction pour tracer une droite naïve, inutilisé dans l'algo
 * @param
 */
int droiteNaive(int x0,int y0,int x1,int a,int b,int u){
    int r = a*x0-b*y0-u; // équation de la droite naïve
    int x = x0;
    int y = y0;

    while (x<x1)
    {
        x++;
        r = r + a;

        if (r < 0 || r >= b)
        {
            y++;
            r = r - b;
        }

        cout << "Pixel en position" << "(" << x << "," << y << ")" << endl;
        cout << "r = " << r << endl;
    }
}


/**
 * fonction permettant de calculer l'index de chaque configuration précise
 * @param z
 * @return index de la config
 */
int calculIndexConfig(Sphere sphere, Point point){
    int distance = 0;
    int compteur = 0;
    bool target_config[8] = { false,false, false, false, false, false, false, false };

    //  ON CONSIDERE QUE LE CUBE EST 1x1x1, AVEC LE SOMMET 2 EN (0,0,0)

    for (int i = 0; i <= 1; i++) {               // parcours en fonction des X
        for (int j = 0; j <= 1; j++) {           // itération des y
            for (int k = 0; k <= 1; k++) {       // itération des z

                /*
                 * le point renseigné constitue le sommet 2 du cube d'interêt
                 * parcours de chaque sommet de ce cube pour savoir s'il est dans la sphère ou non,
                 * consitution d'une target_config_confirmation en boolean
                 * -> si le sommet est dans la sphère = TRUE
                 * calcul de l'index de la confirmation (pour aller chercher les arêtes affectées dans la LookUp_target_configle)
                 */

                // on calcule la dist centre_sphere->sommet
                // les coordonées des sommets sont calculées en fonction du point d'intérêt et de la disposition du cube dans l'espace
                distance = static_cast<int>(sqrt(
                        pow((point.x + i) - sphere.centre_x, 2)
                        + pow((point.y + j) - sphere.centre_y, 2)
                        + pow((point.z + k) - sphere.centre_z, 2)
                ));

                if (distance <= sphere.rayon)
                {
                    // le sommet est dans le sphère
                    switch (compteur)
                    {
                        case 0: target_config[2] = true; break;   // (0,0,0)    n°2                             Z
                        case 1: target_config[6] = true; break;   // (0,0,1)    n°6                             |
                        case 2: target_config[3] = true; break;   // (0,1,0)    n°3                             |
                        case 3: target_config[7] = true; break;   // (0,1,1)    n°7                         X   |
                        case 4: target_config[1] = true; break;   // (1,0,0)    n°1                          \  |
                        case 5: target_config[2] = true; break;   // (1,0,1)    n°2                           \ |
                        case 6: target_config[0] = true; break;   // (1,1,0)    n°0                            \|
                        case 7: target_config[4] = true; break;   // (1,1,1)    n°4             Y <-------------0
                        default: break;
                    }
                }

                compteur++;
            }
        }
    }

    return indexation(target_config); // on retourne l'index calculé pour la configuration renseignée
}

/**
 * Calcul de la distance entre le point d'intérêt et le centre de la sphère
 * @param r : rayon de la sphère
 * @param Cz : coord Z du centre de la sphère
 * @return un vector3 rempli de 0 (ou pas) si le point est dans la sphère
 */
vector<vector<vector<int>>> grille_INTER_sphere(Sphere sphere, unsigned int grille_dim){
    int dist = 0;

    // définition d'un tableau à 3 vecteurs_dimensions
    vector<vector<vector<int>>> grille;


    // parcours des dimensions de la grille passé en paramètre
    grille.resize(grille_dim);
    for (int i = 0; i < grille_dim; i++)
    {
        grille[i].resize(grille_dim);
        for (int j = 0; j < grille_dim; j++)
        {
            grille[j].resize(grille_dim);
            for (int k = 0; k < grille_dim; k++)
            {
                // calcul de la distance avec le centre de la sphere et le point de la grille
                dist = static_cast<int>(sqrt(pow(i - sphere.centre_x, 2) + pow(j - sphere.centre_y, 2) + pow(k - sphere.centre_z, 2)));

                if (dist <= sphere.rayon) // cela signifie qu'on est dans le cercle
                {
                    grille[i][j].push_back(255); // on affiche le point
                }
                else
                {
                    grille[i][j].push_back(0); // on le laisse en blanc
                }
            }
        }
    }
    return grille;
}

/**
 * fonction permettant de dessiner à partir de la LUT une sphère
 * @param LUT tableau des différentes configurations de triangles utilisé pour faire les faces
 * @param grille_dim la grille utilisé avec ses dimensions (cubique)
 * @param Sphere la sphere avec ses dimensions (rayon, centre...)
 */
void writeFiles(int LUT[256][13], int grille_dim, Sphere sphere){
    Point point{};

    float x, y, z;
    bool ecrit  = false;
    int compteur = 1;
    int tab_bool[8] = { false,false, false, false, false, false, false, false };

    // création du fichier pour stocker toutes les données utiles à Blender
    ofstream sphere_obj_file;
    sphere_obj_file.open("FILE_TO_OPEN_3D.obj");
    sphere_obj_file << "g maSphere" << endl;

    // si le fichier a bien été créé...
    if(sphere_obj_file)
    {
        for (float i = 0.0; i < grille_dim - 1; i++) // x
        {
            for (float j = 0.0; j < grille_dim - 1; j++) // y
            {
                for (float k = 0.0; k < grille_dim - 1; k++) // z
                {
                    ecrit = false;
                    point = {i, j, k};

                    // on appelle la fonction calculIndexConfig pour connaitre quelle ligne de la LUT nous allons avoir besoin
                    int targetIndex = calculIndexConfig(sphere, point);

                    // parcours de chaque colonne pour la LUT
                    for (int col = 0; col < 13; col++)
                    {

                        // ré/initilisation des variables d'espaces x, y et z
                        x = 0.0;
                        y = 0.0;
                        z = 0.0;

                        // La LUT étant initiliasé à -1, dès que cette valeur sera rencontrée cela signifie qu'il n'y a plus de valeurs à prendre dans la LUT
                        if(LUT[targetIndex][col] != -1 )
                        {
                            // la ligne a été remplie || on n'est pas encore à la fin des valeurs intéressantes
                            switch (LUT[targetIndex][col]) // Pour chaque sommet allumés, je calcule la position dans l'espace 3D pour un cube 1x1x1
                            {
                                case 0: x = 1.0;    y = 0.5;                break;
                                case 1: x = 0.5;                            break;
                                case 2:             y = 0.5;                break;
                                case 3: x = 0.5;    y = 1.0;                break;
                                case 4: x = 1.0;    y = 0.5;    z = 1.0;    break;
                                case 5: x = 0.5;                z = 1.0;    break;
                                case 6:             y = 0.5;    z = 1.0;    break;
                                case 7: x = 0.5;    y = 1.0;    z = 1.0;    break;
                                case 8: x = 1.0;    y = 1.0;    z = 0.5;    break;
                                case 9: x = 1.0;                z = 0.5;    break;
                                case 10:                        z = 0.5;    break;
                                case 11:            y = 1.0;    z = 0.5;    break;
                                default:                                    break;
                            }

                            // on écrit dans le fichier les positions des triangles
                            sphere_obj_file << "v " << fixed << setprecision(5) << (x + i) << " " << (y + j) << " " << (z + k) << endl;
                            compteur++;
                            ecrit = true;
                        }

                    }
                }
            }
        }
    }

    // on écrit également le nombre de face le nombre de faces
    for (int j = 1; j < compteur; j+=3)
    {
        sphere_obj_file << "f " << j << " " << j+1 << " " << j+2 << " " << endl;
    }

    // fermeture du fichier
    sphere_obj_file.close();

}


/**
 * fonction permettant d'afficher ce que contient la LUT
 */
void affichageLUT(){
    for(int i = 0; i < 256; i++){
        cout << i << "-> \t";
        for (int j = 0; j < 13; j++){
            cout << LOOKUP_table[i][j] << "\t";
        }
        cout << endl;
    }
}


/**
 * Construction de la LOOK_UP table
 */
void constructionLUT() {

    // parcours des configs de base [15][8]
    for (int iter_config_base_ligne = 0; iter_config_base_ligne < 15; iter_config_base_ligne++)
    {
        int tab_TamponTriangles[13] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        bool tab_TamponLUT[13] = {false, false, false, false, false, false, false, false, false, false, false, false, false};

        // parcours du tableau des sommets [24][8]
        for (int iter_sommet_ligne = 0; iter_sommet_ligne < 24; iter_sommet_ligne++) // état de rotation du cube
        {
            bool tab_TamponConfBase[8] = {false, false, false, false, false, false, false, false};

            // parcours en fonction des colonnes des sommets
            for(int iter_sommet_colonne = 0; iter_sommet_colonne < 8; iter_sommet_colonne++)
            {
                if (TABLEAU_configDeBase[iter_config_base_ligne][iter_sommet_colonne] == true)
                {
                    tab_TamponConfBase[TABLEAU_sommets[iter_sommet_ligne][iter_sommet_colonne]] = true;
                }
            }

            // ici, on a récupéré la config de base en fonction de chaque colonne du TAB_SOMMET
            // parcours en fonction des colonnes du tableau des aretes
            // le but est d'identifier les triangles associés à la config sur laquelle on travaille

            for (int iter_aretes_colonne = 0; iter_aretes_colonne < 12; iter_aretes_colonne++)
            {
                if (TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne] != -1)  // si -1 alors on est pas à la fin de la ligne des triangles
                {
                    if (iter_sommet_ligne < 1) // on est sur la 1ere ligne du tab_sommets
                    {
                        // on stock le n° du triangle à la m ligne & col que dans la table sommet
                        tab_TamponTriangles[iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];

                        // on update, on indique à quelle col la modif vient d'avoir lieu pour que les autres modifs se fassent sur cette même colonne
                        tab_TamponLUT[iter_aretes_colonne] = true;

                        // remplissage de la LUT
                        if (index_reference[indexation(tab_TamponConfBase)] == false)
                        {
                            // indique que la ligne n'a toujours pas été remplie dans la LUT à l'index sur lequel on travaille

                            // remplit la LookUp Table à l'index correspondant à la configuration calculée
                            LOOKUP_table[indexation(tab_TamponConfBase)][iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];

                            // remplit la config inverse au bon index
                            LOOKUP_table[255 - indexation(tab_TamponConfBase)][iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];
                        }

                    }
                    else
                    {
                        for (int iter_aretes = 0; iter_aretes < 12; iter_aretes++) // parcours des arêtes (une seconde fois), même démarche que précédemment
                        {
                            if (TABLEAU_aretes[iter_sommet_ligne - 1][iter_aretes] == tab_TamponTriangles[iter_aretes_colonne] && tab_TamponLUT[iter_aretes_colonne])
                            {
                                tab_TamponTriangles[iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes];
                                tab_TamponLUT[iter_aretes_colonne] = false;

                                if (!index_reference[indexation(tab_TamponConfBase)])
                                {
                                    LOOKUP_table[indexation(tab_TamponConfBase)][iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes];
                                    LOOKUP_table[255 - indexation(tab_TamponConfBase)][iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes];
                                }
                            }
                        }
                        tab_TamponLUT[iter_aretes_colonne] = true;
                    }
                }
            }
            // on passe la ligne de l'index correspondant à la config traité à true, ce cas a été traité et ne devra plus l'être
            index_reference[indexation(tab_TamponConfBase)] = true;
        }
    }
}

// ==========================================================================================================================================
//                             MAIN
// ==========================================================================================================================================

int main() {

    // ________________________________________________________________________________________________
    //                              INITIALISATION DES VARIABLES

    // initialiation de la LOOKUP_TABLE à -1
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 13; j++) {
            LOOKUP_table[i][j] = -1;
        }
    }

    // initialisation de l'Index_reference à false
    for (int i = 0; i < 256; i++) {
        index_reference[i] = false;
    }

    // déclaration de la taille de la sphere
    sphere = {30, 50, 50, 50};

    // creation de la grille (cubique)
    grille = 100;


    // ________________________________________________________________________________________________
    //                              CREATION LUT + JEU DE DONNEE

    constructionLUT();

    // TEST
    // cout << "sphere vs p(4,4,4): " << calculIndexConfig(sphere, {4, 4, 4}) << endl; // = 223

    // grille_INTER_sphere(sphere, grille);

    writeFiles(LOOKUP_table, grille, sphere);

    // affichageLUT();

    cout << "PROCESS FINALIZED SUCCESSFULLY";

    return EXIT_SUCCESS;

}
