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
#include "ui.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

char MazeUI::m_startField[][CMaxInvWidth] = {
    {"   _____"},
    {"  |  __ \\"},
    {"  | |__) |___   ___  _ __ ___"},
    {"  |  _  // _ \\ / _ \\| '_ ` _ \\"},
    {"  | | \\ \\ (_) | (_) | | | | | |"},
    {"  |_|__\\_\\___/ \\___/|_| |_| |_|"},
    {"  |  ____|          | |"},
    {"  | |__  __  ___ __ | | ___  _ __ ___ _ __"},
    {"  |  __| \\ \\/ / '_ \\| |/ _ \\| '__/ _ \\ '__|"},
    {"  | |____ >  <| |_) | | (_) | | |  __/ |"},
    {"  |______/_/\\_\\ .__/|_|\\___/|_|  \\___|_|"},
    {"              | |"},
    {"              |_|"}
};

char MazeUI::m_endField[][CMaxInvWidth] = {
  {"        .-\"\"\"\"."},
  {"       /       \\"},
  {"   __ /   .-.  .\\"},
  {"  /  `\\  /   \\/  \\"},
  {"  |  _ \\/   .==.==."},
  {"  | (   \\  /____\\__\\"},
  {"   \\ \\      (_()(_()"},
  {"    \\ \\            '---._"},
  {"     \\                   \\_"},
  {"  /\\ |`       (__)________/"},
  {" /  \\|     /\\___/"},
  {"|    \\     \\||VV"},
  {"|     \\     \\|\"\"\"\","},
  {"|      \\     ______)"},
  {"\\       \\  /`"},
  {"         \\("}
};

char MazeUI::m_monsterField[][CMaxInvWidth] = {
  {"                               .----.__"},
  {"        _.       ._           /---.__  \\"},
  {"        |\\\\___  //|          /       `\\ |"},
  {"        | )   `(/ |         | o     o  \\|"},
  {"        |/      \\ |  /|   ./| .vvvvv.  |\\"},
  {"        / _   _  `|_ ||  / /| |     |  | \\"},
  {"        | O)  O   ) \\|| //' | `^vvvv'  |/\\\\"},
  {"        \\        /  | | ~   \\          |  \\\\"},
  {"       / \\ Y   /'   | \\     |          |   ~"},
  {"       |  `._/' |   |  \\     7        /"},
  {"   -'-'|  |`-._/   /    \\ _ /    .    |"},
  {"       \\  \\   .   / \\_.  \\ -|_/\\/ `--.|_"},
  {"        \\  \\ |   /    |  |              `-"},
  {"         \\uU \\UU/     |  /"}
};

char MazeUI::m_itemStats[] =  "DMG | MOOD | HEL";
char MazeUI::m_sep[] = " | ";

// default constructor
MazeUI::MazeUI(Logger *log, Player *player) {
  m_log = log;
  m_player = player;
  memset(m_padding, '=', sizeof(m_padding));
  memset(m_space, ' ', sizeof(m_space));
}

MazeUI::~MazeUI() {
  if (!resetTerminal()) {
    m_log->error("Failed to reset terminal");
  }
}

int MazeUI::init() {
  if (!prepTerminal()) {
    m_log->error("Faled to set terminal");
    return false;
  }

  // calculate ui screen sizes depending on terminal screen size
  if (!calcScreenSizes()) {
    m_log->error("terminal screen is too small for game");
    return false;
  }
  return true;
}

int MazeUI::prepTerminal() {
  struct termios term_new;
  m_log->info("Changing terminal settings...");

  if (tcgetattr(0, &term_new) != 0) {
    m_log->error("Failed to get terminal settings");
    return 0;
  }

  m_orig = term_new;
  term_new.c_lflag &= ~ECHO;    // turn off echoing input
  term_new.c_lflag &= ~ICANON;  // turn off reading per line
  term_new.c_cc[VMIN] = 1;      // min length = 0
  term_new.c_cc[VTIME] = 0;     // min time = 0 -> effectively setting read to nonblocking

  if (tcsetattr(0, TCSANOW, &term_new) != 0) {
    m_log->error("Failed to set new terminal settings");
    return 0;
  }

  m_log->info("Successfully set new terminal settings");
  return 1;
}

int MazeUI::resetTerminal() {
  if (tcsetattr(0, TCSANOW, &m_orig) != 0) {
    m_log->error("Failed to reset terminal settings");
    return 0;
  }
  return 1;
}

bool MazeUI::calcScreenSizes() {

  int screen_width = getCurrentScreenWidth();
  m_log->debug("screensize: %d", screen_width);
  if (screen_width < CMinScreenWidth) return false;
  if (screen_width > CMaxInvWidth) screen_width = CMaxInvWidth;

  // calculate all different widths
  m_screenWidth = screen_width;
  m_fieldWidth  = screen_width - (2 * CSepWidth);
  m_invWidth    = m_fieldWidth;

  if (((m_fieldWidth - CSepWidth) / 2) < ROOM_WIDTH) {
    m_roomWidth = ROOM_WIDTH;
  } else {
    m_roomWidth = (m_fieldWidth - CSepWidth) / 2;
  }

  m_logWidth = m_fieldWidth - m_roomWidth - CSepWidth;
  m_monsterWidth = CDefaultMonsterWidth;
  m_fightWidth = m_fieldWidth - m_monsterWidth - CSepWidth;

  m_log->debug("Got game screen size %d; fieldWidth %d; roomWidth %d; logWidth %d, fightWidth: %d",
                m_screenWidth, m_fieldWidth, m_roomWidth, m_logWidth, m_fightWidth);
  return true;
}

void MazeUI::clearScreen(int type) {
  switch (type) {
    case EScreenRoom:   memset(m_roomField, '\0', sizeof(m_roomField)); break;
    case EScreenInv:    memset(m_inventoryField, '\0', sizeof(m_inventoryField)); break;
    case EScreenFight:  memset(m_fightField, '\0', sizeof(m_fightField)); m_fightLines = 0; break;
    case EScreenLog:    memset(m_logField, '\0', sizeof(m_logField)); m_logLines = 0; break;
    case EScreenStart:  /*fallthrough*/
    case EScreenLoss:   /*fallthrough*/
    case EScreenWin:    /*fallthrough*/
    case EScreenQuit:   /*fallthrough*/
    default: return;
  }
}

void MazeUI::showScreen(int type, void *data) {
  char line[CMaxInvWidth];
  memset (line, '\0', sizeof(line));

  char descr[CMaxTextLen];
  char controls[CMaxControlLen];
  char infobar[CMaxInfoLen];
  char infobar2[CMaxInfoLen];
  char (*field)[CMaxInvWidth];

  int fieldHeight = CMaxUiHeight;

  switch(type) {
    case EScreenRoom:
      if (!data) return;
      createRoomField((Room*)data);
      sprintf(descr,    "Traversing the maze...");
      sprintf(controls, "Move w/a/s/d%sUse e%sLook space%sQuit q", m_sep, m_sep, m_sep);
      sprintf(infobar,  "health: %3d", m_player->getHealth());
      sprintf(infobar2, "gold: %3d", m_player->getGold());
      field = m_roomField;
      break;
    case EScreenInv:
    if (!data) return;
      createInvField((unsigned int*)data);
      sprintf(descr,    "Inventory");
      sprintf(controls, "Close i%sQuit q", m_sep);
      sprintf(infobar,  "health: %3d", m_player->getHealth());
      sprintf(infobar2, "gold: %3d", m_player->getGold());
      field = m_inventoryField;
      break;
    case EScreenFight:
      if (!data) break;
      sprintf(descr,    "A FIGHT! Against who? %s!", ((Monster*)data)->represent());
      sprintf(controls, "Attack e%sRun Away r", m_sep);
      sprintf(infobar,  "health: %3d", m_player->getHealth());
      sprintf(infobar2, "monster health: %3d", ((Monster*)data)->getHealth());
      field = m_fightField;
      break;
    case EScreenStart:
      sprintf(descr,    "ROOM EXPLORER");
      sprintf(controls, "Start enter%sQuit q", m_sep);
      sprintf(infobar,  "<%s>", AUTHOR_STRING);
      sprintf(infobar2, "v%s", VERSION_STRING);
      field = m_startField;
      fieldHeight = (sizeof(m_startField) / (sizeof(char) *CMaxInvWidth));
      break;
    case EScreenLoss:
      sprintf(descr,    "YOU DIED");
      sprintf(controls, "Quit enter%sNew Game space", m_sep);
      sprintf(infobar,  "<%s>", AUTHOR_STRING);
      sprintf(infobar2, "v%s", VERSION_STRING);
      field = m_endField;
      fieldHeight = (sizeof(m_endField) / (sizeof(char) *CMaxInvWidth));
      break;
    case EScreenWin:
      sprintf(descr,    "YOU WON");
      sprintf(controls, "Quit enter%sNew Game space", m_sep);
      sprintf(infobar,  "<%s>", AUTHOR_STRING);
      sprintf(infobar2, "v%s", VERSION_STRING);
      field = m_endField;
      fieldHeight = (sizeof(m_endField) / (sizeof(char) *CMaxInvWidth));
      break;
    case EScreenQuit:
      sprintf(descr,    "GOODBYE");
      sprintf(controls, "Quit enter");
      sprintf(infobar,  "<%s>", AUTHOR_STRING);
      sprintf(infobar2, "v%s", VERSION_STRING);
      field = m_endField;
      fieldHeight = (sizeof(m_endField) / (sizeof(char) *CMaxInvWidth));
      break;
    case EScreenStartFadeIn:
      sprintf(descr,    "ROOM EXPLORER");
      sprintf(controls, "Start enter%sQuit q", m_sep);
      sprintf(infobar,  "<%s>", AUTHOR_STRING);
      sprintf(infobar2, "v%s", VERSION_STRING);
      field = m_startFieldFade;
      fieldHeight = (sizeof(m_startField) / (sizeof(char) *CMaxInvWidth));
      break;
    default: return;
  }

  int padlen = m_fieldWidth - strlen(descr);
  if (padlen < 0) padlen = 0;
  int ctrlen = strlen(controls);
  int infolen = strlen(infobar);
  int info2len = strlen(infobar2);

  system("clear");
  printf("%*.*s\n",       m_screenWidth, m_screenWidth, m_padding);
  printf("%s%*.*s%s\n",   m_sep, m_fieldWidth, m_fieldWidth, m_space, m_sep);
  printf("%s%s%*.*s%s\n", m_sep, descr, padlen, padlen, m_space, m_sep);
  printf("%*.*s\n",       m_screenWidth, m_screenWidth, m_padding);
  printf("%s%*.*s%s\n",   m_sep, m_fieldWidth, m_fieldWidth, m_space, m_sep);

  for (int i = 0, j = 0; i < CMaxUiHeight; i++) {

    if (type == EScreenRoom) {

      if ( (((CMaxUiHeight - ROOM_HEIGHT) / 2) > i) ||
           (((CMaxUiHeight - ROOM_HEIGHT) / 2) + ROOM_HEIGHT) < i) {
             printf("%s%*.*s%s%s%*.*s%s\n", m_sep, m_roomWidth, m_roomWidth,
                                            m_space, m_sep,m_logField[i],
                                            (int)(m_logWidth - strlen(m_logField[i])),
                                            (int)(m_logWidth - strlen(m_logField[i])),
                                            m_space, m_sep);
             continue;
      }


      int padlen_pre = (m_roomWidth - ROOM_WIDTH) / 2;
      if (padlen_pre < 0) padlen_pre = 0;

      sprintf(line, "%*.*s%s", padlen_pre, padlen_pre, m_space, field[j]);
      int padlen_post = m_roomWidth - strlen(line);
      if (padlen_post < 0) padlen_post = 0;

      printf("%s%s%*.*s%s%s%*.*s%s\n", m_sep, line, padlen_post, padlen_post,
                                       m_space, m_sep, m_logField[i],
                                       (int)(m_logWidth - strlen(m_logField[i])),
                                       (int)(m_logWidth - strlen(m_logField[i])),
                                       m_space, m_sep);
      j++;

    } else if (type == EScreenFight) {

      int monsterHeight = sizeof(m_monsterField) / sizeof(m_monsterField[0]);

      if ( (((CMaxUiHeight - monsterHeight) / 2) > i) ||
           (((CMaxUiHeight - monsterHeight) / 2) + monsterHeight) <= i) {
             printf("%s%s%*.*s%s%*.*s%s\n", m_sep,
                      field[i], (int)(m_fightWidth - strlen(field[i])),
                      (int)(m_fightWidth - strlen(field[i])), m_space,
                      m_sep, m_monsterWidth, m_monsterWidth, m_space, m_sep);
             continue;
      }

      int8_t padlen_pre = 0;
      snprintf(line, sizeof(line), "%*.*s%s", padlen_pre, padlen_pre, m_space, m_monsterField[j]);
      int padlen_post = m_monsterWidth - strlen(line);
      if (padlen_post < 0) padlen_post = 0;

      printf("%s%s%*.*s%s%s%*.*s%s\n", m_sep,
              field[i], (int)(m_fightWidth - strlen(field[i])),
              (int)(m_fightWidth - strlen(field[i])), m_space, m_sep,
              line, padlen_post, padlen_post, m_space, m_sep);
      j++;

    } else {
      // regular fields (i.e. not room or fight)
      if (i < fieldHeight) {
        padlen = m_fieldWidth - strlen(field[i]);
        if (padlen < 0) padlen = 0;
        printf("%s%s%*.*s%s\n", m_sep, field[i], padlen, padlen, m_space, m_sep);
      } else {
        printf("%s%*.*s%s\n", m_sep, m_fieldWidth, m_fieldWidth, m_space, m_sep);
      }
    }
  }

  printf("%s%*.*s%s\n", m_sep, m_fieldWidth, m_fieldWidth, m_space, m_sep);
  printf("%*.*s\n", m_screenWidth, m_screenWidth, m_padding);
  printf("%s%s%*.*s%s%s\n", m_sep, controls, m_fieldWidth - ctrlen - infolen,
                            m_fieldWidth - ctrlen - infolen, m_space, infobar, m_sep);
  printf("%s%*.*s%s%s\n", m_sep, m_fieldWidth - info2len, m_fieldWidth - info2len,
                          m_space, infobar2, m_sep);
  printf("%*.*s\n", m_screenWidth, m_screenWidth, m_padding);

  fflush(stdout);
}

void MazeUI::createRoomField(Room *room) {
  bool ew = (room->East() && room->West());
  char line[CMaxInvWidth] = "";

  // calculate padding
  int pad = (ROOM_WIDTH - 6) / 2;
  memset(m_roomField, '\0', sizeof(m_roomField));

  for (int i = 0; i <= ROOM_HEIGHT; i++) {
    switch (i) {
      case 0:
        if (room->North())      sprintf(line, "   %*.*s%2d%*.*s", pad, pad, m_space, room->North()->getNo(), pad, pad, m_space);
        else                    sprintf(line, " ");
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      case 1:
        if (room->North())      sprintf(line, "   %*.*s| |%*.*s",    pad, pad, m_padding, pad, pad, m_padding);
        else                    sprintf(line, "   %*.*s%3.3s%*.*s",  pad, pad, m_padding, m_padding, pad, pad, m_padding);
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      case (ROOM_HEIGHT / 2):
        if (ew)                 sprintf(line, "%2d  %*.*s%2d%*.*s %2d", room->West()->getNo(), pad, pad, m_space, room->getNo(), pad, pad, m_space, room->East()->getNo());
        else if (room->West())  sprintf(line, "%2d  %*.*s%2d%*.*s|",    room->West()->getNo(), pad, pad, m_space, room->getNo(), pad, pad, m_space);
        else if (room->East())  sprintf(line, "   |%*.*s%2d%*.*s %2d",  pad, pad, m_space, room->getNo(), pad, pad, m_space, room->East()->getNo());
        else                    sprintf(line, "   |%*.*s%2d%*.*s|",     pad, pad, m_space, room->getNo(), pad, pad, m_space);
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      case (ROOM_HEIGHT / 2) - 1:
      case (ROOM_HEIGHT / 2) + 1:
        if (ew)                 sprintf(line, "   =%*.*s  %*.*s=", pad, pad, m_space, pad, pad, m_space);
        else if (room->West())  sprintf(line, "   =%*.*s  %*.*s|", pad, pad, m_space, pad, pad, m_space);
        else if (room->East())  sprintf(line, "   |%*.*s  %*.*s=", pad, pad, m_space, pad, pad, m_space);
        else                    sprintf(line, "   |%*.*s  %*.*s|", pad, pad, m_space, pad, pad, m_space);
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      case ROOM_HEIGHT - 1:
        if (room->South())      sprintf(line, "   %*.*s| |%*.*s", pad, pad, m_padding, pad, pad, m_padding);
        else                    sprintf(line, "   %*.*s%3.3s%*.*s", pad, pad, m_padding, m_padding, pad, pad, m_padding);
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      case ROOM_HEIGHT:
        if(room->South())       sprintf(line, "    %*.*s%-2d%*.*s", pad, pad, m_space, room->South()->getNo(), pad, pad, m_space);
        else                    sprintf(line, " ");
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
      default:
                                sprintf(line, "   |%*.*s  %*.*s|", pad, pad, m_space, pad, pad, m_space);
        memcpy(m_roomField[i], line, CMaxInvWidth);
        break;
    }
  }
}

void MazeUI::createInvField(unsigned int *startIdx) {
  unsigned int idx = *startIdx;
  char line[CMaxInvWidth];
  memset(m_inventoryField, '\0', sizeof(m_inventoryField));

  int  stats_len = strlen(m_itemStats);
  int i = 0;

  m_log->debug(" - Generating Inventory - ");

  // overview
  sprintf(line, "You feel %s", m_player->getMentalHealthStr());
  int linepad = m_invWidth - stats_len - strlen(line) - strlen(m_sep) -1;
  sprintf(m_inventoryField[i++], "%s.%*.*s%s%s", line,
            linepad, linepad, m_space,
            m_sep, m_itemStats);
  sprintf(m_inventoryField[i++], "%*.*s", m_invWidth, m_invWidth, m_padding);

  if (!m_inventoryLen) {
    m_log->debug("inventory list empty or not generated");
    return;
  }

  // check if index is ok
  if (m_inventoryLen <= CMaxInvHeight) {
    idx = 0;
  } else if ((*startIdx + CMaxInvHeight) > m_inventoryLen) {
    idx = m_inventoryLen - CMaxInvHeight;
    if (idx > *startIdx) idx = 0;
  }

  *startIdx = idx;

  // items
  m_log->debug("Writing item list to field (startIdx: %d)", idx);
  while (i <= CMaxUiHeight) {
    memcpy(m_inventoryField[i++], m_itemList[idx++], CMaxInvWidth);
  }
}

void MazeUI::writeItemList() {

  LiList<Item> *list = m_player->getItems();
  unsigned int i = 0;

  int  stats_len = strlen(m_itemStats);
  unsigned int max_text_len = m_invWidth - stats_len - strlen(m_sep);
  unsigned int max_name_len = max_text_len - 1; // to account for space at line start

  memset(m_itemList, 0, sizeof(m_itemList));

  for (Item *it = list->Head(); it != NULL; it = list->Next()) {

    m_log->debug("Generating entry for %s", it->represent());

    char lines[CMaxSplitLines][CMaxInvWidth] = {'\0'};
    sprintf(lines[0], "%s", it->represent());
    unsigned int len = max_name_len;
    int n = 1;

    if (strlen(lines[0]) > max_name_len -1) {
      n = CMaxSplitLines;
      splitLine(lines[0], lines, len, &n);
    }

    int row = --n;
    while (row) {
      m_log->debug("printing name line %d of length %d", n - row, strlen(lines[n - row]));
      sprintf(m_itemList[i++], " %s%*.*s",
              lines[n - row],
              0, int(m_invWidth - strlen(lines[n - row]) - 1), m_space);
      row--;
    }

    // last line with stats
    m_log->debug("printing last line with stats: line %d", n);
    sprintf(m_itemList[i++], " %s%*.*s%s%3d%s%4d%s%3d%*.*s",
              lines[n],
              0, int(max_name_len - strlen(lines[n])), m_space,
              m_sep, it->Damage(), m_sep, it->Mood(), m_sep, it->Health(),
              0, int(stats_len - (2 * strlen(m_sep)) - 10), m_space);
  }
  m_log->debug("Generated inventory of length %d", i);

  for (int j = 0; j <= (int)i; j++) {
    m_log->debug("%s", m_itemList[j]);
  }

  m_inventoryLen = i;
}

void MazeUI::screenFadeIn() {

  int totalLineLen = sizeof(m_startField[0]);
  int totalLines   = sizeof(m_startField) / totalLineLen;

  memset(m_startFieldFade, '\0', sizeof(m_startFieldFade));

  for (int i = 0; i < totalLineLen; i++) {
    for (int j = 0; j < totalLines; j++) {
      memcpy(m_startFieldFade[j], m_startField[j], i);
      m_log->debug("adding line: %s", m_startFieldFade[j]);
    }
    showScreen(EScreenStartFadeIn);
    for (int k = 0; k < 300000; k++) {}
  }

  m_log->debug("done with fade in");
}

int MazeUI::getCurrentScreenWidth() {
  // get current screen size for correct width of inventory screen
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

void MazeUI::addLogLine(const char* line) {

  m_log->debug("Adding logLine w/ len %d: %s", strlen(line), line);

  if (strlen(line) > m_logWidth - 1) {
    char lines[CMaxSplitLines][CMaxInvWidth] = {'\0'};
    unsigned int len = m_logWidth - 1;
    int n = CMaxSplitLines;

    splitLine(line, lines, len, &n);
    for (int i = 0; i < n; i++) {
      addLogLine(lines[i]);
    }
    return;
  }

  if (m_logLines >= CMaxUiHeight) {
    m_logLines = CMaxUiHeight - 1;
    // move all lines one up
    for (int i = 1; i < CMaxUiHeight; i++) {
      memmove(m_logField[i - 1], m_logField[i], CMaxLogWidth);
    }
  }

  memcpy(m_logField[m_logLines++], line, CMaxLogWidth);
}

void MazeUI::removeLogLine() {
  memset(m_logField[m_logLines-- - 1], '\0', CMaxLogWidth);
}

void MazeUI::addFightLine(const char* line) {

  m_log->debug("Adding fightLine w/ len %d: %s", strlen(line), line);

  if (strlen(line) > m_fightWidth -1) {
    char lines[CMaxSplitLines][CMaxInvWidth] = {'\0'};
    unsigned int len = m_fightWidth - 1;
    int n = CMaxSplitLines;

    splitLine(line, lines, len, &n);
    for (int i = 0; i < n; i++) {
      addFightLine(lines[i]);
    }
    return;
  }

  if (m_fightLines >= CMaxUiHeight) {
    m_fightLines = CMaxUiHeight - 1;
    // move all lines one up
    for (int i = 1; i < CMaxUiHeight; i++) {
      memmove(m_fightField[i - 1], m_fightField[i], CMaxInvWidth);
    }
  }

  memcpy(m_fightField[m_fightLines++], line, CMaxInvWidth);
}

void MazeUI::splitLine(const char *line, char lines[][CMaxInvWidth], unsigned int len, int *n) {

  m_log->debug("preparing processing line of len %d: %s", strlen(line), line);
  char outlines[CMaxSplitLines][CMaxBufLen] = {'\0'};

  sprintf(outlines[0], "%s", line);
  int idx = 0;
  char *t;

  m_log->debug("processing line of len %d: %s", strlen(outlines[0]), outlines[0]);

  while (strlen(outlines[idx]) >= len) {
    m_log->debug("line %d exceeds max width: %d of %d: %s",
                  idx, strlen(outlines[idx]), len, outlines[idx]);
    t = &outlines[idx][len];
    while (*t != ' ') {
      t--;
    }

    // t should now point to last white space before max name len
    for (unsigned int i = len; i >= 0; i--) {
      if (t == &outlines[idx][i]) {
        m_log->debug("found cut point at char %d", i);
        outlines[idx][i] = '\0';
        strncat(outlines[idx + 1], &outlines[idx][i + 1], CMaxInvWidth);
        idx++;
        break;
      }
    }
  }

  // copy to lines and set n
  for (int i = 0; i <= idx; i++) {
    memcpy(lines[i], outlines[i], len);
  }
  *n = ++idx;

}
