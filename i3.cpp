#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "json/json.h"
#include "config.h"
#include "i3.h"

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

std::string i3GetCurrentMonitor(std::string IPCCmd) {
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

int i3GetCurrentWorkspace(std::string IPCCmd) {
    std::string monitor = i3GetCurrentMonitor(IPCCmd);
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

void i3MoveCurrentWindow(std::string IPCCmd, std::string newWorkspace) {
    exec((IPCCmd + " move container to workspace " + newWorkspace).c_str());
}

void i3GoToWorkspace(std::string IPCCmd, std::string newWorkspace) {
    exec((IPCCmd + " workspace " + newWorkspace).c_str());
}

std::string i3FormatWorkspace(std::string IPCCmd, std::string workspace) {
    if (SEPARATE_WORKSPACES_PER_MONITOR) {
        return i3GetCurrentMonitor(IPCCmd) + ":" + workspace;
    } else {
        return workspace;
    }
}
