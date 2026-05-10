#include "LandingPage.h"
#include <fstream>
int main() {
    srand(time(0)); //Creating a seed for the random number generator using the current time
    LandingPage landingPage;
    landingPage.beginProgram();
    return 0; 
}
