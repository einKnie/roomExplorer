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
#ifndef _BEING_H_
#define _BEING_H_

#include "roomExplorer.h"

class Being {

protected:
  typedef enum {
    ETypeNone,
    ETypeMonster,
    ETypeFriend,
    ETypePlayer
  } type_t;

  Logger *m_log;        ///< logger
  int     m_health;     ///< health
  int     m_dmg;        ///< damage
  int     m_def;        ///< defense
  int     m_acc;        ///< accuracy

  bool    m_shy;        ///< flag determines whether being will make first contact
  type_t  m_type;       ///< determines whether being is friend or foe to player

public:
  static const int CDefaultHealth     = 100;  ///< default health of all beings
  static const int CDefaultAcc        = 100;
  static const int CDefaultDef        = 100;

  /// @brief default constructor
  /// @param[in] log pointer to a logger object
  Being(Logger *log);

  /// @brief extended constructor
  /// @param[in] log a logger object
  /// @param[in] health the beings starting health
  /// @param[in] damage the beings starting damage
  /// @param[in] shy flag for shyness
  /// @param[in] type the being's type
  Being(Logger *log, int health, int dmg, int def, int acc, bool shy, type_t type);
  ~Being();

  int getHealth();
  int subHealth(int h);
  int addHealth(int h);
  int setHealth(int h);

  int getDamage();
  int subDamage(int d);
  int addDamage(int d);
  int setDamage(int d);

  int getDefense();
  int subDefense(int d);
  int addDefense(int d);
  int setDefense(int d);

  int getAcc();
  int subAcc(int a);
  int addAcc(int a);
  int setAcc(int a);

  int takeDamage(int dmg);

  bool isShy();
  bool isFriend();
  bool isAlive();

};

#endif
