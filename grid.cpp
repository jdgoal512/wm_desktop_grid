#include <iostream>
#include "config.h"
#include "grid.h"

int left(int currentPos) {
    if (currentPos < 1 || currentPos > HEIGHT * WIDTH) {
        return -1;
    }
    if (currentPos % WIDTH == 1) {
        if (WRAP) {
            return currentPos + WIDTH - 1;
        } else {
            return currentPos;
        }
    } else {
        return currentPos - 1;
    }
}

int right(int currentPos) {
    if (currentPos < 1 || currentPos > HEIGHT * WIDTH) {
        return -1;
    }
    if (currentPos % WIDTH) {
        return currentPos + 1;
    } else {
        if (WRAP) {
            return currentPos - WIDTH + 1;
        } else {
            return currentPos;
        }
    }
}

int down(int currentPos) {
    if (currentPos < 1 || currentPos > HEIGHT * WIDTH) {
        return -1;
    }
    if (WRAP) {
        return (currentPos + WIDTH) % (HEIGHT * WIDTH);
    } else {
        if (currentPos > WIDTH * HEIGHT - WIDTH) {
            return currentPos;
        } else {
            return currentPos + WIDTH;
        }
    }
}

int up(int currentPos) {
    if (currentPos < 1 || currentPos > HEIGHT * WIDTH) {
        return -1;
    }
    if (currentPos <= WIDTH) {
        if (WRAP) {
            return currentPos + WIDTH * HEIGHT - WIDTH;
        } else {
            return currentPos;
        }
    } else {
        return currentPos - WIDTH;
    }
}

int getNextWorkspace(int currentWorkspace, Direction direction) {
    std::cout << "Current workspace: " << currentWorkspace << std::endl;
    switch (direction) {
        case UP:
            return up(currentWorkspace);
        case DOWN: 
            return down(currentWorkspace);
        case LEFT:
            return left(currentWorkspace);
        case RIGHT:
            return right(currentWorkspace);
        default:
            return -1;
    }
}
