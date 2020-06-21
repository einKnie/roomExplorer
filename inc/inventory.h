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
#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include "roomExplorer.h"
#include "item.h"
#include "lilist.h"

/// @brief Inventory class
class Inventory {
private:

  Logger       *m_log;          ///< logger
  LiList<Item> *m_items;        ///< item list
  int           m_gold;         ///< total gold
  int           m_mood_effect;  ///< total mood effect of all items in inventory

public:
  Inventory(Logger *log);
  ~Inventory();

  /// @brief Add an item to the inventory
  /// @param[in] item an item
  /// @return 1 on success, 0 on failure
  int addItem(Item *item);

  /// @brief Remove an item from the inventory
  /// @param[in] item an item
  /// @return 1 on success, 0 on failure
  int removeItem(Item *item);

  /// @brief Use an item
  /// @param[in] item an item or NULL to use a random item
  /// @todo implement or deprecate
  int useItem(Item *item = NULL);

  /// @brief Return a random item from the inventory
  Item* getRandomItem();

  /// @brief Check if item \a item is in inventory
  /// @return true if item is in inventory, false otherwise
  bool checkItem(Item* item);

  /// @brief return the list of items
  LiList<Item>* getItems();

  /// @brief Add gold
  /// @param[in] gold an amount of gold to add
  void addGold(int gold);

  /// @brief Subtract some gold
  /// @param[in] gold the amount of gold to remove
  /// @returns 1 on success, 0 on failure
  bool subGold(int gold);

  /// @brief Set the amount of gold
  /// @param[in] gold the amount of gold to set
  void setGold(int gold);

  /// @brief Return the amount of gold currently held
  /// @return total gold
  int  getGold();

  /// @brief Return total value of mood modifier from items in inventory
  int getTotalMoodChange();

};

#endif
