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
#include "maze.h"
#include "room_factory.h"
#include <stdlib.h>
#include <time.h>

// Simple constructor
// Generates its own room list from room factory if no room list is given
Maze::Maze(int s, Logger *log, LiList<Room> *rooms) {
  m_room_list = NULL;
  m_seed = s;
  srand(m_seed);
  m_log = log;

  if (rooms) {
    m_destroy_roomlist = false;
    m_room_list = rooms;
  } else {
    m_destroy_roomlist = true;

    RoomFactory *fac = new RoomFactory(log, 12); // TODO: remove magic number
    m_room_list = fac->createRoomList();
    if (!m_room_list) {
      m_log->error("Failed to create room list");
    }

    delete fac;
    logList(m_room_list);
  }

  m_log->debug("room list size: %d", m_room_list->Size());
}

Maze::~Maze() {
  if (m_destroy_roomlist) {
    m_room_list->free();
    delete m_room_list;
  }
}

int Maze::size() {
  return m_room_list->Size();
}

Room *Maze::getRandom(bool open) {
  if (!m_room_list->Head()) return NULL;
  Room *tmp = NULL;
  // if open, return only rooms with open directions
  do {
    tmp = m_room_list->get_random();
  } while (!tmp || (open && !tmp->isOpen()));

  return tmp;
}

void Maze::listRooms() {
  int i = 0;
  if (m_room_list->Head()) {
    Room *tmp = m_room_list->Head();
    do {
      m_log->debug("Element %02d : Room %02d", i, tmp->getNo());
      i++;
      tmp = m_room_list->Next();
    } while (tmp);
  }
}

bool Maze::anyIsolated() {
  Room *tmp = m_room_list->Head();
  while (tmp != NULL) {
    if (!tmp->isConnected()) return true;
    tmp = m_room_list->Next();
  }
  return false;
}

Room* Maze::createLinearMaze() {

  LiList<Room> *isolated = NULL;
  LiList<Room> *full = NULL;
  LiList<Room> *open = NULL;

  open = getRoomList(open);
  isolated = getIsolated(isolated);

  // get random starting room
  Room *start = NULL;
  do {
    start = open->get_random();
    m_log->debug("Getting starting room: room %d", start ? start->getNo() : -1);
  } while (!start || start->isEnd());

  Room *curr = start;
  Room *to_attach = NULL;
  direction_e dir = ENone;

  m_log->debug("Starting maze creation of %d rooms at room %d", open->Size(), curr->getNo());

  do {
    // choose a random room from open
    // choose a random neighbor from isolated
    to_attach = isolated->get_random();
    if (to_attach == NULL) {
      m_log->debug("Not attaching NULL room");
      continue;
    }

    m_log->debug("Trying to attach random room %d", to_attach->getNo());
    to_attach->logConnections();

    // attach neighbor to current room in random direction
    do {
      dir = getRandDir();
      m_log->debug(".");
    } while (!curr->isOpenDir(dir));

    m_log->debug("Using direction %s", (dir == ENorth) ? "north" :
                                        (dir == ESouth) ? "south" :
                                          (dir == EWest) ? "west" : "east");

    // attach neighbor room to current room
    if (!attachRoom(curr, to_attach, dir)) {
      m_log->error("Failed to attach room!");
      continue;
    }

    isolated = getIsolated(isolated);
    full = getFull(full);
    open = getOpen(open);

    m_log->debug("Got new room lists:\nisolated: %d\nfull: %d\nopen: %d",
                  isolated->Size(), full->Size(), open->Size());

    // get next room
    curr = open->get_random();
    m_log->debug("Moving on to room %d", curr->getNo());
    curr->logConnections();

  } while (isolated->Size());

  m_log->debug("Finished maze creation");

  delete isolated;
  delete full;
  delete open;
  return start;
}

// Attach roomB to roomA in direction dir
//  __________  dir __________
// |_ROOM_A__| --> |_ROOM_B__|
int Maze::attachRoom(Room *roomA, Room *roomB, direction_e dir) {
  // First, basic checks
  // e.g. if we're trying to attach a room to itself
  if (!roomA || !roomB || (roomA == roomB)) {
    m_log->error("Invalid room attachment: room%d to room %d", roomA ? roomA->getNo() : -1,
                                                               roomB ? roomB->getNo() : -1);
    return 0;
  }

  // check if rooms already have neighbors
  bool valid = true;
  switch(dir) {
    case ENorth:
      valid =  !(roomA->North() || roomB->South());
      break;
    case ESouth:
      valid = !(roomA->South() || roomB->North());
      break;
    case EWest:
      valid = !(roomA->West() || roomB->East());
      break;
    case EEast:
      valid = !(roomA->East() || roomB->West());
      break;
    case ENone: /*fallthrough*/
    default:
      m_log->debug("Invalid direction: rooms have neighbors already");
      return 0;
  }

  // avoid multiple connections between same rooms
  if (((roomA->North() == roomB) || (roomA->South() == roomB) ||
       (roomA->West() == roomB)  || (roomA->East() == roomB)) ||
      ((roomB->North() == roomA) || (roomB->South() == roomA) ||
       (roomB->West() == roomA)  || (roomB->East() == roomA))) {
         m_log->debug("Invalid rooms: rooms are neighbors already");
        valid = false;
      }

  m_log->debug("Attaching (prev):");
  roomA->logConnections();
  roomB->logConnections();

  if (!valid) {
    m_log->debug("Invalid room placement. aborting...");
    return 0;
  }

  // actually attach roomB to roomA
  if (!roomA->attach(roomB, dir)) {
    m_log->error("Failed to attach room %d to room %d", roomB->getNo(), roomA->getNo());
    return 0;
  }
  m_log->info("Attached room %d to room %d", roomB->getNo(), roomA->getNo());

  // Check roomB's neighbors
  // unset any preexisting neighbors if they conflict with the new setting
  switch(dir) {
    case ENorth:
      if (roomB->West() && roomB->West()->South()) {
        roomB->West()->South()->detach(EEast);
      }
      if (roomB->East() && roomB->East()->South()) {
        roomB->East()->South()->detach(EWest);
      }
      break;
    case ESouth:
      if (roomB->West() && roomB->West()->North()) {
        roomB->West()->North()->detach(EEast);
      }
      if (roomB->East() && roomB->East()->North()) {
        roomB->East()->North()->detach(EWest);
      }
      break;
    case EWest:
      if (roomB->North() && roomB->North()->East()) {
        roomB->North()->East()->detach(ESouth);
      }
      if (roomB->South() && roomB->South()->East()) {
        roomB->South()->East()->detach(ENorth);
      }
      break;
    case EEast:
      if (roomB->North() && roomB->North()->West()) {
        roomB->North()->West()->detach(ESouth);
      }
      if (roomB->South() && roomB->South()->West()) {
        roomB->South()->West()->detach(ENorth);
      }
      break;
    case ENone: /*fallthrough*/
    default:
      m_log->error("Invalid direction given");
      return 0;
  }

  // set new room neighbors for roomA
  m_log->debug("Setting room neighbors...");
  switch (dir) {
    case ENorth: {
      m_log->debug("nNorth: room %d", roomA->North()->getNo());
      // west of north of east
      if (roomA->East() && roomA->East()->North()) {
        roomA->East()->North()->detach(EWest);
        roomA->East()->North()->attach(roomB, EWest);
        m_log->info("Attached neighbor room %d to room %d", roomB->getNo(),
                roomA->East()->North()->getNo());
      }
      // east of north of west
      if (roomA->West() && roomA->West()->North()) {
        roomA->West()->North()->detach(EEast);
        roomA->West()->North()->attach(roomB, EEast);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->West()->North()->getNo());
      }
    } break;
    case ESouth: {
      // remember south neighbor
      m_log->debug("nSouth: room %d", roomA->South()->getNo());
      // west of south of east
      if (roomA->East() && roomA->East()->South()) {
        roomA->East()->South()->detach(EWest);
        roomA->East()->South()->attach(roomB, EWest);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->East()->South()->getNo());
      }
      // east of south of west
      if (roomA->West() && roomA->West()->South()) {
        roomA->West()->South()->detach(EEast);
        roomA->West()->South()->attach(roomB, EEast);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->West()->South()->getNo());
      }
    } break;
    case EWest: {
      // remember west neighbor
      m_log->debug("nWest: room %d", roomA->West()->getNo());
      // south of west of north
      if (roomA->North() && roomA->North()->West()) {
        roomA->North()->West()->detach(ESouth);
        roomA->North()->West()->attach(roomB, ESouth);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->North()->West()->getNo());
      }
      // north of west of south
      if (roomA->South() && roomA->South()->West()) {
        roomA->South()->West()->detach(ENorth);
        roomA->South()->West()->attach(roomB, ENorth);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->South()->West()->getNo());
      }
    } break;
    case EEast: {
      // remember east neighbor
      m_log->debug("nEast: room %d", roomA->East()->getNo());
      // south of east of north
      if (roomA->North() && roomA->North()->East()) {
        roomA->North()->East()->detach(ESouth);
        roomA->North()->East()->attach(roomB, ESouth);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->North()->East()->getNo());
      }
      // north of east of south
      if (roomA->South() && roomA->South()->East()) {
        roomA->South()->East()->detach(ENorth);
        roomA->South()->East()->attach(roomB, ENorth);
        m_log->info("Attached room %d to room %d", roomB->getNo(),
                roomA->South()->East()->getNo());
      }
    } break;
    default: m_log->alert("%s:%d Error: shouldn't ever get here", __FILE__, __LINE__);
  }

  m_log->debug("Attaching (post):");
  roomA->logConnections();
  roomB->logConnections();
  return 1;
}


LiList<Room>* Maze::getRoomList(LiList<Room> *list) {
  if (list) delete list;
  list = new LiList<Room>(*m_room_list);

  for (Room* r = m_room_list->Head(); r != NULL; r = m_room_list->Next()) {
    if (!r->isOpen()) {
      m_log->debug("Removing room%d from room list (full)", r->getNo());
      list->remove(r);
    }
  }
  return list;
}

// retrieve all rooms that are neither full nor isolated
LiList<Room>* Maze::getOpen(LiList<Room> *list) {
  if (list) delete list;
  list = new LiList<Room>(*m_room_list);

  for (Room* r = m_room_list->Head(); r != NULL; r = m_room_list->Next()) {
    if (!r->isOpen() || !r->isConnected()) {
      m_log->debug("Removing room%d from open room list (full | isolated)", r->getNo());
      list->remove(r);
    }
  }
  return list;
}

// retrieve all isolated rooms
LiList<Room>* Maze::getIsolated(LiList<Room> *list) {
  if (list) delete list;
  list = new LiList<Room>(*m_room_list);

  for (Room* r = m_room_list->Head(); r != NULL; r = m_room_list->Next()) {
    if (r->isConnected()) {
      m_log->debug("Removing room%d from isolated list (connected)", r->getNo());
      list->remove(r);
    }
  }
  return list;
}

// retrieve all fully connected rooms
LiList<Room>* Maze::getFull(LiList<Room> *list) {
  if (list) delete list;
  list = new LiList<Room>(*m_room_list);

  for (Room* r = m_room_list->Head(); r != NULL; r = m_room_list->Next()) {
    m_log->debug("looking at room %d", r->getNo());
    if (r->isOpen()) {
      m_log->debug("Removing room%d from full list (open slots)", r->getNo());
      list->remove(r);
    }
  }
  return list;
}

direction_e Maze::getRandDir() {
  return (direction_e)((rand() % 4) + 1);
}

void Maze::logList(LiList<Room> *list) {
  if (list->Size() < 1) return;
  m_log->debug("Logging list of size %d", list->Size());
  for (Room *r = list->Head(); r != NULL; r = list->Next()) {
    m_log->debug("Room %d", r->getNo());
    r->logConnections();
  }
    m_log->debug("-------");
}
