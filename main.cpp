#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "json/json.h"
#include "config.h"
#include "grid.h"
#include "i3.h"

enum Wm { I3, SWAY, INVALID_WM };
enum Command { MOVE, SWITCH, GOTO, MOVETO, INVALID_CMD };

int gui_notification(std::string workspace) {
    exec(("twmnc --id 123 -c \"Workspace " + workspace + "\"").c_str());
    return 0;
}

int getCurrentWorkspace(Wm wm) {
    switch (wm) {
        case I3:
            return i3GetCurrentWorkspace(I3_IPC);
            break;
        case SWAY:
            return i3GetCurrentWorkspace(SWAY_IPC);
            break;
        default:
            return -1;
    }
}

int goToWorkspace(Wm wm, std::string workspaceName) {
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, workspaceName);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                gui_notification(workspaceName.c_str());
            }
            break;
        default:
            break;
    }
    return 0;
}

int moveToWorkspace(Wm wm, std::string workspaceName) {
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, workspaceName);
                i3MoveCurrentWindow(IPCCmd, formattedWorkspace);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                gui_notification(workspaceName.c_str());
            }
            break;
        default:
            break;
    }
    return 0;
}

int moveToWorkspace(Wm wm, Direction direction) {
    int currentWorkspace = getCurrentWorkspace(wm);
    int nextWorkspace = getNextWorkspace(currentWorkspace, direction);
    if (nextWorkspace == -1) {
        return 0;
    }
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, std::to_string(nextWorkspace));
                i3MoveCurrentWindow(IPCCmd, formattedWorkspace);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                gui_notification(std::to_string(nextWorkspace).c_str());
            }
            break;
        default:
            break;
    }
    return 0;
}

int shiftToWorkspace(Wm wm, Direction direction) {
    int currentWorkspace = getCurrentWorkspace(wm);
    int nextWorkspace = getNextWorkspace(currentWorkspace, direction);
    if (nextWorkspace == -1) {
        return 0;
    }
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, std::to_string(nextWorkspace));
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                gui_notification(std::to_string(nextWorkspace).c_str());
            }
            break;
        default:
            break;
    }
    return 0;
}

Wm getWm(const char* wm) {
    if (strcmp(wm, "i3") == 0) {
        return I3;
    } else if (strcmp(wm, "sway") == 0) {
        return SWAY;
    } else {
        return INVALID_WM;
    }
}

Command getCommand(const char* command) {
    if (strcmp(command, "move") == 0) {
        return MOVE;
    } else if (strcmp(command, "switch") == 0) {
        return SWITCH;
    } else if (strcmp(command, "goto") == 0) {
        return GOTO;
    } else if (strcmp(command, "moveto") == 0) {
        return MOVETO;
    } else {
        return INVALID_CMD;
    }
}

Direction getDirection(const char* direction) {
    if (strcmp(direction, "left") == 0) {
        return LEFT;
    } else if (strcmp(direction, "right") == 0) {
        return RIGHT;
    } else if (strcmp(direction, "up") == 0) {
        return UP;
    } else if (strcmp(direction, "down") == 0) {
        return DOWN;
    } else {
        return INVALID_D;
    }
}

int printUsage(std::string cmd) {
    std::cout << "Usage: " << cmd << " WM COMMAND ARG" << std::endl;
    std::cout << "  WM:      i3, sway" << std::endl;
    std::cout << "  COMMAND: move, switch, goto, moveto" << std::endl;
    std::cout << "  ARG:     left, right, up, down" << std::endl;
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }

    Wm wm = getWm(argv[1]);
    if (wm == INVALID_WM) {
        return printUsage(argv[0]);
    }

    Command command = getCommand(argv[2]);
    switch (command) {
        case MOVE:
            {
                Direction direction = getDirection(argv[3]);
                if (direction == INVALID_D) {
                    break;
                }
                return moveToWorkspace(wm, direction);
            }
        case SWITCH:
            {
                Direction direction = getDirection(argv[3]);
                if (direction == INVALID_D) {
                    break;
                }
                return shiftToWorkspace(wm, direction);
                // return to_nextDesktop(wm, command, direction);
            }
        case GOTO:
            return goToWorkspace(wm, argv[3]);
        case MOVETO:
            return moveToWorkspace(wm, argv[3]);
        default:
            return printUsage(argv[0]);
    }
    return printUsage(argv[0]);
}
