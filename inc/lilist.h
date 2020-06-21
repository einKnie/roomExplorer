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
#ifndef _LILIST_H_
#define _LILIST_H_

#include <stdlib.h>

template <class T> class LiList {
public:

  LiList();
  LiList(const LiList &other);
  ~LiList();

  T *Head();
  T *Tail();
  T *Next();
  // T *Prev();
  int add(T *t);
  int remove(T *t);
  T *find(T *t);
  T *get_random();
  unsigned int Size();
  void free();


private:

  typedef struct list {
    T *obj;

    list *next;
    list *prev;

    list() : list(NULL) {}
    list(T *o) {
      obj = o;

      next = NULL;
      prev = NULL;
    }

    ~list() {
      next = NULL;
      prev = NULL;
    }
  } list_t;

  list_t *m_head;
  list_t *m_tail;
  list_t *m_curr;
  unsigned int m_len;

  list_t* find_internal(T *t) {
    list_t *fnd = NULL;
    list_t *tmp = m_head;

    while (tmp != NULL) {
      if (*tmp->obj == *t) {
        fnd = tmp;
        break;
      }
      tmp = tmp->next;
    }
    return fnd;
  }

};

template <class T> LiList<T>::LiList() {
  m_head = NULL;
  m_tail = m_head;
  m_curr = m_head;
  m_len = 0;
}

template <class T> LiList<T>::LiList(const LiList &other) {
  m_head = NULL;
  m_tail = m_head;
  m_curr = m_head;
  m_len = 0;

  list_t *tmp = other.m_head;
  while (tmp != NULL) {
    add(tmp->obj);
    tmp = tmp->next;
  }
}

template <class T> LiList<T>::~LiList() {
  if (m_head != NULL) {
    list_t *tmp = m_tail;
    do {
      tmp = tmp->prev;
      delete m_tail;
      m_tail = tmp;
    } while (tmp != NULL);
  }
}

template <class T> int LiList<T>::add(T *t) {
  if (t == NULL) {
    return 0;
  }

  list_t *tmp = new list_t(t);
  if (!m_head) {
    m_head = tmp;
    m_tail = tmp;
  } else {
    m_tail->next = tmp;
    tmp->prev = m_tail;
    m_tail = tmp;
  }
  m_len++;
  return 1;
}

template <class T> int LiList<T>::remove(T *t) {
  list_t *rem = NULL;
  if ((rem = find_internal(t)) == NULL) {
    return 0;
  }

  if (rem->next) {
    rem->next->prev = rem->prev;
  } else {
    m_tail = rem->prev;
  }

  if (rem->prev) {
    rem->prev->next = rem->next;
    if (m_curr == rem) m_curr = rem->prev;
  } else {
    m_head = rem->next;
    if (m_curr == rem) m_curr = m_head;
  }

  delete rem;
  m_len --;
  return 1;
}

template <class T> T* LiList<T>::find(T *t) {
  list_t *fnd = NULL;
  list_t *tmp = m_head;

  while (tmp != NULL) {
    if (*tmp->obj == *t) {
      fnd = tmp;
      break;
    }
    tmp = tmp->next;
  }
  return fnd ? fnd->obj : NULL;
}

template <class T> T* LiList<T>::get_random() {
  if (m_len > 0) {
    int idx = rand() % m_len;
    list_t *tmp = m_head;

    while (tmp != NULL && idx) {
      tmp = tmp->next;
      idx--;
    }
    return tmp->obj;
  } else {
    return NULL;
  }
}

template <class T> T* LiList<T>::Head() {
  m_curr = m_head;
  return m_head ? m_head->obj : NULL;
}

template <class T> T* LiList<T>::Tail() {
  m_curr = m_tail;
  return m_tail->obj;
}

template <class T> T* LiList<T>::Next() {
  m_curr = m_curr ? m_curr->next : m_curr;
  return m_curr ? m_curr->obj : NULL;
}

template <class T> void LiList<T>::free() {
  if (m_head != NULL) {
    list_t *tmp = m_tail;
    do {
      tmp = tmp->prev;
      delete m_tail->obj;
      delete m_tail;
      m_tail = tmp;
    } while (tmp != NULL);
  }
  m_head = NULL;
}

template <class T> unsigned int LiList<T>::Size() {
  return m_len;
}

#endif // _LILIST_H_
