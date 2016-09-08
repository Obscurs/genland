

// TODO: reference additional headers your program requires here

#include "Game.h"



int main(int argc, char** argv)
{

  {
    Game *g = new Game();
    #pragma omp parallel
    #pragma omp single
    g->Start();
  }
  return 0;
}
