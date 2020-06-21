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

#include "monster_factory.h"
#include "monster_seed.cpp"

MonsterFactory::MonsterFactory(Logger *log) {
  m_log = log;
}

MonsterFactory::~MonsterFactory() {}

Monster* MonsterFactory::getMonster(subtype_e type) {
  // Create new monster with
  //  * subtype
  //  * attacks
  //  * stats (health, dms, def, acc, ...)
  //  * name
  //  * representation string (might be added by monster)
  m_log->debug("Creating random monster");
  monster_type_t m;
  subtype_e t = type;

  if (t == ESubTypeNone) {
    t = (subtype_e)(rand() % ((int)ESubTypeMax - 1) + 1);
  }

  // get base seed
  switch (t) {
    case ESubTypeTiny:
      m = tiny_monsters[rand() % tinySize()];
      break;
    case ESubTypeSmall:
      m = small_monsters[rand() % smallSize()];
      break;
    case ESubTypeMedium:
      m = medium_monsters[rand() % mediumSize()];
      break;
    case ESubTypeLarge:
      m = large_monsters[rand() % largeSize()];
      break;
    case ESubTypeBoss:
      m = boss_monsters[rand() % bossSize()];
      break;
    case ESubTypeNone: /*fallthrough*/
    default: m_log->error("invalid monster type"); return NULL;
  }

  // buff/nerf
  int buff = getBuff();

  int health = m.baseHealth + (int)(buff * ((double)m.baseHealth / 100));
  m_log->debug("Set monster health to %d (base %d, buff %d%%)", health, m.baseHealth, buff);

  int dmg = m.baseDmg + (int)(buff * ((double)m.baseDmg / 100));
  m_log->debug("Set monster dmg to %d (base %d, buff %d%%)", dmg, m.baseDmg, buff);

  int def = m.baseDef + (int)(buff * ((double)m.baseDef / 100));
  m_log->debug("Set monster def to %d (base %d, buff %d%%)", def, m.baseDef, buff);

  int acc = m.baseAcc + (int)(buff * ((double)m.baseAcc / 100));
  m_log->debug("Set monster acc to %d (base %d, buff %d%%)", acc, m.baseAcc, buff);

  // set name
  char name[Monster::CMonsterNameLen];
  sprintf(name, "%s%s", (buff > CEpicThreshold) ? "Epic " :
                        (buff < CScrawnyThreshold) ? "Scrawny " : "",
                        m.name);
  m_log->debug("Set monster name: %s", name);

  // set shy
  bool isShy = ((rand() % CMaxRand) > CShyChance);
  m_log->debug("Monster is %sshy", isShy ? "" : "not ");

  // set attacks
  Repertoire rep = Repertoire(m_log);
  for (int i = 0; i < CMaxAttacks; i++) {
    m_log->debug(".");
    if (m.attacks[i] == EAtkNone) break;
    if (!rep.addAtk(new Attack(m_log, m.attacks[i]))) {
      m_log->error("Failed to add new attack to monster");
    } else {
      m_log->debug("added attack %d", m.attacks[i]);
    }
  }

  m_log->debug("Prepared all data for new monster");
  return new Monster(m_log, name, &rep, health, dmg, def, acc, isShy);
}

int MonsterFactory::getBuff() {
  int buff = 0;
  int mod = 0;

  // on buff chance, add a buff/nerf
  while ((rand() % CMaxRand) < CContThreshold) {
    mod = (rand() % CMaxBuff) - CNerfChance;
    m_log->debug("monster is being %s: %d", (mod < 0) ? "nerfed" : "buffed", mod);
    buff += mod;
  }

  // total_buff is percentage to add to or subtract from stats
  return buff;
}
