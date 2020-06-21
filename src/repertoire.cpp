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
#include "repertoire.h"
#include <assert.h>

Repertoire::Repertoire(Logger *log) {
  m_log = log;
  m_attacks = new LiList<Attack>();
  m_log->debug("created repertoire...");
}

Repertoire::Repertoire(const Repertoire& other) {
  m_log = other.m_log;
  m_attacks = new LiList<Attack>(*other.m_attacks);
  assert(m_attacks->Size() == other.m_attacks->Size());
  m_log->debug("copied repertoire...");
}

Repertoire::~Repertoire() {
  m_log->debug("Destroying repertoire...");
  if (m_attacks) {
    delete m_attacks;
  }
}

void Repertoire::deleteAttacks() {
  m_attacks->free();
}

int Repertoire::addAtk(Attack *atk) {
  if (!atk) return 0;
  if (!m_attacks->add(atk)) return 0;
  m_log->debug("added attack to repertoire");
  return 1;
}

int Repertoire::removeAtk(Attack* atk) {
  if (!atk) return 0;
  if (!m_attacks->remove(atk)) return 0;
  m_log->debug("removed attack from repertoire");
  return 1;
}

int Repertoire::useAtk(Attack *atk) {
  m_log->debug("using attack");
  if (!atk) {
    Attack *a = getRandomAttack();
    return (a? a->use() : 0);
  }

  return atk->use();
}

bool Repertoire::checkAttack(Attack *atk) {
  if (m_attacks->find(atk) == NULL) return false;
  return true;
}

LiList<Attack>* Repertoire::getAttacks() {
  return m_attacks;
}

int Repertoire::getAttacksLen() {
  return m_attacks->Size();
}

Attack* Repertoire::getRandomAttack() {
  return m_attacks->get_random();
}
