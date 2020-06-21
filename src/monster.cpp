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
 *              |_| v0.2
 *****************************************************************************/
#include <stdlib.h>
#include "monster.h"
#include "monster_seed.cpp"

/*
  what does a monster do?
  - fight
    - attack
    - react to attack
    -
*/

Monster::Monster(Logger *log,
                  const char *name,
                  Repertoire *atks,
                  int health,
                  int dmg,
                  int def,
                  int acc,
                  bool shy) : Being(log) {
  m_type = ETypeMonster;

  memcpy(m_name, name, CMonsterNameLen);
  m_attacks = new Repertoire(*atks);
  m_health = health;
  m_dmg = dmg;
  m_def = def;
  m_acc = acc;
  m_shy = shy;

  logStats();
  snprintf(m_repr, CMonsterReprStrLen, "%s %s",
            endsWithPlural(m_name) ? "Some" : startsWithVowel(m_name) ? "An" : "A", m_name);
}

Monster::~Monster() {
  m_log->debug("destroying monster");
  if (m_attacks) {
    m_attacks->deleteAttacks();
    delete m_attacks;
  }
}

void Monster::logStats() {
  m_log->debug("Monster \"%s\", health %d, dmg %d, def %d, acc %d, is %s",
          m_name, m_health, m_dmg, m_def, m_acc, m_shy ? "shy" : "not shy");
}

const char* Monster::represent() {
  return m_repr;
}

const char* Monster::getName() {
  return (const char*)m_name;
}

bool Monster::hasAttacks() {
  return (m_attacks->getAttacksLen() > 0);
}

Attack* Monster::getAttack() {
  return m_attacks->getRandomAttack();
}

int Monster::attack(Attack *atk) {
  if (!atk || !m_attacks->checkAttack(atk)) atk = m_attacks->getRandomAttack();
  if (!atk) return 0;

  m_log->debug("monster attacks");
  int dmg = atk->use();

  // account for dmg && acc
  int dmg_mod = ((double)dmg * ((double)m_dmg / 100));
  dmg += dmg_mod;
  m_log->debug("attack dealt %d damage (%d from dmg stat)", dmg, dmg_mod);

  return dmg;
}
