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

#include "item.h"
#include "item_seed.cpp"
#include <stdlib.h>
#include <string.h>

unsigned int Item::nItems = 0;

bool operator==(const Item& lhs, const Item& rhs) {
  return (lhs.no == rhs.no);
}

const char Item::CItemStateStr[][MAX_STATE_LEN] = {
  "broken",
  "almost broken",
  "damaged",
  "fine",
  "new"
};

Item::Item(Logger *log) {
  no = nItems;
  nItems++;

  m_log = log;
  m_log->info("generating random item");

  // generate stats
  int base_idx = rand() % classSize();
  m_log->debug("base index: %d", base_idx);
  itemclass_t baseclass = itemclasses[base_idx];
  m_log->debug("got baseclass name %s", baseclass.name);

  int rrun = CModLikelyhoodBase;
  int idx[CMaxMods] = {-1};
  int n_idx = 0;

  while (((rand() % CMaxRand) < rrun) && n_idx < CMaxMods) {
    bool got_idx;

    do {
      got_idx = true;
      idx[n_idx] = rand() % modSize();
      for (int i = 0; i < n_idx; i++) {
        if (idx[i] == idx[n_idx]) {
          got_idx = false;
          break;
        }
      }
    } while (!got_idx);
    n_idx++;
    rrun -= CModLikelyhoodInc;
  }

  m_log->debug("n_idx: %d", n_idx);
  m_health = CDefaultHealth;
  m_damage = baseclass.damage;
  m_mood_effect = baseclass.mood;
  strncpy(this->name, baseclass.name, MAX_ITEM_NAME);

  for (int i = 0; i < n_idx; i++) {
    if (idx[i] < 0) break;
    m_log->debug("mod index: %d", idx[i]);
    m_damage += modifiers[idx[i]].damage_mod;
    m_mood_effect += modifiers[idx[i]].mood_mod;
    char tmp[MAX_ITEM_NAME];
    strncpy(tmp, this->name, MAX_ITEM_NAME);
    snprintf(this->name, MAX_ITEM_NAME, "%s %s", modifiers[idx[i]].name, tmp);
  }

  // value sanity checks
  if (m_damage < 0) m_damage = 0;

  // get item value
  calcValue();

  snprintf(this->repr_str, MAX_REPR_LEN, "%s %s",
            baseclass.plural ? "Some" : startsWithVowel(this->name) ? "An" : "A", name);
  m_log->debug("created random item");
  logStats();
}

Item::Item(Logger *log, const char *name, unsigned int damage, unsigned int health) {
  no = nItems;
  nItems++;

  m_log = log;
  strncpy(this->name, name, MAX_ITEM_NAME);

  m_damage = damage ? damage : rand() % POWER_MAX;
  m_health = health;
  m_mood_effect = CDefaultMood;
  snprintf(repr_str, MAX_REPR_LEN, "%s %s",
            endsWithPlural(this->name) ? "Some" : \
            startsWithVowel(this->name) ? "An" : "A", this->name);
}

Item::~Item() {

}

void Item::calcValue() {
  // value depends on factors damage + mood_effect + health
  int base_value = CGoldPerDamage * m_damage;
  int mood_mod = (int)((double)m_mood_effect / 10);
  double health_mod = (double)getState() / 100;  // percentage to subtract from value based on item health

  m_value = (base_value + mood_mod) * health_mod;

  m_log->debug("Calculated item value: %d (base: %d, mood mod: %d, health mod: %f%%)", m_value, base_value, mood_mod, health_mod);
}

void Item::logStats() {
  m_log->debug("Item \"%s\", pow: %d, health: %d, mood: %+d, value: %d", name, m_damage, m_health, m_mood_effect, m_value);
}

item_state_e Item::getState() {
  if (m_health <= EBroken)  return EBroken;
  if (m_health <= ERavaged) return ERavaged;
  if (m_health <= EDamaged) return EDamaged;
  if (m_health <= EHealthy) return EHealthy;
  return EMax;
}

unsigned int Item::Damage() {
  return m_damage;
}

unsigned int Item::Health() {
  return m_health;
}

int Item::Mood() {
  return m_mood_effect;
}

int Item::Value() {
  return m_value;
}

int Item::use() {
  int dmg = m_damage + ((rand() % 5) - 2);
  int hit = rand() % CMaxRand;
  int health_change = CDefaultItemHealthLoss;

  if (hit <= CHitChanceFail) {
    m_log->debug("Hit failed!");
    dmg -= CFailedHitPenalty;
    health_change += 5;
  } else if (hit >= CHitChanceCrit) {
    m_log->debug("critical hit!");
    dmg += CCriticalHitBonus;
    health_change -= 5;
  }

  m_log->debug("Using item with base dmg: %d and actual dmg %d", m_damage, dmg);

  if ((m_health - health_change) <= 0) {
    m_health = 0;
    m_log->debug("item broke while using");
  } else {
    m_health -= health_change;
  }

  return (dmg > 0) ? dmg : 0;
}

const char* Item::represent() {
  return repr_str;
}
