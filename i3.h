#ifndef I3H
#define I3_H
#include <string>

#define I3_IPC "i3-msg"
#define SWAY_IPC "swaymsg"

std::string exec(const char* command);
std::string i3GetCurrentMonitor(std::string IPCCmd);
int i3GetCurrentWorkspace(std::string IPCCmd);
void i3MoveCurrentWindow(std::string IPCCmd, std::string newWorkspace);
void i3GoToWorkspace(std::string IPCCmd, std::string newWorkspace);
std::string i3FormatWorkspace(std::string IPCCmd, std::string workspace);

#endif
