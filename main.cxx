#include "graphics.h"
#include <deque>
#include <iostream>
enum keys { SPACE = 32, ENTER = 13, UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75 };

struct Point {
    int r;
    int c;
};

class Map {
    int row;
    int col;
    int cell_size;
public:
    Map(int row_, int col_, int cell_) {
        row = row_;
        col = col_;
        cell_size = cell_;
    }
    void draw() {
        for (int i = 0; i <= col*cell_size; i+=cell_size) {
            line(0, i, cell_size*row, i);
        }
        for (int i = 0; i <= row * cell_size; i += row) {
            line(i, 0,i,cell_size*col);
        }
    }
    int GetCellSize() {
        return cell_size;
    }
    void DrawElem(int row, int col) {
        setfillstyle(1, RED);
        bar(col * cell_size+1, row * cell_size+1, (col+1)*cell_size, (row+1)*cell_size);
    }
};

class Snake {
    std::deque <Point> elems;
    int d;
public:
    Snake(int row, int col){
        elems.push_back({ row,col });
        d = UP;
    }
    Point GetHead() {
        return elems.back();
    }
    int GetD() {
        return d;
    }
};

int main()
{
    const int GAMESPEED = 200;
    initwindow(850, 850, "Snake");
    Map* map = new Map(20, 20, 20); 
    map->draw();
    Snake* snake = new Snake(19, 19);
    map->DrawElem(snake->GetHead().r, snake->GetHead().c);
    while (true)
    {
        if (kbhit()) {
            int code = getch();
            if (code == 0) {
                code = getch();
            }
            std::cout << code << std::endl;
        }
        delay(GAMESPEED);
        //std::cout << snake->GetD();
    }
    return 0;
}