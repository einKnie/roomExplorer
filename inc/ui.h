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
#ifndef _MAZE_UI_H_
#define _MAZE_UI_H_

#include <termios.h>
#include "roomExplorer.h"
#include "player.h"
#include "room.h"

#define ARROWKEY '\033'  ///< macro to detect arrow key presses

enum {
  EScreenStart,
  EScreenRoom,
  EScreenInv,
  EScreenFight,
  EScreenLoss,
  EScreenWin,
  EScreenLog,
  EScreenQuit,
  EScreenStartFadeIn
};

/// @brief User Interface class
class MazeUI {

private:

  static const int CMinScreenWidth          = 80;
  static const int CMaxBufLen               = 500;  ///< max buffer length for logs
  static const int CMaxInvWidth             = 100;  ///< max inventory width
  static const int CMaxTextLen              = 100;  ///< max length of any text in output
  static const int CMaxLogWidth             = 50;   ///< max length of log output
  static const int CMaxControlLen           = 50;   ///< max length of controls field
  static const int CMaxInfoLen              = 30;   ///< max length of info field
  static const int CMaxItemName             = 80;   ///< max item name len per line in inventory
  static const int CMaxUiHeight             = 20;   ///< max lines of fight text before scrolling up
  static const int CMaxInvHeight            = CMaxUiHeight - 2;
  static const int CMaxRoomHeight           = 20;   ///< max height of a room
  static const int CSepWidth                = 3;    ///< width of the separator in output
  static const int CDefaultMonsterWidth     = 50;   ///< width of default monster image
  static const int CMaxItems                = 30;

  static const int CMaxSplitLines           = 10;   ///< max number of lines to split long text into

  static char m_startField  [][CMaxInvWidth];       ///< field for start screen
  static char m_endField    [][CMaxInvWidth];       ///< field for end/win/loss screen
  static char m_monsterField[][CMaxInvWidth];       ///< field for monster image
  static char m_itemStats[];
  static char m_sep[];

  char m_space[CMaxInvWidth];
  char m_padding[CMaxInvWidth];

  char m_fightField    [CMaxUiHeight][CMaxInvWidth];    ///< scroll text for fight screen
  char m_inventoryField[CMaxUiHeight][CMaxInvWidth];    ///< inventory field
  char m_roomField     [CMaxRoomHeight][CMaxInvWidth];  ///< room field
  char m_logField      [CMaxUiHeight][CMaxLogWidth];    ///< scroll text for log screen
  char m_startFieldFade[CMaxUiHeight][CMaxInvWidth];    ///< experimental, fade-in start screen
  char m_itemList      [CMaxUiHeight * CMaxSplitLines * CMaxItems][CMaxInvWidth];

  Logger *m_log;                ///< logger
  Player *m_player;             ///< player class

  struct termios m_orig;        ///< original terminal settings

  unsigned int m_logLines;      ///< current lines of output in log screen
  unsigned int m_fightLines;    ///< current lines of output in fight screen
  unsigned int m_inventoryLen;  ///< current number of items in inventory

  unsigned int m_screenWidth;   ///< total screen width
  unsigned int m_fieldWidth;    ///< max width of any field
  unsigned int m_invWidth;      ///< max width of inventory (i.e. all 'inner' output)
  unsigned int m_roomWidth;     ///< max room width
  unsigned int m_monsterWidth;
  unsigned int m_logWidth;      ///< max width of the log field
  unsigned int m_fightWidth;    ///< max width of fight log field

  /// @brief Prepare the terminal for the game
  /// @rturn 1 on success, 0 on failure
  int prepTerminal();

  /// @brief Reset terminal to original settings
  /// @return 1 on success, 0 on failure
  int resetTerminal();

  /// @brief Create the room field for room
  /// @param [in] room the room to draw
  void createRoomField(Room *room);

  /// @brief Create the inventory field
  /// @param [in,out] startIdx index of first line in itemList to print
  void createInvField(unsigned int *startIdx);

  /// @brief Get the current scrren width
  /// @return the currently set screen width in columns
  int getCurrentScreenWidth();

  /// @brief Calculate gae screen sizes depending on terminal screen size
  bool calcScreenSizes();

  /// @brief Split \a line into \a n chunks of max size \a len
  /// @param [in] line the line to split
  /// @param [in,out] lines hold the split lins after call
  /// @param [in] len max length of a line in output
  /// @param [in,out] n max number of lines to split into on input, holds number of outlut lines after call
  void splitLine(const char *line, char lines[][CMaxInvWidth], unsigned int len, int *n);

  /// @brief Show game menu screen with fade in
  /// @note: EXPERIMENTAL, looks horrid
  void screenFadeIn();

public:

  MazeUI(Logger *log, Player *player);
  ~MazeUI();

  /// @brief Initialize screen
  /// @note must be called, game cannot run if this returns false
  /// @return true if all is well, false on error
  int init();

  /// @brief Add a line to the fight log
  /// @param[in] line the line to add
  /// @note if the max number of lines is reached, the text scrolls
  /// @note the line is split into multiple lines if too long
  void addFightLine(const char *line);

  /// @brief Add a line to the regular log
  /// @param[in] line the line to add
  /// @note if the max number of lines is reached, the text scrolls
  /// @note the line is split into multiple lines if too long
  void addLogLine(const char *line);

  /// @brief Remove the last logged line
  void removeLogLine();

  /// @brief Clear a screen field
  void clearScreen(int type);

  /// @brief Write the full list of items
  void writeItemList();

  /// @brief Show a screen field
  /// @param type the type of screen to show
  /// @param data optional data, depend on type of screen
  void showScreen(int type, void *data = NULL);

};

#endif //_MAZE_UI_H_
