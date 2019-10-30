#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <json/json.h>
#include "config.h"

enum Wm { I3, SWAY };
enum Command { MOVE, SWITCH };
enum Direction { LEFT, RIGHT, UP, DOWN };

std::string exec(const char* command) {
    char buffer[128];
    std::string result = "";
    FILE* fd = popen(command, "r");
    if (!fd) {
        throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer, sizeof(buffer), fd) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(fd);
        throw;
    }
    pclose(fd);
    return result;
}

std::string i3_get_current_monitor() {
    std::string json_outputs = exec("i3-msg -t get_outputs");
    const int json_outputs_length = static_cast<int>(json_outputs.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(json_outputs.c_str(), json_outputs.c_str() + json_outputs_length, &root, &errs)) {
        for (auto output : root) {
            if (output["active"]) {
                return output["name"].asString();
            }
        }
    } else {
        std::cout << errs << std::endl;
    }
    return "";
}

int i3_get_current_desktop() {
    std::string monitor = i3_get_current_monitor();
    std::string json_workspaces = exec("i3-msg -t get_workspaces");
    const int json_workspaces_length = static_cast<int>(json_workspaces.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(json_workspaces.c_str(), json_workspaces.c_str() + json_workspaces_length, &root, &errs)) {
        for (auto workspace : root) {
            if (strcmp(workspace["output"].asCString(), monitor.c_str()) == 0) {
                if (workspace["focused"].asBool()) {
                    std::string s =  workspace["name"].asString();
                    size_t pos = 0;
                    while ((pos = s.find(":")) != std::string::npos) {
                        s.erase(0, pos + 1);
                    }
                    return stoi(s);
                }
            }
        }
    } else {
        std::cout << errs << std::endl;
    }
    return 0;
}

void i3_to_desktop(Command command, int desktop) {
    std::string new_workspace;
    if (SEPARATE_WORKSPACES_PER_MONITOR) {
        new_workspace = i3_get_current_monitor() + ":" + std::to_string(desktop);
    } else {
        new_workspace = std::to_string(desktop);
    }
    switch (command) {
        case MOVE:
            exec(("i3-msg move container to workspace " + new_workspace).c_str());
            // [{"success":true}]
        case SWITCH:
            exec(("i3-msg workspace " + new_workspace).c_str());
            break;
    }
    return;
}

std::string sway_get_current_monitor() {
    std::string json_outputs = exec("swaymsg -t get_outputs");
    const int json_outputs_length = static_cast<int>(json_outputs.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(json_outputs.c_str(), json_outputs.c_str() + json_outputs_length, &root, &errs)) {
        for (auto output : root) {
            if (output["active"]) {
                return output["name"].asString();
            }
        }
    } else {
        std::cout << errs << std::endl;
    }
    return "";
}

int sway_get_current_desktop() {
    std::string monitor = sway_get_current_monitor();
    std::string json_workspaces = exec("swaymsg -t get_workspaces");
    const int json_workspaces_length = static_cast<int>(json_workspaces.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(json_workspaces.c_str(), json_workspaces.c_str() + json_workspaces_length, &root, &errs)) {
        for (auto workspace : root) {
            if (strcmp(workspace["output"].asCString(), monitor.c_str()) == 0) {
                if (workspace["focused"].asBool()) {
                    std::string s =  workspace["name"].asString();
                    size_t pos = 0;
                    while ((pos = s.find(":")) != std::string::npos) {
                        s.erase(0, pos + 1);
                    }
                    return stoi(s);
                }
            }
        }
    } else {
        std::cout << errs << std::endl;
    }
    return 0;
}

void sway_to_desktop(Command command, int desktop) {
    std::string new_workspace;
    if (SEPARATE_WORKSPACES_PER_MONITOR) {
        new_workspace = sway_get_current_monitor() + ":" + std::to_string(desktop);
    } else {
        new_workspace = std::to_string(desktop);
    }
    switch (command) {
        case MOVE:
            exec(("swaymsg move container to workspace " + new_workspace).c_str());
            // [{"success":true}]
        case SWITCH:
            exec(("swaymsg workspace " + new_workspace).c_str());
            break;
    }
    return;
}

int left(int current_pos) {
    if (current_pos % WIDTH == 1) {
        if (WRAP) {
            return current_pos + WIDTH - 1;
        } else {
            return current_pos;
        }
    } else {
        return current_pos - 1;
    }
}

int right(int current_pos) {
    if (current_pos % WIDTH) {
        return current_pos + 1;
    } else {
        if (WRAP) {
            return current_pos - WIDTH + 1;
        } else {
            return current_pos;
        }
    }
}

int down(int current_pos) {
    if (WRAP) {
        return (current_pos + WIDTH) % (HEIGHT * WIDTH);
    } else {
        if (current_pos > WIDTH*HEIGHT-WIDTH) {
            return current_pos;
        } else {
            return current_pos + WIDTH;
        }
    }
}

int up(int current_pos) {
    if (current_pos <= WIDTH) {
        if (WRAP) {
            return current_pos + WIDTH * HEIGHT - WIDTH;
        } else {
            return current_pos;
        }
    } else {
        return current_pos - WIDTH;
    }
}

int get_next_workspace(Wm wm, Direction direction) {
    int current_desktop = 1;
    switch (wm) {
        case I3:
            current_desktop = i3_get_current_desktop();
            break;
        case SWAY:
            current_desktop = sway_get_current_desktop();
            break;
    }
    std::cout << "Current desktop: " << current_desktop << std::endl;
    switch (direction) {
        case UP:
            return up(current_desktop);
        case DOWN: 
            return down(current_desktop);
        case LEFT:
            return left(current_desktop);
        case RIGHT:
            return right(current_desktop);
    }
    return -1;
}

int gui_notification(int desktop) {
    return 0;
}

int to_next_desktop(Wm wm, Command command, Direction direction) {
    int next_desktop = get_next_workspace(wm, direction);
    std::cout << "Next desktop: " << next_desktop << std::endl;
    switch (wm) {
        case I3:
            i3_to_desktop(command, next_desktop);
            break;
        case SWAY:
            sway_to_desktop(command, next_desktop);
            break;
    }
    return gui_notification(next_desktop);
}

int print_usage(std::string cmd) {
    std::cout << "Usage: " << cmd << " WM COMMAND DIRECTION" << std::endl;
    std::cout << "  WM:        i3, sway" << std::endl;
    std::cout << "  COMMAND:   move, switch" << std::endl;
    std::cout << "  DIRECTION: left, right, up, down" << std::endl;
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    Wm wm;
    if (strcmp(argv[1], "i3") == 0) {
        wm = I3;
    } else if (strcmp(argv[1], "sway") == 0) {
        wm = SWAY;
    } else {
        return print_usage(argv[0]);
    }

    Command command;
    if (strcmp(argv[2], "move") == 0) {
        command = MOVE;
    } else if (strcmp(argv[2], "switch") == 0) {
        command = SWITCH;
    } else {
        return print_usage(argv[0]);
    }

    Direction direction;
    if (strcmp(argv[3], "left") == 0) {
        direction = LEFT;
    } else if (strcmp(argv[3], "right") == 0) {
        direction = RIGHT;
    } else if (strcmp(argv[3], "up") == 0) {
        direction = UP;
    } else if (strcmp(argv[3], "down") == 0) {
        direction = DOWN;
    } else {
        return print_usage(argv[0]);
    }

    return to_next_desktop(wm, command, direction);
}
