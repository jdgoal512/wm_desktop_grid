#include <iostream>
#include "config.h"
#include "grid.h"

/**
 * Gets the workspace to the left
 * 
 * @param currentPos Current workspace number
 * @return position to the left currentPos, -1 if currentPos is invalid
 */
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

/**
 * Gets the workspace to the right
 * 
 * @param currentPos Current workspace number
 * @return position to the right currentPos, -1 if currentPos is invalid
 */
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

/**
 * Gets the workspace to below the current one
 * 
 * @param currentPos Current workspace number
 * @return position the below the current one, -1 if currentPos is invalid
 */
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

/**
 * Gets the workspace to above the current one
 * 
 * @param currentPos Current workspace number
 * @return position the above the current one, -1 if currentPos is invalid
 */
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

/**
 * Gets the workspace in a direction from the current one in the desktop grid
 * 
 * @param currentPos Current workspace number
 * @param direction Direction to move in
 * @return position of the next workspace in the d
 */
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
