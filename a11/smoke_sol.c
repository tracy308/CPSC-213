#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000 //(change back after this is working)

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

struct Agent*  a;
struct Listener* listener;
int keepGoing;

struct Listener {
  struct Agent* agent;
  uthread_cond_t  match_and_paper;
  uthread_cond_t  paper_and_tobacco;
  uthread_cond_t  tobacco_and_match;
  uthread_cond_t  ready_to_smoke;
  int tobacco_is_ready;
  int paper_is_ready;
  int match_is_ready;
};

struct Listener* createListener(struct Agent* agent) {
  struct Listener* listener = malloc (sizeof (struct Listener));
  listener->agent = agent;
  listener->match_and_paper   = uthread_cond_create (agent->mutex);
  listener->paper_and_tobacco   = uthread_cond_create (agent->mutex);
  listener->tobacco_and_match = uthread_cond_create (agent->mutex);
  listener->ready_to_smoke   = uthread_cond_create (agent->mutex);
  listener->tobacco_is_ready = 0;
  listener->paper_is_ready = 0;
  listener->match_is_ready = 0;
  return listener;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

void* smoker_director(uthread_cond_t wait_for_resource, int* resource_is_ready) {
  uthread_mutex_lock(listener->agent->mutex);
  while (keepGoing) {
    uthread_cond_wait(wait_for_resource);
    *resource_is_ready = 1;
    if (listener->tobacco_is_ready == 1 && listener->paper_is_ready == 1) {
      uthread_cond_signal(listener->paper_and_tobacco);
    } else if (listener->match_is_ready == 1 && listener->tobacco_is_ready == 1) {
      uthread_cond_signal(listener->tobacco_and_match);
    } else if (listener->match_is_ready == 1 && listener->paper_is_ready == 1) {
      uthread_cond_signal(listener->match_and_paper);
    }
  }
  uthread_mutex_unlock(listener->agent->mutex);
  return NULL;
}

void* match_watcher() {
  smoker_director(a->match, &listener->match_is_ready);
  return NULL;
}

void* paper_watcher() {
  smoker_director(a->paper, &listener->paper_is_ready);
  return NULL;
}

void* tobacco_watcher() {
  smoker_director(a->tobacco, &listener->tobacco_is_ready);
  return NULL;
}

void* smoker_match() {
  uthread_mutex_lock(listener->agent->mutex);
  while (keepGoing) {
    uthread_cond_wait(listener->paper_and_tobacco);
    smoke_count[MATCH]++;
    listener->paper_is_ready = 0;
    listener->tobacco_is_ready = 0;
    uthread_cond_signal(listener->agent->smoke);
  }
  uthread_mutex_unlock(listener->agent->mutex);
  return NULL;
}

void* smoker_paper() {
  uthread_mutex_lock(listener->agent->mutex);
  while (keepGoing) {
    uthread_cond_wait(listener->tobacco_and_match);
    smoke_count[PAPER]++;
    listener->match_is_ready = 0;
    listener->tobacco_is_ready = 0;
    uthread_cond_signal(listener->agent->smoke);
  }
  uthread_mutex_unlock(listener->agent->mutex);
  return NULL;
}

void* smoker_tobacco() {
  uthread_mutex_lock(listener->agent->mutex);
  while (keepGoing) {
    uthread_cond_wait(listener->match_and_paper);
    smoke_count[TOBACCO]++;
    listener->match_is_ready = 0;
    listener->paper_is_ready = 0;
    uthread_cond_signal(listener->agent->smoke);
  }
  uthread_mutex_unlock(listener->agent->mutex);
  return NULL;
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  a = createAgent();
  // TODO
  listener = createListener(a);
  keepGoing = 1;
  uthread_create(match_watcher, NULL);
  uthread_create(paper_watcher, NULL);
  uthread_create(tobacco_watcher, NULL);
  uthread_create(smoker_match, NULL);
  uthread_create(smoker_paper, NULL);
  uthread_create(smoker_tobacco, NULL);

  uthread_join (uthread_create (agent, a), 0);
  free(a);
  free(listener);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}