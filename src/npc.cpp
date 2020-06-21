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
#include "npc.h"

// WIP

Npc::Npc(Logger *log, npcType_e type) : Being(log) {

  m_type    = ETypeFriend;
  m_subType = type;
  m_health  = CNpcDefaultHealth;
  m_dmg     = CNpcDefaultDmg;
  m_def     = CNpcDefaultDef;
  m_acc     = CNpcDefaultAcc;
  m_shy     = true;

  m_log->debug("base NPC created");
}

Npc::~Npc() {
  m_log->debug("Npc destroyed");
}

void Npc::logStats() {
  m_log->debug("NPC stats: health %d, dmg %d, acc %d, def %d", m_health, m_dmg, m_acc, m_def);
}
