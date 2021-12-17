#include "graphics.h"
#include <deque>
#include <iostream>
#include <random>
enum keys { SPACE = 32, ENTER = 13, UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75 };

struct Point {
    int r;
    int c;
};

class Tools {
public:
    static bool MatchRC(int r, int c, std::deque <Point>* d) {
        std::deque <Point> ::iterator it;
        for (it = d->begin(); it != d->end(); ++it) {
            if ((*it).r == r && (*it).c == c) {
                return true;
            }
        }
        return false;
    }
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
    Point GetLimits() {
        return { row,col };
    }
    int GetCellSize() {
        return cell_size;
    }
    void DrawElem(int row_, int col_) {
        setfillstyle(1, RED);
        bar(col_ * cell_size+1, row_ * cell_size+1, (col_+1)*cell_size, (row_+1)*cell_size);
    }
    void UndrawElem(int row_, int col_) {
        setfillstyle(1, BLACK);
        bar(col_ * cell_size + 1, row_ * cell_size + 1, (col_ + 1) * cell_size, (row_ + 1) * cell_size);
    }
    void DrawFood(int row, int col, const char* name) {
        readimagefile(name, col*cell_size+1, row*cell_size+1, (col+1) * cell_size-1, (row+1) * cell_size-1);
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

    std::deque <Point>* GetDeque() {
        return &elems;
    }
    Point GetHead() {
        return elems.back();
    }
    Point GetTail() {
        return elems.front();
    }
    int GetD() {
        return d;
    }
    void Step(Map map) {
        int row = elems.back().r;
        int col = elems.back().c;
        switch (d) {
        case UP: 
            row = elems.back().r - 1 < 0 ? map.GetLimits().r-1 : row - 1;
            break;
        case DOWN:
            row = elems.back().r + 1 >= map.GetLimits().r ? 0 : row + 1;
            break;
        case RIGHT:
            col = elems.back().c + 1 >= map.GetLimits().c ? 0 : col + 1;
            break;
        case LEFT:
            col = elems.back().c - 1 < 0 ? map.GetLimits().c-1 : col - 1;
            break;
        default:
            break;
        }
        elems.pop_front();
        elems.push_back({ row,col });
        return;
    }
    void SetD(int d_) {
        d = d_;
        return;
    }
};

class Food {
protected:
    Point coor;
    int value;
    char* filename;
public:
    Point GetCoor() {
        return coor;
    }
    char* GetFilename () {
        return filename;
    }
    void ChangeCoor(int row_limit, int col_limit, std::deque <Point> *busy) {
        do {
            coor.r = rand() % row_limit;
            coor.c = rand() % col_limit;
        } while (Tools::MatchRC(coor.r, coor.c, busy));
    }
};

class Cherry : public Food {
public:
    Cherry(int row, int col, int value, char* filename) {
        coor.r = row;
        coor.c = col;
        this->value = value;
        this->filename = filename;
    }
    std::string GetImage() {
        return this->filename;
    }
};

int main()
{
    bool pause = false;
    srand(time(0));
    const int GAMESPEED = 200;
    initwindow(850, 850, "Snake");
    Map* map = new Map(20, 20, 20); 
    map->draw();
    Snake* snake = new Snake(10, 10);
    map->DrawElem(snake->GetHead().r, snake->GetHead().c);

    Point food_place;
    do {
        food_place.r = rand() % map->GetLimits().r;
        food_place.c = rand() % map->GetLimits().c;
    } while (food_place.r == snake->GetHead().r && food_place.c == snake->GetHead().c);
    Cherry* cherry = new Cherry(food_place.r, food_place.c, 10, "cherry.jpg");
    map->DrawFood(cherry->GetCoor().r, cherry->GetCoor().c, cherry->GetFilename());
    while (true)
    {
        if (kbhit()) {
            int code = getch();
            if (code == 0) {
                code = getch();
                snake->SetD(code);
            }
            else if (code == SPACE) {
                pause = !pause;
            }
        }
        if (pause) {
            delay(100);
            continue;
        }
        map->UndrawElem(snake->GetTail().r, snake->GetTail().c);
        snake->Step(*map);
        Point cherry_p = cherry->GetCoor();
        Point head_p = snake->GetHead();
        map->DrawElem(head_p.r, head_p.c);
        if (cherry_p.r == head_p.r && cherry_p.c == head_p.c) {
            //std::cout << 12;
            cherry->ChangeCoor(map->GetLimits().r, map->GetLimits().c, snake->GetDeque());
            map->DrawFood(cherry->GetCoor().r, cherry->GetCoor().c, cherry->GetFilename());
        }
        //std::cout << 123 << ' ' << cherry->GetCoor().r << ' ' << cherry->GetCoor().c << std::endl;
        delay(GAMESPEED);
        //std::cout << snake->GetD();
    }
    return 0;
}