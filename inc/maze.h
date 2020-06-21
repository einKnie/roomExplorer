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
#ifndef _MAZE_H_
#define _MAZE_H_

#include "room.h"
#include "lilist.h"
#include "roomExplorer.h"

/// @brief Class definition of the Maze
class Maze {
private:
  int           m_seed;               ///< random generator seed
  LiList<Room> *m_room_list;          ///< internal list of rooms
  Logger       *m_log;                ///< logger
  bool          m_destroy_roomlist;   ///< flag set to keep or delete room list on destruction

  /// Return all rooms that are not fully connected
  LiList<Room>* getRoomList(LiList<Room> *list);
  /// Return all rooms which are neither full nor isolated
  LiList<Room>* getOpen(LiList<Room> *list);
  /// Return all rooms that are isolated
  LiList<Room>* getIsolated(LiList<Room> *list);
  /// Return all rooms that are fully connected
  LiList<Room>* getFull(LiList<Room> *list);

  /// Return  random direction
  direction_e  getRandDir();

  /// Log list stats
  void logList(LiList<Room> *r);

public:
  /// Default constructor
  /// @param[in] s seed
  /// @param[in] log, logger
  /// @param[in] rooms [optional] a premade list of rooms
  /// @note This creates a list of rooms automatically if \a rooms is NULL
  Maze(int s, Logger *log, LiList<Room> *rooms = NULL);

  /// Copy constructor
  Maze(Maze *m);
  ~Maze();

  /// @brief Return number of rooms
  int size();

  /// @brief Get a random room from list
  /// @pram[in] @optional open only return rooms with open connections
  /// @return a random room or NULL if no room was found
  Room *getRandom(bool open = false);

  /// @brief Debug print all rooms (identifiers) in list
  /// @todo think about whether i can replace this by the room's own logging
  void listRooms();

  /// @brief check is any isolated rooms exist
  /// @return true if isolated rooms exist, false otherwise
  bool anyIsolated();

  /// @brief Create a standard dungeon
  /// @return start room
  Room *createLinearMaze();

  /// @brief Attach a room to another room
  /// @param[in] roomA the attached
  /// @param[in] roomB the attachee
  /// @param[in] dir direction to attach \a roomB to \a roomA in
  /// @return 1 if attachment was successful, 0 otherwise
  /// @todo can this be private?
  int attachRoom(Room *roomA, Room *roomB, direction_e dir);

};

#endif // _MAZE_H_
