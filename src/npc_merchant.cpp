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
#include "npc_merchant.h"

// WIP

NpcMerchant::NpcMerchant(Logger *log) : Npc(log) {
  // create base merchant
  m_subType = ENpcTypeMerchant;
  m_health  = CMerchantHealth;
  m_dmg = CMerchantDmg;
  m_acc = CMerchantAcc;
  m_def = CMerchantDef;
  m_log = log;
  m_inv = new Inventory(m_log);

  if (!fillInv()) {
    m_log->error("Failed to fill merchant inventory");
  }
  m_inv->setGold(CMerchantGold);

  m_log->debug("Created Merchant NPC");
  logMerch();
}

NpcMerchant::~NpcMerchant() {
  delete m_inv;

  m_log->debug("Destroyed Merchant NPC");
}

int NpcMerchant::fillInv(unsigned int amount) {
  for (unsigned int i = 0; i < amount; i++) {
    if (!m_inv->addItem(new Item(m_log))) break;
  }

  if (m_inv->getItems()->Size() != amount)
    return 0;

  return 1;
}

void NpcMerchant::logMerch() {
  logStats();
  m_log->debug("%d items, %d gold", m_inv->getItems()->Size(), m_inv->getGold());
}
