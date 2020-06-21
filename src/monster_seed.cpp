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
#ifndef _MONSTER_SEED_C_
#define _MONSTER_SEED_C_
// this file only contains data for monster generation

#include "monster_factory.h"

static const int CDefaultHealthTiny   = 15;
static const int CDefaultHealthSmall  = 25;
static const int CDefaultHealthMedium = 60;
static const int CDefaultHealthLarge  = 100;
static const int CDefaultHealthBoss   = 200;

static const int CDefaultAcc = 40;

static const int CDefaultDefTiny      = 0;
static const int CDefaultDefSmall     = 2;
static const int CDefaultDefMedium    = 3;
static const int CDefaultDefLarge     = 6;
static const int CDefaultDefBoss      = 8;

typedef struct monsterType {
  subtype_e subtype;
  char name[Monster::CMonsterNameLen];
  attack_e attacks[MonsterFactory::CMaxAttacks];
  int baseHealth;
  int baseDmg;
  int baseAcc;
  int baseDef;
} monster_type_t;

const monster_type_t tiny_monsters[] = {
  {ESubTypeTiny, "Mouse",           {EAtkNibble,      EAtkNone}, CDefaultHealthTiny, 10, CDefaultAcc, CDefaultDefTiny},
  {ESubTypeTiny, "Bug",             {EAtkScuttle,     EAtkNone}, CDefaultHealthTiny, 5, CDefaultAcc, CDefaultDefTiny},
  {ESubTypeTiny, "Butterfly",       {EAtkFlutter,     EAtkNone}, CDefaultHealthTiny, 5, CDefaultAcc, CDefaultDefTiny},
  {ESubTypeTiny, "Sentient Spoon",  {EAtkReflect,     EAtkNone}, CDefaultHealthTiny, 3, CDefaultAcc, CDefaultDefTiny},
  {ESubTypeTiny, "Tiny Mushroom",   {EAtkTinySpores,  EAtkNone}, CDefaultHealthTiny, 10, CDefaultAcc, CDefaultDefTiny}
};

const monster_type_t small_monsters[] = {
  {ESubTypeSmall, "Black Cat",      {EAtkNibble, EAtkScratch, EAtkLookCute, EAtkNone}, CDefaultHealthSmall, 20, CDefaultAcc, CDefaultDefSmall},
  {ESubTypeSmall, "Green Cat",      {EAtkNibble, EAtkScratch, EAtkConfuse, EAtkNone},  CDefaultHealthSmall, 15 , CDefaultAcc, CDefaultDefSmall},
  /*{ESubTypeSmall, "", CDefaultHealthSmall, , CDefaultAcc, CDefaultDefSmall},
  {ESubTypeSmall, "", CDefaultHealthSmall, , CDefaultAcc, CDefaultDefSmall},*/
  {ESubTypeSmall, "Small Mushroom", {EAtkTinySpores, EAtkSmallSpores, EAtkNone},       CDefaultHealthSmall, 18, CDefaultAcc, CDefaultDefSmall}
};

const monster_type_t medium_monsters[] = {
  {ESubTypeMedium, "Talking Gorilla",         {EAtkScratch, EAtkConfuse, EAtkNone}, CDefaultHealthMedium, 25, CDefaultAcc, CDefaultDefMedium},
  {ESubTypeMedium, "Medium-sized Statue",     {EAtkReflect, EAtkNone},              CDefaultHealthMedium, 35, CDefaultAcc, CDefaultDefMedium},
  /*{ESubTypeMedium, "", CDefaultHealthMedium, , CDefaultAcc, CDefaultDefMedium},
  {ESubTypeMedium, "", CDefaultHealthMedium, , CDefaultAcc, CDefaultDefMedium},*/
  {ESubTypeMedium, "Normally-sized Mushroom", {EAtkTinySpores, EAtkSmallSpores, EAtkSpores, EAtkNone}, CDefaultHealthMedium, 30, CDefaultAcc, CDefaultDefMedium}
};

const monster_type_t large_monsters[] = {
  /*{ESubTypeLarge, "", CDefaultHealthLarge, , CDefaultAcc, CDefaultDefLarge},*/
  {ESubTypeLarge, "Floating Chess Table",     {EAtkConfuse, EAtkShove, EAtkReflect, EAtkNone},      CDefaultHealthLarge, 42, CDefaultAcc, CDefaultDefLarge},
  {ESubTypeLarge, "Way Too Cheesy Sandwich",  {EAtkConfuse, EAtkSmallSpores, EAtkShove, EAtkNone},  CDefaultHealthLarge, 40, CDefaultAcc, CDefaultDefLarge},
  {ESubTypeLarge, "Large Mushroom",           {EAtkTinySpores, EAtkSmallSpores, EAtkSpores, EAtkReflect, EAtkNone}, CDefaultHealthLarge, 38, CDefaultAcc, CDefaultDefLarge},
};

const monster_type_t boss_monsters[] = {
  /*{ESubTypeBoss, "", CDefaultHealthBoss, , CDefaultAcc, CDefaultDefBoss},*/
  {ESubTypeBoss, "Your Boss",       {EAtkSpores, EAtkPunch, EAtkNone}, CDefaultHealthBoss, 66, CDefaultAcc, CDefaultDefBoss},
  {ESubTypeBoss, "Demagorgon",      {EAtkPunch, EAtkScratch, EAtkNone}, CDefaultHealthBoss, 50, CDefaultAcc, CDefaultDefBoss},
  {ESubTypeBoss, "Mushroom Colony", {EAtkTinySpores, EAtkSmallSpores, EAtkSpores, EAtkReflect, EAtkPsilocybin, EAtkNone}, CDefaultHealthBoss, 48, CDefaultAcc, CDefaultDefBoss}
};

static int tinySize() {
  return (sizeof(tiny_monsters) / sizeof(monster_type_t));
}

static int smallSize() {
  return (sizeof(small_monsters) / sizeof(monster_type_t));
}

static int mediumSize() {
  return (sizeof(medium_monsters) / sizeof(monster_type_t));
}

static int largeSize() {
  return (sizeof(large_monsters) / sizeof(monster_type_t));
}

static int bossSize() {
  return (sizeof(boss_monsters) / sizeof(monster_type_t));
}


#endif
