# wm_desktop_grid
Desktop grid manager for i3 and sway

Manages a desktop grid using the ipc-manager for i3 and sway. It has the option to have shared or separate workspaces for each monitor. Wrapping and the grid dimensions are also customizable in config.h

Usage:
  ./switch_workspace [i3|sway] [move|switch] [up|down|left|right]

Example sway config:
set $move_workspace   ~/bin/switch_desktop sway move
set $switch_workspace ~/bin/switch_desktop sway switch

bindsym Control+Mod1+Up    exec $switch_workspace up
bindsym Control+Mod1+Down  exec $switch_workspace down
bindsym Control+Mod1+Left  exec $switch_workspace left
bindsym Control+Mod1+Right exec $switch_workspace right

bindsym Control+Shift+Mod1+Up    exec $move_workspace up
bindsym Control+Shift+Mod1+Down  exec $move_workspace down
bindsym Control+Shift+Mod1+Left  exec $move_workspace left
bindsym Control+Shift+Mod1+Right exec $move_workspace right

Example i3 config:
set $move_workspace   ~/bin/switch_desktop i3 move
set $switch_workspace ~/bin/switch_desktop i3 switch

bindsym Control+Mod1+Up    exec $switch_workspace up
bindsym Control+Mod1+Down  exec $switch_workspace down
bindsym Control+Mod1+Left  exec $switch_workspace left
bindsym Control+Mod1+Right exec $switch_workspace right

bindsym Control+Shift+Mod1+Up    exec $move_workspace up
bindsym Control+Shift+Mod1+Down  exec $move_workspace down
bindsym Control+Shift+Mod1+Left  exec $move_workspace left
bindsym Control+Shift+Mod1+Right exec $move_workspace right


Thanks to Baptiste Lepilleur and The JsonCpp Authors for jsoncpp
