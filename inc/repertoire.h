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
 *              |_| v0.2
 *****************************************************************************/
#ifndef _REPERTOIRE_H_
#define _REPERTOIRE_H_

#include "attack.h"
#include "lilist.h"

class Repertoire {
private:

  Logger          *m_log;       ///< logger
  LiList<Attack>  *m_attacks;   ///< linked list of attacks

public:
  Repertoire(Logger *log);
  Repertoire(const Repertoire& other);
  ~Repertoire();

  /// @brief Add an attack to the repertoire
  /// @pram[in] atk the attack to add
  /// @return 1 on success, 0 on failure
  int addAtk(Attack *atk);

  /// @brief Remove an attack from the repertoire
  /// @pram[in] atk the attack to remove
  /// @return 1 on success, 0 on failure
  int removeAtk(Attack *atk);

  /// @brief Use an attack
  /// @pram[in] atk the attack to use, or NULL for a random attack
  /// @return the raw damage generated from the attack
  int useAtk(Attack *atk = NULL);

  /// @brief Check if an attack is in the repertoire
  /// @param[in] atk the attack to check
  /// @return true if the given attack is in the repertoire, else false
  bool checkAttack(Attack *atk);

  /// @brief Return a random attack from the repertoire
  /// @return a random attack, or ULL if no attacks in the repertoire
  Attack* getRandomAttack();

  /// @brief Return the number of attacks in the repertoire
  /// @return number of available attacks
  int getAttacksLen();

  /// @brief Return the linked list of attacks
  /// @return linked list of attacks
  LiList<Attack>* getAttacks();

  /// @brief Delete all attacks
  /// @note: must be called in order to free attacks
  void deleteAttacks();

};


#endif //!_REPERTOIRE_H_
