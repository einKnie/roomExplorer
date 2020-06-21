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
#ifndef _ROOM_FAC_H_
#define _ROOM_FAC_H_

#include <list>
#include <stdio.h>
#include <unistd.h>
#include "room.h"
#include "lilist.h"
#include "roomExplorer.h"
#include "item.h"
#include "monster.h"

#define MAXRAND           100 ///< max rand
#define GOLD_CHANCE       40  ///< change of gold in room
#define GOLD_HIGH_CHANCE  20  ///< chance of high amount of gold in room
#define ITEM_CHANCE       40  ///< chance of item in room
#define MONSTER_CHANCE    40  ///< chance of monster in room
#define MAX_GOLD          20  ///< max amount of gold
#define MAX_HIGH_GOLD     60  ///< max amount of gold if high gold amount

// #define ONLY_ITEMS
#ifdef ONLY_ITEMS
# undef ITEM_CHANCE
# undef MONSTER_CHANCE
# define ITEM_CHANCE 100
# define MONSTER_CHANCE 0
#endif

typedef struct roomSource {
  char onentry[MAX_TEXT_LEN]; ///< entry text
  bool is_end;
  bool has_gold;
  bool has_item;
  unsigned int gold;
  Item *item;

  roomSource *next;
  roomSource *prev;

  roomSource() {
    memset(onentry, '\0', MAX_TEXT_LEN);
    is_end = false;
    has_gold = false;
    has_item = false;
    gold = 0;
    item = NULL;

    next = NULL;
    prev = NULL;
  }

  roomSource(const char *entry, const char *exit, bool is_end, bool has_gold, unsigned int gold, bool has_item, Item *item) {
    strncpy(onentry, entry, MAX_TEXT_LEN);
    this->has_item = has_item;
    this->is_end = is_end;
    this->has_gold = has_gold;
    this->gold = gold;
    this->item = item;

    next = NULL;
    prev = NULL;
  }

  roomSource(roomSource *r) {
    strncpy(onentry, r->onentry, MAX_TEXT_LEN);
    is_end = r->is_end;
    has_gold = r->has_gold;
    has_item = r->has_item;
    gold = r->gold;
    item = r->item;

    next = r->next;
    prev = r->prev;
  }
} room_source_t;


/// @brief Room Factory
class RoomFactory {
private:
  static const int CMaxRooms   = 20; ///< max amount of rooms

  unsigned int  m_no;                ///< number of rooms to create
  Logger       *m_log;               ///< logger
  LiList<Room> *m_room_list;         ///< list of created rooms

  /// @brief Generate \a m_no random rooms
  int generateRooms();

public:
  /// Default constructor
  /// @param[in] log logger
  /// @param[in] no number of rooms to create
  RoomFactory(Logger *log, int no);
  ~RoomFactory();

  /// @brief Create a linked list of rooms
  /// param[in,out] rList contains rooms after call
  /// @return the created linked list
  /// @note The list returned from this must be freed by the caller
  LiList<Room>* createRoomList();

  /// @brief Return the room linked list
  LiList<Room>* getRoomList();

};

#endif // _ROOM_FAC_H_
