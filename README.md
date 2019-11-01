# wm_desktop_grid
Workspace manager for i3 and sway

## Overview
Wm workspace manager adds a few features not found in i3 and sway. Namely:

* Workspace grid
* Separate workspaces per monitor

It manages a desktop grid using the IPC manager for i3 and sway. It has the option to have shared or separate workspaces for each monitor. Wrapping and the grid dimensions are also customizable in config.h

## Usage
`switch_workspace WM COMMAND ARG`

### WM

Either `i3` or `sway`

###Commands

#### goto


`switch_workspace WM goto WORKSPACE`

Switches to another workspace.
For example to switch to workspace term in i3, run `switch_workspace i3 goto term`
This can be a named non-numerical workspace.

#### moveto

`switch_workspace WM moveto WORKSPACE`

Moves the active window to another workspace and then switches to that workspace. It also can be a named non-numerical workspace.
For example to move the active window to workspace 4 in sway, run `switch_workspace sway moveto 4`

#### switch

`switch_workspace WM switch [up|down|left|right]`

Switches to the workspace up, down, left or right of the active workspace on the grid. If the active workspace it not numerical or outside the grid it does nothing.
For example, if you have a 3x3 grid and are actively in workspace 1, then `switch_workspace i3 switch down` would move you to workspace 4.

### move

`switch_workspace WM move [up|down|left|right]`

Moves the active window to the workspace up, down, left or right of the active workspace on the grid. If the active workspace it not numerical or outside the grid it does nothing.
For example, if you have a 2x2 grid and are currently in workspace 1, then `switch_workspace i3 switch right` would the active window to workspace 2 and switch to workspace 2.

## Installation

### Configuration

The configuration can be found in [config.h](config.h) and is documented there.

### Requirements
* i3 or sway
* g++
* [twmn](https://github.com/sboli/twmn) if you want desktop notifications

### Building

1. `make`
2. `sudo make install` Will install the program to /usr/bin/switch_workspace

## Example Configuration

For i3 config:

    set $goto_workspace switch_workspace i3 goto
    set $move_to_workspace switch_workspace i3 moveto
    set $shift_workspace switch_workspace i3 switch
    set $move_workspace switch_workspace i3 move

For sway config:

    set $goto_workspace switch_workspace sway goto
    set $move_to_workspace switch_workspace sway moveto
    set $shift_workspace switch_workspace sway switch
    set $move_workspace switch_workspace sway move
   
Example config (for both):

    # Switch to workspace
    bindsym $mod+1 exec $goto_workspace 1
    bindsym $mod+2 exec $goto_workspace 2
    bindsym $mod+3 exec $goto_workspace 3
    bindsym $mod+4 exec $goto_workspace 4
    bindsym $mod+5 exec $goto_workspace 5
    bindsym $mod+6 exec $goto_workspace 6
    bindsym $mod+7 exec $goto_workspace 7
    bindsym $mod+8 exec $goto_workspace 8
    bindsym $mod+9 exec $goto_workspace 9
    bindsym $mod+0 exec $goto_workspace 0
    
    # Switch to an adjacent workspace
    bindsym Control+$mod+Left exec $shift_workspace left
    bindsym Control+$mod+Down exec $shift_workspace down
    bindsym Control+$mod+Up exec $shift_workspace up
    bindsym Control+$mod+Right exec $shift_workspace right

    # Move focused container to workspace
    bindsym Shift+$mod+1 exec $move_to_workspace 1
    bindsym Shift+$mod+2 exec $move_to_workspace 2
    bindsym Shift+$mod+3 exec $move_to_workspace 3
    bindsym Shift+$mod+4 exec $move_to_workspace 4
    bindsym Shift+$mod+5 exec $move_to_workspace 5
    bindsym Shift+$mod+6 exec $move_to_workspace 6
    bindsym Shift+$mod+7 exec $move_to_workspace 7
    bindsym Shift+$mod+8 exec $move_to_workspace 8
    bindsym Shift+$mod+9 exec $move_to_workspace 9
    bindsym Shift+$mod+0 exec $move_to_workspace 0
    
    # Move focused container to an adjacent workspace
    bindsym Control+Shift+$mod+Left exec $move_workspace left
    bindsym Control+Shift+$mod+Down exec $move_workspace down
    bindsym Control+Shift+$mod+Up exec $move_workspace up
    bindsym Control+Shift+$mod+Right exec $move_workspace right


## Thanks
Thanks to Baptiste Lepilleur and The JsonCpp Authors for jsoncpp
