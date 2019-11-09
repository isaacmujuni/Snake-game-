#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <curses.h>

using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
bool wrapWalls = false;
const int tickMs = 120;
const int keyEscape = 27;
std::vector<int> tailX(width * height + 1), tailY(width * height + 1);
int nTail;
int x, y, fruitX, fruitY, score;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
int randomCell(int limit) { return std::uniform_int_distribution<int>(0, limit - 1)(rng); }

bool cellOccupied(int cx, int cy)
{
    if (cx == x && cy == y) return true;
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == cx && tailY[i] == cy) return true;
    return false;
}

void placeFruit()
{
    if (nTail + 1 >= width * height)
    {
        fruitX = -1;
        fruitY = -1;
        return;
    }
    do { fruitX = randomCell(width); fruitY = randomCell(height); }
    while (cellOccupied(fruitX, fruitY));
}

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width /2;
    y = height /2;
    nTail = 0;
    placeFruit();
    score = 0;
}
void Draw()
{
    erase();
    for (int i = 0; i < width + 2; i++)
    {
        mvaddch(0, i, '#');
        mvaddch(height + 1, i, '#');
    }

    for (int i = 0; i < height; i++)
    {
        mvaddch(i + 1, 0, '#');
        mvaddch(i + 1, width + 1, '#');
    }

    for (int k = 0; k < nTail; k++)
        mvaddch(tailY[k] + 1, tailX[k] + 1, 'o');
    if (fruitX >= 0)
        mvaddch(fruitY + 1, fruitX + 1, 'F');
    mvaddch(y + 1, x + 1, '0');

    mvprintw(height + 2, 0, "Score: %d", score);
    refresh();
}
void setDirection(eDirection next)
{
    bool reversing = (dir == LEFT && next == RIGHT) || (dir == RIGHT && next == LEFT) ||
                     (dir == UP && next == DOWN) || (dir == DOWN && next == UP);
    if (!reversing)
        dir = next;
}

void Input()
{
    int key;
    while ((key = getch()) != ERR)
    {
        switch (key)
        {
        case 'a':
        case 'A':
        case KEY_LEFT:
            setDirection(LEFT);
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            setDirection(RIGHT);
            break;
        case 'w':
        case 'W':
        case KEY_UP:
            setDirection(UP);
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            setDirection(DOWN);
            break;
        case 'x':
        case 'X':
        case 'q':
        case 'Q':
        case keyEscape:
            gameOver = true;
            break;
        }
    }
}
void Logic ()
{
   int prevX = tailX[0];
   int prevY = tailY[0];
   int prev2X, prev2Y;
   tailX[0] = x;
   tailY[0] = y;
   for ( int i = 1; i < nTail; i++)
   {
       prev2X = tailX[i];
       prev2Y = tailY[i];
       tailX[i] = prevX;
       tailY[i] = prevY;
       prevX = prev2X;
       prevY = prev2Y;
   }
   switch (dir)
{

    case LEFT:
        x--;
    break;
     case RIGHT:
         x++;
    break;
     case UP:
         y--;
    break;
     case DOWN:
         y++;
    break;

    default:
break;
}

    if (!wrapWalls && (x >= width || x < 0 || y >= height || y < 0))
        gameOver = true;
    if (x >= width) x = 0; else if (x < 0) x = width -1;
    if (y >= height) y = 0; else if (y < 0) y = height -1;


    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
        gameOver = true;

if (x == fruitX && y == fruitY)
{
    score +=10;
    nTail++;
    if (nTail > 1)
    {
        tailX[nTail - 1] = prevX;
        tailY[nTail - 1] = prevY;
    }
    placeFruit();
    if (fruitX < 0)
        gameOver = true;
}
}


bool gameOverScreen()
{
    mvprintw(height + 3, 0, "%s  Final score: %d  Play again? (y/n)",
             fruitX < 0 ? "You Win!" : "Game Over!", score);
    refresh();
    nodelay(stdscr, FALSE);
    timeout(-1);
    int key = getch();
    return key == 'y' || key == 'Y';
}

int main()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
#ifdef NCURSES_VERSION
    set_escdelay(25);
#endif
    do {
        nodelay(stdscr, TRUE);
        Setup();
        while (!gameOver)
        {
            Draw();
            Input();
            Logic();
            napms(tickMs);
        }
    } while (gameOverScreen());
    endwin();
    return 0;
}
