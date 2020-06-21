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
#include "room_factory.h"
#include <time.h>

// default constructor
MazeEngine::MazeEngine(Logger *log) {

  if (log) {
    m_log = log;
    m_createdLog = false;
  } else {
    m_log = new Logger();
    m_createdLog = true;
  }
  m_seed = time(NULL);
  m_player = new Player(m_log);
  m_ui = new MazeUI(m_log, m_player);
  m_maze = NULL;
}

MazeEngine::~MazeEngine() {

  if (m_ui) {
    m_log->debug("removing ui");
    delete m_ui;
    m_ui = NULL;
  }

  if (m_maze) {
    m_log->debug("removing maze");
    delete m_maze;
    m_maze = NULL;
  }

  if (m_player) {
    m_log->debug("removing player");
    delete m_player;
    m_player = NULL;
  }

  if (m_createdLog && m_log) {
    delete m_log;
    m_log = NULL;
  }
}

int MazeEngine::init() {
  return m_ui->init();
}

gameState_e MazeEngine::runMenu() {
  bool show = true;
  action_e action = EActionNone;
  gameState_e state = SStart;

  m_ui->showScreen(EScreenStart);
  // screenFadeIn();    // experimental!
  while (show) {
    action = getInput();
    switch(action) {
      case ENextLevel: state = SLevelUp; show = false; break;
      case EQuit:      state = SQuit;    show = false; break;
      default: break;
    }
  }
  return state;
}

int MazeEngine::runGame() {
  unsigned int level = 0;
  LiList<Room>* levels[CMaxLevels] = {NULL};
  int nRooms = CBaseRoomNumber;
  action_e action = EActionNone;
  RoomFactory *fac = NULL;
  Room *current_room = NULL;
  gameState_e state = SStart;

  while (state != SQuit) {

    if (state == SLevelUp) {
      if (level == CMaxLevels) {
        // end of game
        state = SWin;
        break;
      }

      if ((level > 1) && (CMaxLevels % (level + 1) == 0)) {
        m_log->debug("increasing number of rooms at level %d", level);
        nRooms += CRoomIncStep;
      }

      // cleanup previous level
      if (m_maze) delete m_maze;
      if (fac)  delete fac;

      fac = new RoomFactory(m_log, nRooms);
      levels[level] = fac->createRoomList();
      if (!levels[level]) {
        m_log->error("Failed to create rooms. aborting");
        state = SQuit;
        continue;
      }
      m_log->debug("Created %d rooms for level %d", levels[level]->Size(), level);

      // assemble maze
      m_maze = new Maze(m_seed, m_log, levels[level]);
      current_room = m_maze->createLinearMaze();
      m_log->debug("Finished creating maze");
      m_log->info("starting level %d", ++level);

      // clear screen and heal player on levelup
      m_ui->clearScreen(EScreenLog);
      m_player->restoreHealth();

      if (level > 1)
        m_ui->addLogLine("You feel refreshed");

      state = SIdle;
      enterRoom(current_room);
    }

    // single level
    do {
      m_log->debug("current state: %d", (int)state);
      switch (state) {
        case SStart:
          // game start state
          // come from: SLoss, SWin
          // lead to:   SLevelUp
          state = runMenu();
          break;
        case SIdle:
          // come from: SLevelUp, SIdle, SFight
          // lead to:   SLevelUp, SIdle, SFight, SQuit
          {
            state = runIdle(action, &current_room);
            m_ui->showScreen(EScreenRoom, current_room);
            if (state == SFight) {
              Room *oldRoom = current_room;
              waitForEnter();
              runFight(EActionNone, &current_room);
              m_ui->showScreen(EScreenFight, (void*)oldRoom->getMonster());
            }
          }
          break;
        case SFight:
          // come from: SIdle, SFight
          // lead to:   SIdle, SFight, SLoss, SQuit
          {
            Room *oldRoom = current_room;
            state = runFight(action, &current_room);
            m_ui->showScreen(EScreenFight, (void*)oldRoom->getMonster());
            if (state != SFight) {
              waitForEnter();
              m_ui->showScreen(EScreenRoom, current_room);
            }
          }
          break;
        case SLoss:
          // come from: SFight
          // lead to:   SStart, SQuit
          state = showLoss();
          break;
        case SWin:
          // come from: SLevelUp
          // lead to:   SStart, SQuit
          state = showWin();
          break;
        case SQuit:     /*fallthrough*/
        case SLevelUp:  /*fallthrough*/
        default:
          m_log->warning("invalid state reached! (%d)", state);
          state = SQuit; // hart aber fair
          break;
      }

    } while (((state < SLoss) && (action = getInput())) || (state < SStart));

    if (state == SLevelUp) {
      m_log->info("moving on to next level...");
    }
  }

  m_ui->showScreen(EScreenQuit);
  waitForEnter();

  // cleanup
  m_log->info("player has quit/finished the game");
  delete m_maze;
  m_maze = NULL;
  delete fac;
  fac = NULL;

  // delete all rooms (incl items)
  for (int i = 0; i < CMaxLevels; i++) {
    if (levels[i]) {
      levels[i]->free();
      delete levels[i];
      levels[i] = NULL;
    }
  }
  return 1;
}

int MazeEngine::showInv() {
  // inventory subroutine
  action_e act = EActionNone;
  bool show = true;
  unsigned int idx = 0;
  m_ui->writeItemList();

  while (show) {
    m_ui->showScreen(EScreenInv, &idx);
    act = getInput();
    switch (act) {
      case ENextLevel:  show = false; break;
      case EQuit:       return 0;
      case EScrollDown: ++idx; break;
      case EScrollUp:   if (idx) --idx; break;
      default: break;
    }
  }
  return 1;
}

gameState_e MazeEngine::showLoss() {
  gameState_e state = SQuit;
  action_e act = EActionNone;
  bool show = true;

  m_ui->showScreen(EScreenLoss);
  while (show) {
    act = getInput();
    switch (act) {
      case ENextLevel: state = SQuit;  show = false; break;
      case ELook:      state = SStart; show = false; break;
      default: break;
    }
  }
  return state;
}

gameState_e MazeEngine::showWin() {
  gameState_e state = SQuit;
  action_e act = EActionNone;
  bool show = true;

  m_ui->showScreen(EScreenWin);
  while (show) {
    act = getInput();
    switch (act) {
      case ENextLevel: state = SQuit;  show = false; break;
      case ELook:      state = SStart; show = false; break;
      default: break;
    }
  }
  return state;
}

gameState_e MazeEngine::runIdle(action_e action, Room **room) {
  gameState_e state = SIdle;

  // handle action
  direction_e dir = getMovDirFromAction(action);
  if (dir != ENone) {
    Room *next_room = (*room)->move(dir);
    if (!next_room) {
      m_ui->addLogLine("You cannot got that way");
    } else {
      *room = next_room;
      enterRoom(*room);
    }
  } else {
    switch (action) {
      case EInventory:  if (!showInv()) { state = SQuit; } break;
      case ELook:       state = lookAroundS(*room);  break; // might lead to fight
      case EFetchItem:  state = useItemS(*room); break; // might lead to fight
      case ENextLevel:  if ((*room)->foundEnd()) state = SLevelUp; break;
      case EQuit:       state = SQuit; break;
      default:          m_log->info("unhandled action in runIdle()"); break; // do nothing
    }
  }

  return state;
}

gameState_e MazeEngine::runFight(action_e action, Room **room) {
  static unsigned int round = 0;
  gameState_e state = SFight;
  Monster *mon = (*room)->getMonster();
  char line[CMaxBufLen];

  // prepare new fight (monster always attack first)
  if (!round++) {
    m_ui->clearScreen(EScreenFight);
    playerDmg(monsterAtk(mon));
    m_ui->addFightLine(" ");
    if (!m_player->isAlive()) {
      sprintf(line, "You Died! The %s looks at your lifeless body and is content.", mon->getName());
      m_ui->addFightLine(line);
      state = SLoss;
    }
  } else {
    bool nextRound = false;
    // eval action
    switch (action) {
      case EInventory:
        if (!showInv()) { state = SQuit; }
        nextRound = false;
        break;
      case EFetchItem:
        monsterDmg(mon, playerAtk());
        if (!mon->isAlive()) {
          m_log->debug("player defeated %s", mon->represent());
          sprintf(line, "You defeated the vicious %s in glorious battle!", mon->getName());
          m_ui->addFightLine(line);
          state = SIdle;
          nextRound = false;
        } else {
          if (!m_player->getActiveItem() || !m_player->getActiveItem()->Health()) {
            m_player->setActiveItem();
            if (!m_player->getActiveItem()) {
              sprintf(line, "Panic! you don't have any items left!");
            } else {
              sprintf(line, "You randomly grab another item from your backpack. It turns out to be %s",
                        m_player->getActiveItem()->represent());
            }
            m_ui->addFightLine(line);
          }
          nextRound = true;
        }
        break;
      case ERunAway:
        runAway();
        m_ui->addFightLine("You run in a random direction to escape the monster...");
        {
          Room *tmp = m_maze->getRandom();
          *room = tmp ? tmp : *room;
        }
        state = SIdle;
        nextRound = false;
        break;
      case EQuit:
        m_ui->addFightLine("");
        m_ui->addFightLine("Quit the game");
        state = SQuit;
        nextRound = false;
        break;
      default:
        m_ui->addFightLine("The monster capitalizes on your indescision and attacks again!");
        nextRound = true;
        break;
    }

    // next round monster attack
    if (nextRound) {
      playerDmg(monsterAtk(mon));
      m_ui->addFightLine(" ");
    }

    if (!m_player->isAlive()) {
      m_log->debug("player DIED");
      sprintf(line, "You Died! The %s looks at your lifeless body and is content.", mon->getName());
      m_ui->addFightLine(line);
      state = SLoss;
    }
  }

  if (state != SFight) {
    round = 0;
    m_ui->removeLogLine(); // remove the "[enter to continue]" from regular log
    m_ui->addFightLine("[enter to continue]");

    if (!mon->isAlive()) {
      m_ui->addLogLine("You should look around again, in case the monster dropped something!");
    }
  }
  return state;
}

gameState_e MazeEngine::useItemS(Room *room) {
  gameState_e state = SIdle;
  char line[CMaxBufLen];
  m_player->setActiveItem();
  if (!m_player->getActiveItem()) {
    m_ui->addLogLine("You don't have any items to use.");
    return state;
  } else {
    sprintf(line, "You randomly grabbed an item from your backpack. It turns out to be %s!", m_player->getActiveItem()->represent());
    m_ui->addLogLine(line);
  }

  int ret = m_player->useItem();

  sprintf(line, "You used your %s%s and dealt %d damage to %s.",
                  m_player->getActiveItem()->name, room->hasAliveMonster() ? "" : " in an empty room",
                  ret, room->hasAliveMonster() ? room->getMonsterName() : "nothing..");
  m_ui->addLogLine(line);

  if (!m_player->getActiveItem()->Health()) {
    sprintf(line, "Oh no! Looks like your %s broke!", m_player->getActiveItem()->name);
    m_ui->addLogLine(line);
    m_player->loseItem(m_player->getActiveItem());
  }

  if (!room->hasAliveMonster()) return state;

  if (!room->getMonster()->subHealth(ret)) {
    sprintf(line, "The %s died from your hit.", room->getMonsterName());
    m_ui->addLogLine(line);
    sprintf(line, "(...who is the real monster here?)");
    m_ui->addLogLine(line);
  } else {
    sprintf(line, "The %s fights back!", room->getMonsterName());
    m_ui->addLogLine(line);
    m_ui->addLogLine("[enter to continue]");
    state = SFight;
  }

  return state;
}

gameState_e MazeEngine::lookAroundS(Room *room) {
  gameState_e state = SIdle;
  bool found_anything = false;
  char line[CMaxBufLen];
  m_log->debug("Looking around room %d", room->getNo());
  m_ui->addLogLine("You look around the room...");
  unsigned int ret = room->look();

  if (ret & Room::CFoundMonster) {
    found_anything = true;
    m_log->debug("found monster");
    sprintf(line, "There is a %s in this room!", room->getMonsterName());
    m_ui->addLogLine(line);
    if (room->getMonster()->isShy()) {
      sprintf(line, "The %s is shy.", room->getMonsterName());
      m_ui->addLogLine(line);
      m_ui->addLogLine("It will not attack you. To initiate a fight, use an item in this room.");
    } else {
      m_player->setActiveItem();
      m_ui->addLogLine("The monster attacks!");
      m_ui->addLogLine("[enter to continue]");
      state = SFight;
    }
    return state;
  }

  if (ret & Room::CFoundItem) {
    found_anything = true;
    m_log->debug("found item");
    Item *item = room->getItem();
    sprintf(line, "You found an item: %s!", item ? item->name : "gotcha! no item");
    m_ui->addLogLine(line);
    m_player->addItem(item);
  }

  if (ret & Room::CFoundGold) {
    found_anything = true;
    m_log->debug("found gold");
    int gold = room->getGold();
    sprintf(line, "You found %d gold!", gold);
    m_ui->addLogLine(line);
    m_player->addGold(gold);
  } else if (room->hasGold()) {
      m_ui->addLogLine("You find no gold... but you do remember picking up some gold here earlier.");
  }

  if (room->isEnd()) {
    found_anything = true;
    m_ui->addLogLine("This room appears to lead to a different dungeon!");
    m_ui->addLogLine("[press enter to go to next level]");
  }

  if (!found_anything) {
    m_ui->addLogLine("You see nothing out of the ordinary.");
  }

  return state;
}

action_e MazeEngine::getInput() {
  char in = '\0';
  action_e action = EActionNone;

  if (read(STDIN_FILENO, &in, 1) < 1) {
    m_log->error("failed to read from stdin\n");
  } else {
    switch(in) {
      case 'w': action = EMoveNorth; break;
      case 'a': action = EMoveWest; break;
      case 's': action = EMoveSouth; break;
      case 'd': action = EMoveEast; break;
      case 'i': action = EInventory; break;
      case ' ': action = ELook; break;
      case 'e': action = EFetchItem; break;
      case 'r': action = ERunAway; break;
      case 'q': action = EQuit; break;
      case '\n': action = ENextLevel; break;
      case ARROWKEY: {
        if (read(STDIN_FILENO, &in, 1) == 1) { // skip over middle symbol '['
          if (read(STDIN_FILENO, &in, 1) == 1) {
            switch(in) {
              case 'A': action =  EMoveNorth; break;
              case 'B': action =  EMoveSouth; break;
              case 'C': action =  EMoveEast; break;
              case 'D': action =  EMoveWest; break;
              default: action = EActionNone;
            }
          }
        }
      } break;
      default: action = EActionNone;
    }
  }
  return action;
}

direction_e MazeEngine::getMovDirFromAction(action_e action) {
  direction_e dir = ENone;
  switch(action) {
    case EMoveNorth: dir = ENorth; break;
    case EMoveSouth: dir = ESouth; break;
    case EMoveWest: dir = EWest; break;
    case EMoveEast: dir = EEast; break;
    default: dir = ENone;
  }
  return dir;
}

void MazeEngine::enterRoom(Room *room) {
  m_ui->addLogLine(room->getEntry());
  m_ui->showScreen(EScreenRoom, (void*)room);
}

void MazeEngine::waitForEnter() {
  bool show = true;
  fflush(stdin);
  while (show) {
    action_e action = getInput();
    switch(action) {
      case ENextLevel: show = false; break;
      default: break;
    }
  }
}

int MazeEngine::playerAtk() {
  int ret = 0;
  char line[CMaxBufLen] = "";

  if (!m_player->getActiveItem()) {
    m_ui->addFightLine("You don't have any items to use");
  } else {
    ret = m_player->useItem();
    sprintf(line, "You used your %s and dealt %d raw damage", m_player->getActiveItem()->name, ret);
    m_ui->addFightLine(line);

    if (!m_player->getActiveItem()->Health()) {
      sprintf(line, "Oh no! Looks like your %s broke!", m_player->getActiveItem()->name);
      m_ui->addFightLine(line);
      m_player->loseItem(m_player->getActiveItem());
    }
  }

  return ret;
}

void MazeEngine::playerDmg(int dmg) {
  char line[CMaxBufLen] = "";
  int actDmg = m_player->takeDamage(dmg);

  if (!m_player->isAlive()) {
    m_ui->addFightLine("Oh no! The monster killed you");
  } else {
    sprintf(line, "You took %d damage from the attack", actDmg);
    m_ui->addFightLine(line);
  }
}

int MazeEngine::monsterAtk(Monster *mon) {
  int ret = 0;
  char line[CMaxBufLen] = "";
  Attack *atk = NULL;

  if (!mon->hasAttacks()) {
    m_ui->addFightLine("The monster does not know how to fight...");
  } else {
    atk = mon->getAttack();
    ret = mon->attack(atk);
    sprintf(line, "The monster used %s and dealt %d raw damage", atk->getName(), ret);
    m_ui->addFightLine(line);
  }

  return ret;
}

void MazeEngine::monsterDmg(Monster *mon, int dmg) {
  char line[CMaxBufLen] = "";
  int actDmg = mon->takeDamage(dmg);

  if (!mon->isAlive()) {
    m_ui->addFightLine("You killed the monster! ...yay, I guess?");
  } else {
    sprintf(line, "The monster took %d damage", actDmg);
    m_ui->addFightLine(line);
  }
}

void MazeEngine::runAway() {
  m_ui->addFightLine("You run away like the coward you are...");
}
