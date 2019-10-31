#ifndef GRID_H
#define GRID_H
enum Direction { LEFT, RIGHT, UP, DOWN, INVALID_D };
int left(int currentPos);
int right(int currentPos);
int down(int currentPos);
int up(int currentPos);
int getNextWorkspace(int currentWorkspace, Direction direction);
#endif
