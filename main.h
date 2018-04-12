#include <windef.h>

// table de rotation des sommets
static int TABLEAU_sommets[24][8]{
        {0,1,2,3,4,5,6,7},
        {3,0,1,2,7,4,5,6},
        {2,3,0,1,6,7,4,5},
        {1,2,3,0,5,6,7,4},
        {3,2,6,7,0,1,5,4},
        {7,6,5,4,3,2,1,0},
        {4,5,1,0,7,6,2,3},
        {4,0,3,7,5,1,2,6},
        {5,4,7,6,1,0,3,2},
        {1,5,6,2,0,4,7,3},
        {5,6,2,1,4,7,3,0},
        {7,3,2,6,4,0,1,5},
        {7,4,0,3,6,5,1,2},
        {2,6,7,3,1,5,4,0},
        {0,4,5,1,3,7,6,2},
        {0,3,7,4,1,2,6,5},
        {5,1,0,4,6,2,3,7},
        {2,1,5,6,3,0,4,7},
        {6,2,1,5,7,3,0,4},
        {1,5,6,2,0,4,7,3},
        {3,2,6,7,0,1,5,4},
        {6,7,3,2,5,4,0,1},
        {4,7,6,5,0,3,2,1},
        {5,4,7,6,1,0,3,2}
};

// table de rotation des ar�tes
static int TABLEAU_aretes[24][12]{
        {0,1,2,3,4,5,6,7,8,9,10,11},
        {3,0,1,2,7,4,5,6,11,8,9,10},
        {2,3,0,1,6,7,4,5,10,11,8,9},
        {1,2,3,0,5,6,7,4,9,10,11,8},
        {2,10,6,11,0,9,4,8,3,1,5,7},
        {6,5,4,7,2,1,0,3,11,10,9,8},
        {4,9,0,8,6,10,2,11,7,5,1,3},
        {8,3,11,7,9,1,10,5,4,0,2,6},
        {4,7,6,5,0,3,2,1,9,8,11,10},
        {9,5,10,1,8,7,11,3,0,4,6,2},
        {5,10,1,9,7,11,3,8,4,6,2,0},
        {11,2,10,6,8,0,9,4,7,3,1,5},
        {7,8,3,11,5,9,1,10,6,4,0,2},
        {10,6,11,2,9,4,8,0,1,5,7,3},
        {8,4,9,0,11,6,10,2,3,7,5,1},
        {3,11,7,8,1,10,5,9,0,2,6,4},
        {9,0,8,4,10,2,11,6,5,1,3,7},
        {1,9,5,10,3,8,7,11,2,0,4,6},
        {10,1,9,5,11,3,8,7,6,2,0,4},
        {11,7,8,3,10,5,9,1,2,6,4,0},
        {0,8,4,9,2,11,6,10,1,3,7,5},
        {6,11,2,10,4,8,0,9,5,7,3,1},
        {5,4,7,6,1,0,3,2,10,9,8,11},
        {7,6,5,4,3,2,1,0,8,11,10,9}
};

// Table avec identification du sommet allum�
bool TABLEAU_configDeBase [15][8]{
        { false,	false,	false,	false,	false,	false,	false,	false },
        { false,	false,	false,	true,	false,	false,	false,	false },
        { false,	false,	true,	true,	false,	false,	false,	false },
        { false,	false,	false,	true,	false,	false,	true,	false },
        { true,		true,	true,	false,	false,	false,	false,	false },
        { true,		true,	true,	true,	false,	false,	false,	false },
        { true,		true,	true,	false,	false,	false,	false,	true },
        { false,	true,	false,	true,	true,	false,	true,	false },
        { true,		true,	false,	true,	true,	false,	false,	false },
        { true,		true,	true,	false,	true,	false,	false,	false },
        { false,	false,	false,	true,	false,	true,	false,	false },
        { false,	false,	true,	true,	false,	true,	false,	false },
        { false,	false,	true,	false,	false,	true,	false,	true },
        { false,	true,	false,	true,	false,	true,	false,	true },
        { true,	    true,	false,	true,	false,	true,	false,	false }
};

// table des configurations de triangles possible
static int TABLEAU_triangles[15][13]{
        { -1,   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { 3,    2,  11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { 3,    1,  10, 11, 3,  10, -1, -1, -1, -1, -1, -1, -1 },
        { 3,    2,  11, 6,  5,  10, -1, -1, -1, -1, -1, -1, -1 },
        { 2,    3,  10, 3,  8,  10, 8,  9,  10, -1, -1, -1, -1 },
        { 11,   10, 9,  11, 8,  9,  -1, -1, -1, -1, -1, -1, -1 },
        { 11,   6,  7,  2,  3,  10, 3,  8,  10, 8,  9,  10, -1 },
        { 3,    2,  11, 8,  7,  4,  6,  5,  10, 9,  1,  0,  -1 },
        { 7,    4,  11, 11, 4,  2,  4,  9,  2,  2,  9,  1,  -1 },
        { 3,    7,  2,  7,  4,  9,  7,  9,  2,  0,  2,  9,  -1 },
        { 3,    2,  11, 4,  5,  9,  -1, -1, -1, -1, -1, -1, -1 },
        { 3,    1,  10, 11, 3,  10, 4,  5,  9,  -1, -1, -1, -1 },
        { 4,    5,  9,  7,  6,  11, 10, 2,  1,  -1, -1, -1, -1 },
        { 3,    7,  6,  3,  2,  6,  4,  0,  5,  0,  1,  5,  -1 },
        { 8,    4,  5,  2,  1,  5,  2,  8,  5,  2,  8,  11, -1 }
};