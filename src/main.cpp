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
#include "engine.h"
#include "roomExplorer.h"

int main (void) {

  Logger *log = new Logger("maze.log", CfgLog::ELogDebug, CfgLog::ELogProfileDefault);
  MazeEngine *game = new MazeEngine(log);

  if (game->init()) {
    game->runGame();
    system("clear");
  } else {
    printf("Sorry, your terminal window is too small to support this game.\n");
  }

  delete game;
  delete log;
  return 0;
}
