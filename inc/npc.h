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
#ifndef _NPC_H_
#define _NPC_H_

// WIP

#include "roomExplorer.h"
#include "being.h"
#include "inventory.h"

class Npc : public Being {
public:

  static const int CNpcDefaultHealth = 100;
  static const int CNpcDefaultDmg    = 20;
  static const int CNpcDefaultDef    = 20;
  static const int CNpcDefaultAcc    = 5;

  typedef enum {
    ENpcTypeNone,
    ENpcTypeHealer,
    ENpcTypeMerchant,
    ECat
  } npcType_e;

  Npc(Logger *log, npcType_e type = ENpcTypeNone);
  ~Npc();

protected:
  Inventory *m_inv;
  npcType_e  m_subType;

  void logStats();

};

#endif //!_NPC_H_
