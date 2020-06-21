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
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "roomExplorer.h"
#include "inventory.h"
#include "being.h"

#define MAX_MENTAL_STATE_LEN 30

typedef enum {
  EMindBroken,
  EMindFailing,
  EMindTwitchy,
  EMindNervous,
  EMindPerturbed,
  EMindWorried,
  EMindFine,
  EMindHappy,
  EMindEcstatic,
  EMindDangerouslyEcstatic
} mental_state_e;         ///< player mental state enum

typedef struct mental {
  mental_state_e state;
  int min;
  int max;
  const char str[MAX_MENTAL_STATE_LEN];
} mental_state_t; ///< player mental state struct

/// @brief Class representation of the player
class Player : public Being {
private:

  Inventory *m_inv;          ///< player Inventory
  Item * m_activeItem;       ///< currently used item

  static const int  CMaxHealth            = 100;    ///< max player health
  static const int  CDefaultMentalHealth  = 0;      ///< default mental health
  static const int  CDefaultPlayerDmg     = 20;     ///< default damage
  static const int  CDefaultPlayerDef     = 5;      ///< default defense
  static const int  CDefaultPlayerAcc     = 10;     ///< default accuracy

public:

  Player(Logger *log);
  ~Player();

  /// @brief Return a string describing the player's mental health
  /// @return string
  const char* getMentalHealthStr();

  /// @brief Get player's mental state
  /// @return enum mental_state_t
  mental_state_e getMentalState();

  /// @brief Resotry player's health to max
  /// @param [in] heal the amount to heal player
  /// @note if \a heal is -1, set health to max health
  void restoreHealth(int heal = -1);

  /// @brief Return amount of gold
  /// @return amouint of gold in player's inventory
  int getGold();

  /// @brief Add gold to player's inventory
  void addGold(int gold);

  /// @brief Remove gold from player's inventory
  /// @return true if enough gold was available to subtract, else false
  /// @note if player has less gold than \a gold, nothing happens
  bool subGold(int gold);

  /// @brief Set player's gold to fixed amount
  void setGold(int gold);

  /// @brief Set the player's active item
  /// @return pointer to the active item
  /// @note if \a item is NULL, a random item is chosen from the inventory
  Item* setActiveItem(Item* item = NULL);

  /// @brief Get a pointer to the current active item
  Item * getActiveItem();

  /// @brief Return list of items in inventory
  LiList<Item>* getItems();

  /// @brief Add an item to the player's inventory
  void addItem(Item *item);

  /// @brief Remove an item from the player's inventory
  int loseItem(Item *item);

  /// @brief Use an item from the player's inventory
  /// @return damage dealt
  /// @note if \a item is NULL, the current active item is used
  int useItem(Item *item = NULL);

  /// @brief Return a pointer to a random item from the player's inventory
  Item* getRandomItem();

};

#endif // !_PLAYER_H_
