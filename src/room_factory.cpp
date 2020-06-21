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
#include "room_factory.h"
#include "monster_factory.h"
#include <stdlib.h>
#include <string.h>

static const char* const roomTexts[] = {
  "A room",
  "Another room",
  "A boring room",
  "This room is entirely unremarkable",
  "The most marvelous in a row of rooms",
  "A room appears before your eyes!",
  "This room seems to be hiding something...",
  "The room looks gloomy",
  "This room is unusually bright",
  "asdf     ...room",
  "Another day, another room...",
  "This room is an easter egg",
  "An ugly room",
  "Room, Hall, a smaller hall",
  "This room is horrible, you immediately want to leave"
};

static int maxRoomText() {
  return (sizeof(roomTexts) / sizeof(roomTexts[0]));
}

RoomFactory::RoomFactory(Logger *log, int no) {
  m_log = log;
  m_no = (no <= CMaxRooms) ? no : CMaxRooms;
  m_room_list = new LiList<Room>();
}

RoomFactory::~RoomFactory() {
  // m_room_list is not deleted here on purpose. deletion happens in engine
}

LiList<Room>* RoomFactory::createRoomList() {

  if (!generateRooms()) {
    m_log->error("Failed to generate rooms");
    return NULL;
  }

  return m_room_list;
}

LiList<Room>* RoomFactory::getRoomList() {
  return m_room_list;
}

int RoomFactory::generateRooms() {

  MonsterFactory fac = MonsterFactory(m_log);
  m_log->debug("Room factory starting room generation for %d rooms", m_no);
  unsigned int end_room = rand() % m_no;

  for (unsigned int i = 0; i < m_no; i++) {
    const char *entry;
    bool has_gold = false;
    bool has_item = false;
    bool has_monster = false;
    unsigned int gold = 0;
    Item *item = NULL;
    Monster *monster = NULL;
    bool is_end = false;

    m_log->info("Generating room %d of %d", i, m_no);

    // 1 get entry text
    entry = roomTexts[rand() % maxRoomText()];
    m_log->debug("got entry text: %s", entry);

    // 2 get gold && amount
    unsigned int maxGold = 0;
    has_gold = (rand() % MAXRAND) < GOLD_CHANCE;
    if (has_gold) {
      int has_high_gold = (rand() % MAXRAND) < GOLD_HIGH_CHANCE;
      if (has_high_gold) maxGold = MAX_HIGH_GOLD;
      else maxGold = MAX_GOLD;
      gold = (rand() % maxGold) + 1;
    }
    m_log->debug("got %sgold (%d)", has_gold ? "" : "no ", gold);

    // 3 get item
    has_item = (rand() % MAXRAND) < ITEM_CHANCE;
    if (has_item) {
      item = new Item(m_log);
    }
    m_log->debug("got %sitem (%s)", has_item ? "" : "no ", has_item ? item->name : "none");

    // 4 add monster
    has_monster = (rand() % MAXRAND) < MONSTER_CHANCE;
    if (has_monster) {
      monster = fac.getMonster();
    }
    m_log->debug("got %smonster (%s)", has_monster ? "" : "no ", has_monster ? monster->getName() : "none");

    // 5 is end
    if (i == end_room) {
      is_end = true;
      m_log->debug("setting as end room");
    }

    // add new room source to room list
    m_room_list->add(new Room(m_log, entry, is_end, has_gold, gold, has_item, item, has_monster, monster));
  }

  return 1;
}
