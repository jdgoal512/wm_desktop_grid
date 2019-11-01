#ifndef CONFIG_H
#define CONFIG_H

/** 
 * Desktop grid dimension
 * 
 * For 3x3, the grid would look like the following:
 * +---+---+---+
 * | 1 | 2 | 3 |
 * +---+---+---+
 * | 4 | 5 | 6 |
 * +---+---+---+
 * | 7 | 8 | 9 |
 * +---+---+---+
 */

#define HEIGHT 3
#define WIDTH 3

/**
 * Wrap
 * 
 * If wrap is set, moving towards from an edge will wrap to the other side of the grid
 * Example: 3 is left of 1
 */
#define WRAP 1

/**
 * Separate workspaces
 * 
 * If SEPARATE_WORKSPACES_PER_MONITOR is set, then it will prepend the ouput name to the workspace name
 * Example: 1 => eDP-1:1
 */
#define SEPARATE_WORKSPACES_PER_MONITOR 0

/**
 * Gui notifications through twmn
 * 1 for notifications, 0 for no notifications
 */
#define GUI_NOTIFICATIONS 1

#endif
