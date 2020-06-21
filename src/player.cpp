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
#include "player.h"

const mental_state_t CMentalStates[] = {
  {EMindBroken,               -500, -100, "broken"},
  {EMindFailing,              -99,   -70, "your mind failing"},
  {EMindTwitchy,              -79,   -50, "twitchy"},
  {EMindNervous,              -49,   -40, "nervous"},
  {EMindPerturbed,            -39,   -30, "perturbed"},
  {EMindWorried,              -29,   -10, "worried"},
  {EMindFine,                  -9,    10, "fine"},
  {EMindHappy,                 11,    30, "happy"},
  {EMindEcstatic,              31,    50, "ecstatic"},
  {EMindDangerouslyEcstatic,   51,   500, "dangerously ecstatic"}
};

Player::Player(Logger *log) : Being(log) {

  m_health = CMaxHealth;
  m_dmg    = CDefaultPlayerDmg;
  m_def    = CDefaultPlayerDef;
  m_acc    = CDefaultPlayerAcc;

  m_shy    = false;
  m_type   = ETypePlayer;

  m_inv = new Inventory(m_log);
  m_activeItem = NULL;
  m_log->debug("Player constructed");
}

Player::~Player() {
  delete m_inv;
  m_log->debug("Player destructed");
}

const char* Player::getMentalHealthStr() {
  int mind = CDefaultMentalHealth + m_inv->getTotalMoodChange();

  for (unsigned int i = 0; i < (sizeof(CMentalStates) / sizeof(mental_state_t)); i++) {
    if ((mind > CMentalStates[i].max)) {
      // move on
      continue;
    } else {
      return CMentalStates[i].str;
    }
  }
  m_log->error("should never get here, mental state invalid");
  return "";
}

mental_state_e Player::getMentalState() {
  int mind = CDefaultMentalHealth + m_inv->getTotalMoodChange();

  for (unsigned int i = 0; i < (sizeof(CMentalStates) / sizeof(mental_state_t)); i++) {
    if ((mind > CMentalStates[i].max)) {
      // move on
      continue;
    } else {
      return CMentalStates[i].state;
    }
  }
  m_log->error("should never get here, mental state invalid");
  return EMindFine;
}

void Player::restoreHealth(int heal) {
  if (heal < 0 || ((m_health + heal) > CMaxHealth))
    m_health = CMaxHealth;
  else
    m_health += heal;
}

int Player::getGold() {
  return m_inv->getGold();
}

void Player::addGold(int gold) {
  m_inv->addGold(gold);
}

bool Player::subGold(int gold) {
  return m_inv->subGold(gold);
}

void Player::setGold(int gold) {
  m_inv->setGold(gold);
}

Item* Player::setActiveItem(Item *item) {
  if (!item) {
    m_activeItem = getRandomItem();
  } else {
    m_activeItem = item;
  }
  return m_activeItem;
}

Item* Player::getActiveItem() {
  return m_activeItem;
}

void Player::addItem(Item *item) {
  m_inv->addItem(item);
}

int Player::loseItem(Item *item) {
  return m_inv->removeItem(item);
}

Item* Player::getRandomItem() {
  return m_inv->getRandomItem();
}

LiList<Item>* Player::getItems() {
  return m_inv->getItems();
}

int Player::useItem(Item *item) {
  Item *it = item;
  if (!it || !m_inv->checkItem(it)) it = m_activeItem;
  if (!it) return 0;

  int ret = it->use();
  int dmgmod = 0;

  // TODO: account for accuracy

  // factor in mental state
  switch(getMentalState()) {
    case EMindBroken:     dmgmod = -2; break;   // less penalty bc you fight harder with a borken mind
    case EMindFailing:    dmgmod = -10; break;
    case EMindTwitchy:    dmgmod = -8; break;
    case EMindNervous:    dmgmod = -6; break;
    case EMindPerturbed:  dmgmod = -4; break;
    case EMindWorried:    dmgmod = -2; break;
    case EMindFine:       dmgmod =  0; break;
    case EMindHappy:      dmgmod = 10; break;
    case EMindEcstatic:   dmgmod = 15; break;
    case EMindDangerouslyEcstatic: dmgmod = -5; break;  // penalty because you're not thinking straight
    default: break;
  }
  ret += dmgmod;
  if (ret < 0) ret = 0;

  return ret;
}
