#include "LandingPage.h"
#include <fstream>
int main() {
    //Creating a seed for the random number generator using the current time
    //Ran once in the beginning of the program
    srand(time(0)); 
    LandingPage landingPage;
    landingPage.beginProgram();
    return 0; 
}
