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

#include "room.h"
#include <unistd.h>
#include <stdlib.h>

unsigned int Room::nRooms = 0;

bool operator==(const Room& lhs, const Room& rhs) {
  return (lhs.no == rhs.no);
}

Room::Room(Logger *log, const char *entry_text, bool is_end, bool has_gold,
            unsigned int gold, bool has_item, Item *item, bool has_monster, Monster *monster) {

  no = nRooms;
  nRooms++;

  m_log = log;

  north = NULL;
  south = NULL;
  east  = NULL;
  west  = NULL;

  memset(onentry, '\0', MAX_TEXT_LEN);
  strncpy(onentry, entry_text, MAX_TEXT_LEN - 1);
  this->is_end = is_end;
  this->found_end = false;
  this->has_item = has_item;
  this->item = item;
  this->has_monster = has_monster;
  this->monster = monster;
  this->has_gold = has_gold;
  this->gold = gold;

  if (this->has_item && !this->item) {
    m_log->error("Item or no item? in room generation");
    this->has_item = false;
  }

  if (this->has_monster && !this->monster) {
    m_log->error("Monster or no monster? in room generation");
    this->has_monster = false;
  }

  m_log->debug("Created room no %d with %d gold %s", no, gold, is_end ? "as end room" : "");

}

Room::~Room() {
  // remove room from room complex.
  // need to remove myself from my neighbors
  m_log->debug("Deleting room no %d", no);

  Room *n = north;
  Room *s = south;
  Room *w = west;
  Room *e = east;

  if (north != NULL) {
    if (!north->remove(ESouth)) {
      m_log->error("Error: Failed to remove self from North neighbor");
    } else if ((south && !south->remove(ENorth)) || !north->attach(s, ESouth)) {
      m_log->error("Failed to attach South neighbor to North");
    }
  }

  if (south != NULL) {
    if (!south->remove(ENorth)) {
      m_log->error("Error: Failed to remove self from South neighbor");
    } else if ((north && !north->remove(ESouth)) || !south->attach(n, ENorth)) {
      m_log->error("Failed to attach North neighbor to South");
    }
  }

  if (east != NULL) {
    if (!east->remove(EWest)) {
      m_log->error("Error: Failed to remove self from East neighbor");
    } else if ((west && !west->remove(EEast)) || !east->attach(w, EWest)) {
      m_log->error("Failed to attach West neighbor to East");
    }
  }

  if (west != NULL) {
    if (!west->remove(EEast)) {
      m_log->error("Error: Failed to remove self from West neighbor");
    } else if ((east && !east->remove(EWest)) || !west->attach(e, EEast)) {
      m_log->error("Failed to attach East neighbor to West");
    }
  }

  north = NULL;
  south = NULL;
  east = NULL;
  west = NULL;

  if (item) {
    delete item;
  }

  if (monster) {
    delete monster;
  }

}

int Room::attach(Room *room, direction_e dir) {

  if (room == NULL) {
    m_log->debug("Not attaching NULL room");
    return 0;
  }

  switch (dir) {
    case ENorth:
      if (north) {
        m_log->debug("North neighbor already exists: room %d", north->getNo());
        return 0;
      }
      if (room->South()) {
        m_log->error("Error: room to attach already has south neighbor: room %d", room->South()->getNo());
        return 0;
      }

      if ((west && west->North() == room) ||
          (east && east->North() == room)) {
        m_log->error("Error: Room would be its own neighbor");
        return 0;
      }

      north = room;
      room->south = this;
      m_log->debug("Attached room%d to room%d-north", room->getNo(), no);
      break;
    case ESouth:
      if (south != NULL) {
        m_log->debug("South neighbor already exists: room %d", south->getNo());
        return 0;
      }

      if (room->North() != NULL) {
        m_log->error("Error: room to attach already has north neighbor: room %d", room->North()->getNo());
        return 0;
      }

      if ((west && west->South() == room) ||
          (east && east->South() == room)) {
        m_log->error("Error: Room would be its own neighbor");
        return 0;
      }

      south = room;
      room->north = this;
      m_log->debug("Attached room%d to room%d-south", room->getNo(), no);
      break;
    case EWest:
      if (west != NULL) {
        m_log->debug("West neighbor already exists: room %d", west->getNo());
        return 0;
      }
      if (room->East() != NULL) {
        m_log->error("Error: room to attach already has east neighbor: room %d", room->East()->getNo());
        return 0;
      }

      if ((north && north->West() == room) ||
          (south && south->West() == room)) {
        m_log->error("Error: Room would be its own neighbor");
        return 0;
      }

      west = room;
      room->east = this;
      m_log->debug("Attached room%d to room%d-west", room->getNo(), no);
      break;
    case EEast:
      if (east != NULL) {
        m_log->debug("East neighbor already exists: room %d", east->getNo());
        return 0;
      }

      if (room->West() != NULL) {
        m_log->error("Error: room to attach already has west neighbor: room %d", room->West()->getNo());
        return 0;
      }

      if ((north && north->East() == room) ||
          (south && south->East() == room)) {
        m_log->error("Error: Room would be its own neighbor");
        return 0;
      }
      east = room;
      room->west = this;
      m_log->debug("Attached room%d to room%d-east", room->getNo(), no);
      break;
    case ENone:
    default: m_log->debug("Got no valid direction"); return 0;
  }
  return 1;
}

int Room::remove(direction_e dir) {
  switch (dir) {
    case ENorth:
      if (north == NULL) return 0;
      north = NULL;
      break;
    case ESouth:
      if (south == NULL) return 0;
      south = NULL;
      break;
    case EWest:
      if (west == NULL) return 0;
      west = NULL;
      break;
    case EEast:
      if (east == NULL) return 0;
      east = NULL;
      break;
    case ENone:
    default: return 0;
  }
  return 1;
}

int Room::detach(direction_e dir) {
  switch (dir) {
    case ENorth:
      if (north == NULL) return 0;
      m_log->debug("Detaching room%d from room%d", north->getNo(), no);
      north->south = NULL;
      north = NULL;
      break;
    case ESouth:
      if (south == NULL) return 0;
      m_log->debug("Detaching room%d from room%d", south->getNo(), no);
      south->north = NULL;
      south = NULL;
      break;
    case EWest:
      if (west == NULL) return 0;
      m_log->debug("Detaching room%d from room%d", west->getNo(), no);
      west->east = NULL;
      west = NULL;
      break;
    case EEast:
      if (east == NULL) return 0;
      m_log->debug("Detaching room%d from room%d", east->getNo(), no);
      east->west = NULL;
      east = NULL;
      break;
    case ENone:
    default: return 0;
  }
  return 1;
}

Room* Room::move(direction_e dir) {
  Room *move_room = NULL;

  switch (dir) {
    case ENone: return this;
    case ENorth:
      move_room = north;
      break;
    case ESouth:
      move_room = south;
      break;
    case EWest:
      move_room = west;
      break;
    case EEast:
      move_room = east;
      break;
    default: break;
  }
  return move_room;
}

const char* Room::getEntry() {
  return onentry;
}

unsigned int Room::look() {
  unsigned int ret = 0;
  if (has_item) {
    ret |= CFoundItem;
  }
  if (has_gold && gold) {
    ret |= CFoundGold;
  }
  if (has_monster && monster->isAlive()) {
    ret |= CFoundMonster;
  }
  if (is_end) {
    found_end = true;
  }

  return ret;
}

Item* Room::getItem() {
  if (has_item) {
    has_item = false;
    return item;
  }
  return NULL;
}

bool Room::hasGold() {
  return has_gold;
}

unsigned int Room::getGold() {
  if (has_gold) {
    unsigned int g = gold;
    gold = 0;
    return g;
  }
  return  0;
}

Room *Room::North() {
  return north;
}

Room *Room::South() {
  return south;
}

Room *Room::West() {
  return west;
}

Room *Room::East() {
  return east;
}

unsigned int Room::getNo() const {
  return no;
}

unsigned int Room::openSlots() {
  unsigned int open = 0;
  if (north == NULL) open++;
  if (south == NULL) open++;
  if (west == NULL) open++;
  if (east == NULL) open++;

  return open;
}

/// Is room connected to at least one other room
bool Room::isConnected() {
  return (north || south || west || east);
}

/// Does room have any unconnected doors
bool Room::isOpen() {
  return !(north && south && west && east);
}

/// Check if a specific direction \a dir is unconnected
bool Room::isOpenDir(direction_e dir) {
  bool open = true;

  switch(dir) {
    case ENorth: if (north) open = false; break;
    case ESouth: if (south) open = false; break;
    case EWest:  if (west)  open = false; break;
    case EEast:  if (east)  open = false; break;
    case ENone:  /* fallthrough*/
    default: break;
  }
  return open;
}

bool Room::isEnd() {
  return is_end;
}

bool Room::foundEnd() {
  return found_end;
}

bool Room::hasMonster() {
  return has_monster;
}

bool Room::hasAliveMonster() {
  return (has_monster && monster->isAlive());
}

const char* Room::getMonsterName() {
  return (hasMonster() ? monster->getName() : "");
}

Monster* Room::getMonster() {
  return monster;
}

void Room::logConnections() {
  m_log->debug("room%d: N:%d, S:%d, W:%d, E:%d", no,
                                                 north ? north->getNo() : -1,
                                                 south ? south->getNo() : -1,
                                                 west  ? west->getNo()  : -1,
                                                 east  ? east->getNo()  : -1);
}
