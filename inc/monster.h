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

#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "being.h"
#include "repertoire.h"

typedef enum {
  ESubTypeNone = 0,
  ESubTypeTiny,
  ESubTypeSmall,
  ESubTypeMedium,
  ESubTypeLarge,
  ESubTypeBoss,
  ESubTypeMax
} subtype_e;    ///< monster subtype

class Monster: public Being {
public:

  static const int CMonsterNameLen    = 80;   ///< max strlen of monster name
  static const int CMonsterReprStrLen = 100;  ///< max strlen of monster representation string

  Monster(Logger *log, const char *name, Repertoire *atks, int health, int dmg, int def, int acc, bool shy);
  ~Monster();

  /// @brief Get raw monster name
  /// @return the monster's name
  const char* getName();

  /// @brief Get monster representation string
  /// @return full monster name
  const char* represent();

  /// @brief Check if monster has any attacks
  /// @return true if monster has at least one attack, else false
  bool hasAttacks();

  /// @brief Get a random attack
  /// @returns one of hte monster's attacks
  Attack *getAttack();

  /// @brief Simulate a monster attack
  /// @atk a specific attack, or NULL to use a random attack from the monster's repertoire
  /// @return produced damage output
  /// @note if an attack is given that is not in the monster's repertoire, a random attack is used
  int attack(Attack *atk = NULL);

private:
  Repertoire *m_attacks;                  ///< monster repertoire
  char        m_name[CMonsterNameLen];    ///< monster name
  char        m_repr[CMonsterReprStrLen]; ///< monster representation string

  /// @brief log monster stats
  void logStats();

};

#endif
