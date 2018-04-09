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

int LOOKUP_table[256][13];
bool index_reference[256];



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

// pour tracer la droite naive ??? quel intérêt ???
int droiteNaive(int x0,int y0,int x1,int a,int b , int u)
{
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



int calculSommetAllume(int r, int c[3], int x, int y, int z){
    int distance = 0;
    int compteur = 0;
    bool tab[8] = { false,false, false, false, false, false, false, false };

    //  ON CONSIDERE QUE LE CUBE EST 1x1x1, AVEC LE SOMMET 3 EN (0,0,0)
    // parcours en fonction des X
    for (int i = 0; i < 2; i++) {
        // itération des y
        for (int j = 0; j < 2; j++) {
            // itération des z
            for (int k = 0; k < 2; k++) {

                // cast the square-root to an int
                distance = static_cast<int>(sqrt(pow((x + i) - c[0], 2) + pow((y + j) - c[1], 2) + pow((z + k) - c[2], 2))); // on calcule la dist centre_sphere->sommet

                if (distance <= r)                                                                                      // le sommet est dans le sphère
                {
                    switch (compteur)
                    {
                        case 0:                         // 1e sommet (0,0,0)
                            tab[2] = true; break;
                        case 1:                         // 2e sommet (0,0,1)
                            tab[6] = true; break;
                        case 2:                         // 3e sommet (0,1,0)
                            tab[3] = true; break;
                        case 3:                         // 4e sommet (0,1,1)
                            tab[7] = true; break;
                        case 4:                         // 5e sommet (
                            tab[1] = true; break;
                        case 5:
                            tab[2] = true; break;
                        case 6:
                            tab[0] = true; break;
                        case 7:
                            tab[4] = true; break;
                        default: break;
                    }
                }

                compteur++;
            }
        }
    }

    return indexation(tab);
}

/**
 * Calcul de la distance entre le point d'intérêt et le centre de la sphère
 * @param r : rayon de la sphère
 * @param Px : coord X du point
 * @param Py : coord Y du point
 * @param Pz : coord X du point
 * @param Cx : coord X du centre de la sphère
 * @param Cy : coord Y du centre de la sphère
 * @param Cz : coord Z du centre de la sphère
 * @return un vector^3 rempli de 0 (ou pas) si le point est dans la sphère
 */
//vector<int> distance_PointCentre(int r, int Px,int Py,int Pz,int Cx,int Cy,int Cz)
vector<vector<vector<int>>> distance_PointCentre(int r, int Px,int Py,int Pz,int Cx,int Cy,int Cz)
{
    // définition du centre de la sphère
    int Sphere[3] = {Cx, Cy, Cz};
    int dist = 0;

    // définition d'un tableau à 3 vecteurs_dimensions
    vector<vector<vector<int>>> grille;

//    typedef vector<int> v1d;
//    typedef vector<v1d> v2d;
//    typedef vector<v2d> v3d;
//    v3d grille;

    for (int i = 0; i < Px; i++)
    {
        for (int j = 0; j < Py; j++)
        {
            for (int k = 0; k < Pz; k++)
            {
                dist = static_cast<int>(sqrt(pow(i - Sphere[0], 2) + pow(j - Sphere[1], 2) + pow(k - Sphere[2], 2)));

                if (dist <= r) // on est dans le cercle
                {
//                    grille(i, v2d(j, v1d(k, 0)));
//                    grille[i][j][k].push_back(0);
                    grille[i][j][k] = 0;
                }
//                else
//                {
//                    grille.push_back(0);
//                }
            }
        }
    }
    return grille;
}

void Sphere(int LOOKUP_table[256][13])
{
    // paramètre de la sphère
    int centre[3] = {0,0,0}; // 10 x3
    int rayon = 2; // 10
    int dist = 0;

    float x, y, z;
    bool ecrit  = false;
    int compteur = 1;
    int tab_bool[8] = { false,false, false, false, false, false, false, false };

    ofstream fichierV("MarchingCubes_v.txt", ios::out | ios::trunc);
    ofstream fichierF("MarchingCubes_f.txt", ios::out | ios::trunc);

    // Cherche tous les points dans l'espace qui sont allumés
    if(fichierV && fichierF)
    {
        //fichierF << "f";
        for (float i = 0.0; i < 30.0; i++) { // x
            for (float j = 0.0; j < 30.0; j++) { // y
                for (float k = 0.0; k < 30.0; k++) { // z
                    ecrit = false;
                    int ligne = calculSommetAllume(rayon, centre, i, j, k); // Calcule de la ligne dans la lookUpTable en fonction des points allumés

                    for (int l = 0; l < 13; l++){ // Pour chaque valeur de la ligne retrouvé
                        x = 0.0; y = 0.0; z = 0.0;
                        if(LOOKUP_table[ligne][l] != -1 ){
                            switch (LOOKUP_table[ligne][l]) // Pour chaque sommet allumés, je calcule la position dans l'espace 3D
                            {
                                case 0: x = 1.0; y = 0.5;               break;
                                case 1: x = 0.5;                        break;
                                case 2: y = 0.5;                        break;
                                case 3: x = 0.5; y = 1.0;               break;
                                case 4: x = 1.0; y = 0.5; z += 1.0;     break; //       /!\ +=
                                case 5: x = 0.5; z = 1.0;               break;
                                case 6: y = 0.5; z = 1.0;               break;
                                case 7: x = 0.5; y = 1.0; z = 1.0;      break;
                                case 8: x = 1.0; y = 1.0; z = 0.5;      break;
                                case 9: x = 1.0; z = 0.5;               break;
                                case 10: z = 0.5;                       break;
                                case 11: y = 1.0; z = 0.5;              break;
                                default:                                break;
                            }
                            //if (l == 3 || l == 6 || l == 9 || l == 12) { // Nouvelle ligne pour une nouvelle face
                            //	fichierF << " " << lastPoint << "//" << flush; // dernier point
                            //	fichierF << endl;
                            //	fichierF << "f";
                            //}
                            //cout << l << " " << lookUpTable[sommetAllumesCube(i, j, k, rayon, centre)][l] << i << j << k << endl;
                            //lastPoint = lookUpTable[ligne][l];
                            fichierV << "v " << fixed << setprecision(3) << (x + i) / 10 << " " << (y + j) / 10 << " " << (z + k) / 10 << endl;
                            //fichierF << " " << lookUpTable[ligne][l] << "//" << flush; //fichierF << " " << lookUpTable[ligne][l] << "//" << flush;
                            compteur++;
                            ecrit = true;
                        }

                    }
//                    if (sommetAllumesCube(i, j, k, rayon, centre) != 0 && ecrit == true) {
                    //	fichierF << " " << lastPoint << "//" << flush; // dernier point
                    //	fichierF << endl;
                    //	fichierF << "f";
                    //}
                }
            }
        }
    }
    for (int j = 1; j < compteur; j+=3) {
        fichierF << "f " << j << "// " << j+1 << "// " << j+2 << "//" << endl;
    }
}

// ==========================================================================================================================================
//                                                                                                                          MAIN
// ==========================================================================================================================================

int main() {

    // initialisation de la LOOKUP_TABLE (full -1)
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 13; j++) {
            LOOKUP_table[i][j] = -1;
        }
    }

    // initialise tab(bool) à FALSE
    for (int i = 0; i < 256; i++) {
        index_reference[i] = false;
    }


    int tab1[13] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
    bool tab2[13] = { false,false, false, false, false, false, false, false, false, false, false, false, false };

    // création d'un tableau tampon pour sauvegarder les valeurs de la ligne de la config de base sur laquelle on se situe
    bool tab3[8] = {false, false, false, false, false, false, false, false};


    // parcours des configs de base
    for (int iter_config_base_ligne =1; iter_config_base_ligne < 15; iter_config_base_ligne++)
    {
        // parcours du tableau des sommets [24][8]
        for (int iter_sommet_ligne = 0; iter_sommet_ligne < 24; iter_sommet_ligne++)
        {
            // parcours en fonction des colonnes des sommets        CONSTRUCTION TAB3 -> table tampon
            for(int iter_sommet_colonne = 0; iter_sommet_colonne < 8; iter_sommet_colonne++)
            {
                if (TABLEAU_configDeBase[iter_sommet_ligne][iter_sommet_colonne]) // si le sommet est allumé
                {
                    tab3[TABLEAU_sommets[iter_sommet_ligne][iter_sommet_colonne]] = true; // on stock le truc dans la table_auxiliaire
                }
            }

            // parcours en fonction des colonnes du tableau des aretes
            // le but est d'identifier les triangles associé à la config sur laquelle on travaille
            for (int iter_aretes_colonne = 0; iter_aretes_colonne < 12; iter_aretes_colonne++)
            {
                if(TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne] != -1)  // =  on est pas à la fin de la ligne des triangles
                {
                    if(iter_sommet_ligne == 0) // on est sur la 1e ligne du tab_sommets
                    {
                        tab1[iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];
                        tab2[iter_aretes_colonne] = true;

                        // on check que la ligne à l'index_reference calculé sur tab3 est vide
                        // la config n'a pas encore été fate et ajouté à l'index correspondant
                        if (index_reference[indexation(tab3)] == false)
                        {
                            // remplit la LookUp Table à l'index correspondant à la configuration calculée
                            LOOKUP_table[indexation(tab3)][iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];

                            // remplit la config inverse au bon index
                            LOOKUP_table[255 - indexation(tab3)][iter_aretes_colonne] = TABLEAU_triangles[iter_config_base_ligne][iter_aretes_colonne];
                        }

                    }
                    else // on a déjà écrit dans la table_sommets
                    {
//                        for (int iter_arete_colonne = 0; iter_arete_colonne < 12; iter_arete_colonne++) { // parcours des arêtes (une seconde fois)

                        if (TABLEAU_aretes[iter_sommet_ligne - 1][iter_aretes_colonne] == tab1[iter_aretes_colonne] && tab2[iter_aretes_colonne] == true)
                        {
                            tab1[iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes_colonne];
                            tab2[iter_aretes_colonne] = false;

                            if (index_reference[indexation(tab3)] == false)
                            {
                                LOOKUP_table[indexation(tab3)][iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes_colonne];
                                LOOKUP_table[255 - indexation(tab3)][iter_aretes_colonne] = TABLEAU_aretes[iter_sommet_ligne][iter_aretes_colonne];
                            }

//                            }
                        }
                        // INCOMPRIS
                        tab2[iter_aretes_colonne] = true;
                    }

                }
            }
            // on passe la ligne de l'index correspondant à la config traité à true, ce cas a été traité et ne devra plus l'être
            index_reference[indexation(tab3)]=true;

        }
    }
    int rayon, grille_x, grille_y, grille_z, c_x, c_y, c_z;

    // initule car déjà demandé dans la fonction sphere()
//    cout << "Rentrer le rayon de la sphère : " << endl;
//    cin >> rayon;
//    cout << "Rentrer le centre de la sphère : " << endl;
//    cin >> c_x;
//    cin >> c_y;
//    cin >> c_z;
//    cout << "Rentrer la taille de la grille : " << endl;
//    cin >> grille_x;
//    cin >> grille_y;
//    cin >> grille_z;

    // appelle de la fonction pour savoir si le point est compris dans l'espace de la grille
    distance_PointCentre(rayon, grille_x, grille_y, grille_z, c_x, c_y, c_z);

    // écrit toutes les valeurs calculés de la LookUp Table dans des fichiers textes pour la construction de la sphère dans blender
    // Sphere(LOOKUP_table);
    cout << "Notre LookUp Table : " << LOOKUP_table << endl;

    return EXIT_SUCCESS;

//    return 0;
}
