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
#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "roomExplorer.h"

#define MAX_ATTACK_NAME_LEN 40

typedef enum {
  EAtkNone = 0,
  EAtkNibble,
  EAtkScuttle,
  EAtkFlutter,
  EAtkReflect,
  EAtkTinySpores,
  EAtkSmallSpores,
  EAtkSpores,
  EAtkPsilocybin,
  EAtkScratch,
  EAtkLookCute,
  EAtkConfuse,
  EAtkShove,
  EAtkPunch
} attack_e; ///< attack enumeration

typedef struct atk_tbl {
  attack_e type;
  char name[MAX_ATTACK_NAME_LEN];
  int dmg;
} atkTbl_s;

class Attack {
private:

  Logger                 *m_log;
  static const atkTbl_s   m_atkTbl[];
  attack_e                m_type;
  int                     m_damage;
  char                    m_name[MAX_ATTACK_NAME_LEN];

  double randDmgMod(double offset = 0.5);

public:
  Attack(Logger *log);
  Attack(Logger *log, attack_e attack);
  ~Attack();

  /// @brief Use this attack
  /// @return damage produced
  int use();

  /// @brief Return the attack's type
  const attack_e getType() const;

  /// @brief Return the attack's name
  const char *getName();

  /// @brief Comparison operator
  friend bool operator== (const Attack &lhs, const Attack &rhs);
};

 #endif //!_ATTACK_H_
