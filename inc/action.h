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
#ifndef _ACTION_IFC_H_
#define _ACTION_IFC_H_

#include "roomExplorer.h"
#include "inventory.h"
#include "repertoire.h"

class ActionIfc {
private:

  Logger     *m_log;
  Inventory  *m_inv;
  Repertoire *m_atk;

   /*
   the action interface is inherent to every being and is responsible for
   carrying out actions. this may be attack, talk, run, (look?), etc.

   the action interface needs:
    - user stats
    - inventory/learned attacks
   */

public:
  ActionIfc(Logger *log);
  ~ActionIfc();

  int performAttack();
};


 #endif //!_ACTION_IFC_H_
