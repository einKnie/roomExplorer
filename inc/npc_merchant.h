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
#ifndef _NPC_MERCHANT_H_
#define _NPC_MERCHANT_H_

// WIP

#include "npc.h"

class NpcMerchant : public Npc {
private:
  Logger    *m_log;
  Inventory *m_inv;

  static const int CMerchantHealth = 200;
  static const int CMerchantDmg    = 50;
  static const int CMerchantAcc    = 40;
  static const int CMerchantDef    = 8;

  static const int CMerchantInvLen = 4;
  static const int CMerchantGold   = 100;

  int fillInv(unsigned int amount = CMerchantInvLen);

  void logMerch();

public:

  NpcMerchant(Logger *log);
  ~NpcMerchant();

};

#endif // _NPC_MERCHANT_H_
