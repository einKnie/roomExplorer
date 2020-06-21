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
#ifndef _MAZE_ENGINE_H_
#define _MAZE_ENGINE_H_

#include "roomExplorer.h"
#include "maze.h"
#include "player.h"
#include "ui.h"

typedef enum {
  EActionNone,
  EMoveNorth,
  EMoveSouth,
  EMoveWest,
  EMoveEast,
  EInventory,
  ELook,
  EFetchItem,
  ERunAway,
  EQuit,
  ENextLevel,
  EScrollDown = EMoveSouth,
  EScrollUp   = EMoveNorth
} action_e;   ///< user action enumeration

// FIXME: order of items is very important atm
typedef enum {
  SIdle,
  SFight,
  SLoss,
  SWin,
  SStart,
  SLevelUp,
  SQuit
} gameState_e;

class MazeEngine {
private:
  Logger *m_log;                ///< logger
  bool    m_createdLog;         ///< flag to keep or remove logger on destruction
  Player *m_player;             ///< player
  Maze   *m_maze;               ///< maze
  MazeUI *m_ui;                 ///< ui

  unsigned int     m_seed;        ///< global seed for random generation

  static const int CMaxLevels               = 20;   ///< max levels in game
  static const int CBaseRoomNumber          = 10;   ///< starting number of rooms per level
  static const int CRoomIncStep             = 2;    ///< additional rooms per level
  static const int CMaxBufLen               = 500;  ///< max buffer length for logs

  /// @brief Get one char of user input
  /// @return the user action or EActionNone
  /// @see action_e
  action_e getInput();

  /// @brief Translate an action to a movement direction
  /// @param[in] action
  /// @return a direction or ENone if no movment action was provided
  direction_e getMovDirFromAction(action_e action);

  /// @brief Enter a given room
  ///
  /// This draws the room and prints the room's entry text
  /// @param[in] room a room
  void enterRoom(Room *room);

  /// @brief Show the inventory
  /// @return 1 on inventory closed, 0 if user quit game
  int showInv();

  /// @brief Simulate a player attack
  /// @return raw damage dealt
  int  playerAtk();

  /// @brief Simulate the player taking damage
  /// @param[in] dmg the raw damage dealt to the player
  void playerDmg(int dmg);

  /// @brief Simulate a monster attack
  /// @return raw damage dealt
  int  monsterAtk(Monster *mon);

  /// @brief Simulate the monster taking damage
  /// @param[in] dmg the raw damage dealt to the monster
  void monsterDmg(Monster *mon, int dmg);

  /// @brief Simulate running away from the fight
  void runAway();

  /// @brief Wait for user input (specifically ENTER)
  void waitForEnter();

  /// @brief Run the game menu
  /// @return SLevelUp or SQuit, depending on user input
  gameState_e runMenu();

  /// @brief Run the game in room mode
  /// @param [in] action the user's current action
  /// @param [in,out] room, the current room, may change due to user input
  /// @return SIdle, SFight, SQuit depending on user intput and chance
  gameState_e runIdle(action_e action, Room **room);

  /// @brief Run the game in fight mode
  /// @param [in] action the user's current action
  /// @param [in] room the current room
  /// @return Sidle, SFight, SLoss, SQuit depending on user input and chance
  gameState_e runFight(action_e action, Room **room);

  /// @brief Use an item
  /// @param[in] room the current room
  /// @return SIdle, SFight depending on chance
  gameState_e useItemS(Room *room);

  /// @brief Simulate the player looking around
  /// @param [in] room the room to look around in
  /// @return SIdle, SFight depending on chance
  gameState_e lookAroundS(Room *room);

  /// @brief show the loss screen and get user input
  /// @return EStart, EQuit depending on user input
  gameState_e showLoss();

  /// @brief show the win screen and get user input
  /// @return EStart, EQuit depending on user input
  gameState_e showWin();

public:
  MazeEngine(Logger *log);
  ~MazeEngine();

  /// @brief Initialize screen
  /// @note must be called, game cannot run if this returns false
  /// @return true if all is well, false on error
  int init();

  /// @brief Run the game
  int runGame();

};

#endif //!_MAZE_ENGINE_H_
