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

/**
 * Sends a twmn notification
 * 
 * @param workspace Name of the workspace
 * @return 0
 */
int gui_notification(std::string workspace) {
    exec(("twmnc --id 123 -c \"Workspace " + workspace + "\"").c_str());
    return 0;
}

/**
 * Gets the active workspace number
 * 
 * @param wm Wm name (i3 or sway)
 * @return Workspace number, -1 if not a valid number
 */
int getActiveWorkspace(Wm wm) {
    switch (wm) {
        case I3:
            return i3GetActiveWorkspace(I3_IPC);
            break;
        case SWAY:
            return i3GetActiveWorkspace(SWAY_IPC);
            break;
        default:
            return -1;
    }
}

/**
 * Switches to another workspace
 * 
 * @param wm Wm name (i3 or sway)
 * @param workspaceName Name of the workspace to switch to
 * @return 0
 */
int goToWorkspace(Wm wm, std::string workspaceName) {
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, workspaceName);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                if (GUI_NOTIFICATIONS) {
                    gui_notification(workspaceName.c_str());
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

/**
 * Moves the active window to another workspace and then switches
 * to that workspace
 * 
 * @param wm Wm name (i3 or sway)
 * @param workspaceName Name of the workspace to switch to
 * @return 0
 */
int moveToWorkspace(Wm wm, std::string workspaceName) {
    switch (wm) {
        case I3:
        case SWAY:
            {
                std::string IPCCmd = (wm == I3 ? I3_IPC : SWAY_IPC);
                std::string formattedWorkspace = i3FormatWorkspace(IPCCmd, workspaceName);
                i3MoveActiveWindow(IPCCmd, formattedWorkspace);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                if (GUI_NOTIFICATIONS) {
                    gui_notification(workspaceName.c_str());
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

/**
 * Switches to an adjacent workspace
 * 
 * @param wm Wm name (i3 or sway)
 * @param direction Direction to switch to
 * @return 0
 */
int moveToWorkspace(Wm wm, Direction direction) {
    int currentWorkspace = getActiveWorkspace(wm);
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
                i3MoveActiveWindow(IPCCmd, formattedWorkspace);
                i3GoToWorkspace(IPCCmd, formattedWorkspace);
                if (GUI_NOTIFICATIONS) {
                    gui_notification(std::to_string(nextWorkspace).c_str());
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

/**
 * Moves the active window to an adjacent workspace and then switches
 * to that workspace
 * 
 * @param wm Wm name (i3 or sway)
 * @param direction Direction to move window to and switch to
 * @return 0
 */
int shiftToWorkspace(Wm wm, Direction direction) {
    int currentWorkspace = getActiveWorkspace(wm);
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
                if (GUI_NOTIFICATIONS) {
                    gui_notification(std::to_string(nextWorkspace).c_str());
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

/**
 * Converts string to wm enum
 * 
 * @param wm Wm name (string)
 * @return wm (enum), INVALID_WM if invalid string given
 */
Wm getWm(const char* wm) {
    if (strcmp(wm, "i3") == 0) {
        return I3;
    } else if (strcmp(wm, "sway") == 0) {
        return SWAY;
    } else {
        return INVALID_WM;
    }
}

/**
 * Converts string to command enum
 * 
 * @param command Command name (string)
 * @return command (enum), INVALID_CMD if invalid string given
 */
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

/**
 * Converts string to direction enum
 * 
 * @param direction (string)
 * @return direction (enum), INVALID_D if invalid string given
 */
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

/**
 * Prints useage message
 */
void printUsage(std::string cmd) {
    std::cout << "Usage: " << cmd << " WM COMMAND ..." << std::endl;
    std::cout << "  WM:      i3, sway" << std::endl;
    std::cout << "  COMMAND: goto, moveto, switch, move" << std::endl;
    std::cout << std::endl;
    std::cout << "Switch to another workpace:" << std::endl;
    std::cout << "  " << cmd << " WM goto WORKSPACE" << std::endl;
    std::cout << std::endl;
    std::cout << "Move active window to another workspace:" << std::endl;
    std::cout << "  " << cmd << " WM moveto WORKSPACE" << std::endl;
    std::cout << std::endl;
    std::cout << "Switch to an adjacent workspace:" << std::endl;
    std::cout << "  " << cmd << " WM switch [up, down, left, right]" << std::endl;
    std::cout << std::endl;
    std::cout << "Move active window to an adjacent workspace:" << std::endl;
    std::cout << "  " << cmd << " WM move [up, down, left, right]" << std::endl;
}

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 4) {
        printUsage(argv[0]);
        // Don't return an exit code of 1 if "--help" or "-h" flag was passed
        if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
            return 0;
        } else {
            return 1;
        }
    }
    // Get the wm (i3 or sway)
    Wm wm = getWm(argv[1]);
    if (wm == INVALID_WM) {
        printUsage(argv[0]);
        return 1;
    }
    // Get the command (move, switch, goto, or moveto) and execute it
    Command command = getCommand(argv[2]);
    switch (command) {
        case MOVE:
            {
                // Check to make sure the direction is valid
                Direction direction = getDirection(argv[3]);
                if (direction == INVALID_D) {
                    break;
                }
                return moveToWorkspace(wm, direction);
            }
        case SWITCH:
            {
                // Check to make sure the direction is valid
                Direction direction = getDirection(argv[3]);
                if (direction == INVALID_D) {
                    break;
                }
                return shiftToWorkspace(wm, direction);
            }
        case GOTO:
            return goToWorkspace(wm, argv[3]);
        case MOVETO:
            return moveToWorkspace(wm, argv[3]);
        default:
            printUsage(argv[0]);
            return 1;
    }
    printUsage(argv[0]);
    return 1;
}
