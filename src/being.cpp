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
#include "being.h"

Being::Being(Logger *log) : Being(log, 0, 0, 0, 0, false, ETypeNone) {}

Being::Being(Logger *log, int health, int dmg, int def, int acc, bool shy, type_t type) {
  m_log = log;

  m_health = health;
  m_dmg = dmg;
  m_def = def;
  m_acc = acc;
  m_shy = shy;
  m_type = type;

  m_log->debug("being created");
}

Being::~Being() {
  m_log->debug("being destroyed");
}

int Being::getHealth() {
  return m_health;
}

int Being::subHealth(int h) {
  m_health -= h;
  if (m_health < 0) {
    m_log->debug("being died");
    m_health = 0;
  }
  return m_health;
}

int Being::addHealth(int h) {
  m_health += h;
  return m_health;
}

int Being::setHealth(int h) {
  m_health = h;
  return m_health;
}

int Being::getDamage() {
  return m_dmg;
}

int Being::subDamage(int d) {
  m_dmg -= d;
  if (m_dmg < 0) {
    m_log->debug("being turned into healer");
  }
  return m_dmg;
}

int Being::addDamage(int d) {
  if ((m_dmg < 0) && ((m_dmg + d) > 0)) {
    m_log->debug("being is no longer a healer");
  }
  m_dmg += d;
  return m_dmg;
}

int Being::setDamage(int d) {
  m_dmg = d;
  return m_dmg;
}

int Being::getDefense() {
  return m_def;
}

int Being::subDefense(int d) {
  m_def -= d;
  if (m_def < 0) {
    m_log->debug("being has become selfdestructive");
  }
  return m_def;
}

int Being::addDefense(int d) {
  if ((m_def < 0) && ((m_def + d) > 0)) {
    m_log->debug("being is no longer selfdestructive");
  }
  m_def += d;
  return m_def;
}

int Being::getAcc() {
  return m_acc;
}

int Being::subAcc(int a) {
  m_acc -= a;
  if (m_acc < 0) {
    m_log->debug("being suffers from hysterical blindness");
  }
  return m_acc;
}

int Being::addAcc(int a) {
  if ((m_acc < 0) && ((m_acc + a) > 0)) {
    m_log->debug("being no longer suffers from hysterical blindness");
  }
  m_acc += a;
  return m_acc;
}

int Being::setAcc(int a) {
  m_acc = a;
  return m_acc;
}

bool Being::isShy() {
  return m_shy;
}

bool Being::isFriend() {
  switch (m_type) {
    case ETypeNone:
    case ETypeFriend: return true;
    case ETypeMonster:
    default: return false;
  }
}

bool Being::isAlive() {
  return (m_health > 0);
}

int Being::takeDamage(int dmg) {
  int totalDmg = dmg - m_def;
  totalDmg = (totalDmg > 0) ? totalDmg : 0;

  subHealth(totalDmg);
  return totalDmg;
}
