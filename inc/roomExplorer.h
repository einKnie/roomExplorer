/******************************************************************************
 *   _____
 *  |  __ \
 *  | |__) |___   ___  _ __ ___
 *  |  _  // _ \ / _ \| '_ ` _ \
 *  | | \ \ (_) | (_) | | | | | |
 *  |_|__\_\___/ \___/|_| |_| |_|
 *  |  ____|          | |
 *  | |__  __  ___ __ | | ___  _ __ ___ _ __
 *  |  __| \ \/ / '_ \| |/ _ \| '__/ _ \ '__|
 *  | |____ >  <| |_) | | (_) | | |  __/ |
 *  |______/_/\_\ .__/|_|\___/|_|  \___|_|
 *              | | <einKnie@gmx.at>
 *              |_| v0.1
 *****************************************************************************/
#ifndef _ROOM_EXPLORER_H_
#define _ROOM_EXPLORER_H_

#include "log.h"
#include "helper.h"

#define VERSION_STRING "0.3"
#define AUTHOR_STRING "einKnie"

// #define DEBUG
// #define ONLY_ITEMS

/*
@TODO:
  - monster
    - have atk && def && accuracy
    - can be agressive: attack when entering the room
    - or scared: attack upon finding them by looking or by attacking the (seemingly) empty room
      - this can be cheesed in order to have the first strike
        BUT it will also use your item each time even if no monster is there, so win-lose
  - items get better at higher levels
  - combat system
    - round-based would be best suited

  - item generation:
    - have two (or more) sets of attributes to avoid having nonsensical combinations (eg. heavy light axe)
    - e.g. first tier: simple stuff, little stats gain/loss
          secons tier: better stuff, more stats gain/loss etc...
          (this way, we could aso ensure that ownership attribute is always first for grammar reasons)

*/

#endif
