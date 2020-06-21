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
#include "attack.h"
#include <stdlib.h>

bool operator==(const Attack& lhs, const Attack& rhs) {
  return (lhs.getType() == rhs.getType());
}

const atkTbl_s Attack::m_atkTbl[] = {
  {EAtkNone,        "None",         0},
  {EAtkNibble,      "Nibble",       2},
  {EAtkScuttle,     "Scuttle",      2},
  {EAtkFlutter,     "Flutter",      2},
  {EAtkReflect,     "Reflect",      5},
  {EAtkTinySpores,  "Tiny Spores",  5},
  {EAtkSmallSpores, "Small spores", 8},
  {EAtkSpores,      "Spores",       10},
  {EAtkPsilocybin,  "Psilocybin",   15},
  {EAtkScratch,     "Scratch",      5},
  {EAtkLookCute,    "Look Cute",    10},
  {EAtkConfuse,     "Confuse",      8},
  {EAtkShove,       "Shove",        5},
  {EAtkPunch,       "Punch",        7}
};


Attack::Attack(Logger *log) {
  m_log = log;
  m_type = EAtkNone;
  m_damage = 0;
}

Attack::Attack(Logger *log, attack_e attack) : m_log(log), m_type(attack) {
  m_log->debug("creating attack from type %d", m_type);
  m_damage = m_atkTbl[m_type].dmg;
  memcpy(m_name, m_atkTbl[m_type].name, MAX_ATTACK_NAME_LEN);
}

Attack::~Attack() {
  m_log->debug("destroying attack...");
}

int Attack::use() {
  if (m_type == EAtkNone) return 0;

  // caclulate rare damage produced with attack
  // and return that
  int raw = m_damage;
  double mod = randDmgMod();
  int dmg = (double)raw * mod;

  m_log->debug("use attack %s: raw dmg %d, mod %02.2f, total dmg %d",
                m_name, raw, mod, dmg);

  return dmg;
}

double Attack::randDmgMod(double offset) {
  // return a percentage to multiply with raw dmg
  double mod = ((double)rand() / (double)RAND_MAX) + offset;
  return mod;
}

const attack_e Attack::getType() const {
  return (const attack_e)m_type;
}

const char *Attack::getName() {
  return m_name;
}
