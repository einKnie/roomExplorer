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
#ifndef _ITEM_SEED_C_
#define _ITEM_SEED_C_

#include "item.h"

// this file only contains data for items

#ifdef DEBUG
# define ITEM_DEBUG
#endif

#ifdef ITEM_DEBUG

  static const int CMaxRand           = 100;
  static const int CModLikelyhoodBase = 100;
  static const int CModLikelyhoodInc  = 5;
  static const int CMaxMods           = 10;

#else

  static const int CMaxRand           = 100;
  static const int CModLikelyhoodBase = 80;
  static const int CModLikelyhoodInc  = 40;
  static const int CMaxMods           = 2;

#endif

static const int CDefaultHealth       = 100;
static const int CDefaultMood         = 0;

static const int CMaxMoodThresh       = 40;
static const int CMinMoodThresh       = -35;

typedef struct modifier {
  const char name[MAX_ITEM_NAME];
  const int  damage_mod;
  const int  mood_mod;
} modifier_t;

typedef struct itemClass {
  const char name[MAX_ITEM_NAME];
  const int  damage;
  const int  mood;
  const bool plural;
} itemclass_t;

const modifier_t modifiers[] = {
  {"fiery",               10,    0  },
  {"glowing",             5,     10 },
  {"poisonous",           10,   -5  },
  {"shabby",             -10,   -5  },
  {"Connie's",            0,    -50 },
  {"rotten",             -20,   -5  },
  {"shiny",               5,     5  },
  {"red-hot",             15,    0  },
  {"icy",                 10,    0  },
  {"boring",              0,     0  },
  {"void",                15,   -10 },
  {"smokey",              5,    -5  },
  {"light",              -10,    10 },
  {"heavy",               10,   -10 },
  {"irregularly shaped", -5,     0  },
  {"actively burning",    15,   -15 },
  {"wet",                -5,     0  },
  {"giant",               10,    0  },
  {"smelly",              0,    -5  }
};

const itemclass_t itemclasses[] = {
  {"club",               10, CDefaultMood, false},
  {"axe",                12, CDefaultMood, false},
  {"rainbow wand",       17, CDefaultMood, false},
  {"stuffed rat",        5 , CDefaultMood, false},
  {"mirror",             14, CDefaultMood, false},
  {"torch",              12, CDefaultMood, false},
  {"glasses",            7 , CDefaultMood, true },
  {"gloves",             5 , CDefaultMood, true },
  {"loaf of bread",      7 , CDefaultMood, false},
  {"carrot on a string", 9 , CDefaultMood, false},
  {"nfc chip",           3 , CDefaultMood, false},
  {"stereo headphones",  7 , CDefaultMood, true },
  {"tobacco",            3 , CDefaultMood, true },
  {"pouch of water",     4 , CDefaultMood, false},
  {"screwdriver",        10, CDefaultMood, false},
  {"cactus",             15, CDefaultMood, false}
};

static int modSize() {
  return (sizeof(modifiers) / sizeof(modifier_t)) - 1;
}

static int classSize() {
  return (sizeof(itemclasses) / sizeof(itemclass_t)) - 1;
}

#endif
