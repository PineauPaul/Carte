#include "doctest.h"
#include <unistd.h>
#include "Math.h"
#include <math.h>

TEST_CASE("controle de la fonction qui calcul l'angle")
{// on vérifie que l'angle calculé est le bon en calculant des angles connues
    
    float pi = M_PI; // pour avoir un variable arrondie de pi 
    
    triple vec1;
    vec1.first = 1;

    triple vec2;
    vec2.third = 1;

    CHECK(angleVec( vec1, vec2) == pi/2);

    vec2.first = 1;
    vec2.third = 0;
    vec2.first = -1;

    CHECK(angleVec( vec2, vec1) == -pi);

}


