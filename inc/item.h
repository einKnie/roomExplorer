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
#ifndef _ITEM_H_
#define _ITEM_H_

#include "roomExplorer.h"

#define MAX_ITEM_NAME 100     ///< max allowed item name length
#define MAX_REPR_LEN  120     ///< max allowed item represent string length
#define MAX_STATE_LEN 20      ///< max allowed state string length
#define HEALTH_MAX    100     ///< max allowed item health
#define POWER_MAX     100     ///< max allowed item damage

typedef enum itemState {
  EBroken = 0,
  ERavaged = 30,
  EDamaged = 50,
  EHealthy = 80,
  EMax = 100,
} item_state_e;   ///< item health state enumeration

/// @brief Class representation of an item
class Item {
private:

  static unsigned int nItems; ///< no of items in game

  static const int CHitChanceCrit = 80;
  static const int CHitChanceFail = 20;

  static const int CFailedHitPenalty = 10;
  static const int CCriticalHitBonus = 10;
  static const int CDefaultItemHealthLoss = 10;

  static const int CGoldPerDamage = 10;

  unsigned int     no;              ///< unique number of this item
  Logger          *m_log;           ///< logger
  int              m_damage;        ///< item damage
  int              m_health;        ///< item health
  int              m_mood_effect;   ///< item's effect on the player's mood
  int              m_value;         ///< item's value for trade

  /// @brief Calculate the item's value
  void calcValue();

  item_state_e getState();

public:
  static const char CItemStateStr[][MAX_STATE_LEN]; ///< item state string table
  char name[MAX_ITEM_NAME];                         ///< item name
  char repr_str[MAX_REPR_LEN];                     ///< item representation string

  /// @brief Random constructor
  /// Creates a randomized item
  Item(Logger *log);

  /// @brief Individual Constructor
  /// @deprecated
  /// Creates an item with predefined name, damage and health
  Item(Logger *log, const char *name, unsigned int damage = 0, unsigned int health = HEALTH_MAX);
  ~Item();

  /// @brief log item name and stats
  void logStats();

  /// @brief return item damage
  unsigned int Damage();

  /// @brief return item health
  unsigned int Health();

  /// @brief return item mood modifier
  int Mood();

  /// @brief return string representation
  const char* represent();

  /// @brief Return the item's value for trade
  int Value();

  /// @brief use item
  /// @todo implement
  int use();

  /// @brief Comparison operator
  friend bool operator== (const Item &lhs, const Item &rhs);
};

#endif //!_ITEM_H_
