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

#ifndef _MONSTER_FACTORY_H_
#define _MONSTER_FACTORY_H_

#include "monster.h"

/*
  Monster Factory shall create a random monster.
  monster type may be given, else a random type is used
*/

class MonsterFactory {
private:

  Logger *m_log;

  static const int CMaxRand           = 100;  ///< max rand value
  static const int CContThreshold     = 60;   ///< threshold to continue buffig a monster
  static const int CMaxBuff           = 30;   ///< max value to be gained by buff
  static const int CNerfChance        = 15;   ///< nerf "chance" (should be half of CMaxBuff to remain fair)
  static const int CShyChance         = 40;   ///< chance of monster being shy

  static const int CEpicThreshold     = 40;   ///< theshold of buff to make monster epic
  static const int CScrawnyThreshold  = -40;   ///< threshold of nerf to make monster scrawny

  int getBuff();

public:
  static const int CMaxAttacks        = 7;    ///< max attacks per monster

  MonsterFactory(Logger *log);
  ~MonsterFactory();

  Monster* getMonster(subtype_e type = ESubTypeNone);

};

#endif // _MONSTER_FACTORY_H_
