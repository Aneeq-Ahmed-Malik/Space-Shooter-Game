#include "game.h"
int main()
{
    Game *g;
    bool check;
    do
    {
        g = new Game;
        check = g->start_game();
        delete g;
    }while(check);

    return 0;
}
