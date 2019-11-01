#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "json/json.h"
#include "config.h"
#include "i3.h"

/**
 * Executes a shell command
 * 
 * @param command The command to execute
 * @return stdout of the command
 */
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

/**
 * Gets the active montor name in i3/sway
 * 
 * @param IPCCmd The ipc command (i3-msg for i3, swaymsg for sway)
 * @return monitor name
 */
std::string i3GetActiveMonitor(std::string IPCCmd) {
    std::string jsonOutputs = exec((IPCCmd + " -t get_outputs").c_str());
    const int jsonOutputsLength = static_cast<int>(jsonOutputs.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(jsonOutputs.c_str(), jsonOutputs.c_str() + jsonOutputsLength, &root, &errs)) {
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


/**
 * Gets the active workspace name in i3/sway
 * 
 * @param IPCCmd The ipc command (i3-msg for i3, swaymsg for sway)
 * @return workspace name
 */
int i3GetActiveWorkspace(std::string IPCCmd) {
    std::string monitor = i3GetActiveMonitor(IPCCmd);
    std::string jsonWorkspaces = exec((IPCCmd + " -t get_workspaces").c_str());
    const int jsonWorkspacesLength = static_cast<int>(jsonWorkspaces.length());
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(jsonWorkspaces.c_str(), jsonWorkspaces.c_str() + jsonWorkspacesLength, &root, &errs)) {
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

/**
 * Moves the active window in i3/sway to another workspace
 * 
 * @param IPCCmd The ipc command (i3-msg for i3, swaymsg for sway)
 * @param newWorkspace The workspace to move the active window to
 */
void i3MoveActiveWindow(std::string IPCCmd, std::string newWorkspace) {
    exec((IPCCmd + " move container to workspace " + newWorkspace).c_str());
}

/**
 * Switches to another workspace in i3/sway
 * 
 * @param IPCCmd The ipc command (i3-msg for i3, swaymsg for sway)
 * @param newWorkspace The workspace to switch to
 */
void i3GoToWorkspace(std::string IPCCmd, std::string newWorkspace) {
    exec((IPCCmd + " workspace " + newWorkspace).c_str());
}

/**
 * If SEPARATE_WORKSPACES_PER_MONITOR is set, it prepends the workspace name
 * to the workspace, otherwise just returns the given workspace
 * 
 * @param IPCCmd The ipc command (i3-msg for i3, swaymsg for sway)
 * @param workspace The workspace name to format
 * @return formatted workspace name
 */
std::string i3FormatWorkspace(std::string IPCCmd, std::string workspace) {
    if (SEPARATE_WORKSPACES_PER_MONITOR) {
        return i3GetActiveMonitor(IPCCmd) + ":" + workspace;
    } else {
        return workspace;
    }
}
