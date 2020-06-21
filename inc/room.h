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
#ifndef _ROOM_H_
#define _ROOM_H_

#include <stdio.h>
#include <string.h>
#include "roomExplorer.h"
#include "item.h"
#include "monster.h"

#define MAX_TEXT_LEN    80  ///< max length of room entry text
#define MIN_ROOM_SIDE   8   ///< minimal room size
#define ROOM_HEIGHT     8   ///< room height for drawing. must be multiple of two
#define ROOM_WIDTH      16  ///< room width for drawing

#if ROOM_HEIGHT < MIN_ROOM_SIDE
#undef ROOM_HEIGHT
#define ROOM_HEIGHT MIN_ROOM_SIDE
#endif

#if ROOM_WIDTH < MIN_ROOM_SIDE
#undef ROOM_WIDTH
#define ROOM_WIDTH MIN_ROOM_SIDE
#endif

typedef enum {
  ENone,
  ENorth,
  ESouth,
  EEast,
  EWest
} direction_e;  ///< directions enumeration

/// @brief Class representation of a room
class Room {
private:
  Logger *m_log;              ///< logger

  static unsigned int nRooms; ///< total number of rooms
  unsigned int no;            ///< unique room identifier
  Room *north;                ///< north door
  Room *south;                ///< south door
  Room *west;                 ///< west door
  Room *east;                 ///< east door

  char onentry[MAX_TEXT_LEN]; ///< room entry text

  bool is_end;                ///< is end room
  bool found_end;             ///< flag set when user discovers end door
  bool has_item;              ///< room has an item
  bool has_gold;              ///< room has gold
  bool has_monster;           ///< room has a monster in it
  unsigned int gold;          ///< amount of gold in room
  Item *item;                 /// item in room
  Monster *monster;           /// monster in room

public:
  static const unsigned int CFoundGold = 0x1;     ///< found gold flag
  static const unsigned int CFoundItem = 0x2;     ///< found item flag
  static const unsigned int CFoundMonster = 0x04; ///< found monster flag

  /// @brief Constructor
  /// @param[in] log logger
  /// @param[in] entry room entry text
  /// @param[in] is_end room is end room
  /// @param[in] has_gold room has gold
  /// @param[in] gold amount of gold
  /// @param[in] has_item room has item
  /// @param[in] item an item or NULL
  Room(Logger *log, const char *entry, bool is_end = false,
        bool has_gold = false, unsigned int gold = 0,
        bool has_item = 0, Item *item = NULL,
        bool has_monster = 0, Monster *monster = NULL);

  ~Room();

  /// @brief Get the room's entry text
  /// @return the entry text
  const char* getEntry(void);

  /// @brief look around the room
  /// @return mask of found stuff
  /// @todo rethink this
  unsigned int look();

  /// @brief Draw the room on screen
  /// @param[in] width room width
  /// @param[in] height room height
  void draw(int width = ROOM_WIDTH, int height = ROOM_HEIGHT);

  /// @brief Attach \a room in direction \a dir to this
  ///
  /// Performs basic checks before attaching rooms
  /// @param[in] room the room to attach
  /// @param[in] dir the direction to attach \a room in
  /// @return 1 on success, 0 on failure
  int attach(Room *room, direction_e dir);

  /// @brief Detach a neighboring room
  ///
  /// Clean detachment of neighboring room in direction \a dir
  /// @param[in] dir the direction of the room to detach
  /// @return 1 if a neighbor was detached, 0 otherwise
  /// @note this method will return 0 also if there was no neighboring room to detach
  int detach(direction_e dir);

  /// @brief Remove a neighbor
  ///
  /// Simply remove reference to a neighboring room in direction \a dir
  /// @note This does not remove this room's reference in the neighbor. Use detach() for that.
  /// @param[in] dir direction of room to remove
  /// @return 1 if a room was removed
  /// @todo see if we can remove this
  int remove(direction_e dir);

  /// @brief Move to a neighboring room
  /// @param[in] dir direction to move in
  /// @return the moved-in room
  /// @todo: think about whether this should even be handled by the room
  Room *move(direction_e dir);

  /// Return north neighbor
  Room *North();
  /// Return south neighbor
  Room *South();
  /// Return west neighbor
  Room *West();
  /// Return east neighbor
  Room *East();

  /// @brief return unique number of room
  unsigned int getNo() const;

  /// @brief Check if end room
  /// @return true if room is end room
  bool isEnd();

  /// @brief Check is the player already found out that this is an end room
  /// @return true if room is end room and player knows it, else false
  bool foundEnd();

  /// @brief Check if room has item
  /// @return true if room has item
  bool hasItem();

  /// @breif Retrieve the room's item
  /// @return the item of this room
  /// @note this effectively removes the item from this room
  /// @todo think about item destruction
  Item* getItem();

  /// @brief Check if room has monster
  /// @return true if monster in room, false otherwise
  bool hasMonster();

  /// @brief Check if room has a living monster
  /// @return true if monster is present and alive, else false
  bool hasAliveMonster();

  /// @brief Return the monster's name, if there is one
  /// @return the monster's name, or "" if no monster there
  const char* getMonsterName();

  /// @brief Return the room's monster
  /// @return the monster, or NULL if room has no monster
  Monster* getMonster();

  /// @brief Check if room has or has had gold
  /// @return true if the room had gold at creation
  bool hasGold();

  /// @brief Retrieve the room's gold
  /// @return the gold available in the room
  /// @note this sets the room's gold to zero
  unsigned int getGold();

  /// @brief Return true if room has at least one connection to another room
  /// @return true if room has a connection, else false
  bool isConnected();

  /// @brief Return true if room has at least one unconnected door
  /// @return true if room has open slot, else false
  bool isOpen();

  /// @brief Return connection status of door in direction \a dir
  /// @return true if direction is unconnected, else false
  bool isOpenDir(direction_e dir);

  /// @brief Check if room has unconnected doors
  /// @return number of unconnected doors
  unsigned int openSlots();

  /// Log room connections
  void logConnections();

  friend bool operator== (const Room &lhs, const Room &rhs);

};


#endif // _ROOM_H_
