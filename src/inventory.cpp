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
#include "inventory.h"
#include <sys/ioctl.h>


Inventory::Inventory(Logger *log) {
  m_log = log;
  m_log->debug("creating inventory...");
  m_items = new LiList<Item>();
  m_gold = 0;
  m_mood_effect = 0;
}

Inventory::~Inventory() {
  m_log->debug("destroying inventory...");
  delete m_items;
}

int Inventory::addItem(Item *item) {
  if (!item) return 0;
  if ( m_items->add(item)) {
    m_mood_effect += item->Mood();
    return 1;
  }
  return 0;
}

int Inventory::removeItem(Item *item) {
  if (!item) return 0;
  if (m_items->remove(item)) {
    m_mood_effect -= item->Mood();
    return 1;
  }
  return 0;
}

/// @deprecated
int Inventory::useItem(Item *item) {
  if (!item) {
    // use a random item
  } else {
    // use the specified item
  }
  return 1;
}

LiList<Item>* Inventory::getItems() {
  return m_items;
}

Item* Inventory::getRandomItem() {
  return m_items->get_random();
}

bool Inventory::checkItem(Item *item) {
  if (m_items->find(item) == NULL) return false;
  return true;
}

void Inventory::addGold(int gold) {
  m_gold += gold;
}

bool Inventory::subGold(int gold) {
  if ((m_gold - gold) >= 0) {
    m_gold -= gold;
    return true;
  }
  return false;
}

void Inventory::setGold(int gold) {
  m_gold = gold;
}

int Inventory::getGold() {
  return m_gold;
}

int Inventory::getTotalMoodChange() {
  return m_mood_effect;
}
