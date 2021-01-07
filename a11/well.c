#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  uthread_mutex_t mutex;
  uthread_cond_t littleBigCond[2];
  int entered;
  int endianEnter[2];
  int endianWaiting[2];
  int otherEndWaiting;
  enum Endianness endianness;
};

struct Well* createWell() {
  struct Well* Well = malloc (sizeof (struct Well));

  Well->mutex = uthread_mutex_create();
  Well->littleBigCond[LITTLE] = uthread_cond_create(Well->mutex);
  Well->littleBigCond[BIG] = uthread_cond_create(Well->mutex);
  Well->entered = 0;
  Well->endianEnter[LITTLE] = 0;
  Well->endianEnter[BIG] = 0;
  Well->endianWaiting[LITTLE] = 0;
  Well->endianWaiting[BIG] = 0;
  Well->otherEndWaiting = 0;
  Well->endianness = 0;
  return Well;
}

struct Well* Well;
#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                      
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void enterWell (enum Endianness g) {
  uthread_mutex_lock(Well->mutex);
  while(1) {
    if (Well->entered == 0 || (Well->entered < MAX_OCCUPANCY && Well->endianness == g 
    && !(Well->endianWaiting[oppositeEnd[g]] > 0 && Well->otherEndWaiting > FAIR_WAITING_COUNT))) {
      if (Well->endianness == g) {
        Well->otherEndWaiting++;
      } else {
        Well->otherEndWaiting = 0;
      }
      entryTicker++;
      break;
    }
    if (Well->endianWaiting[g] == 0 && !(Well->endianness == g)) {
        Well->otherEndWaiting = 0;
    }
    Well->endianWaiting[g]++;
    uthread_cond_wait(Well->littleBigCond[g]);
    Well->endianWaiting[g]--;
  }
  Well->entered++;
  Well->endianness = g;
  occupancyHistogram[g][Well->entered]++;
  uthread_mutex_unlock(Well->mutex);
}

void leaveWell() {
  uthread_mutex_lock(Well->mutex);
  enum Endianness endi = Well->endianness;
  Well->entered--;

  if ((!(Well->endianWaiting[endi] > 0) || Well->otherEndWaiting > FAIR_WAITING_COUNT) 
  && Well->endianWaiting[oppositeEnd[endi]] > 0) {
    if(Well->entered == 0) {
      uthread_cond_broadcast(Well->littleBigCond[oppositeEnd[endi]]);
    }
  } else if (Well->endianWaiting[endi] > 0) {
    uthread_cond_signal(Well->littleBigCond[endi]);
  }
  uthread_mutex_unlock(Well->mutex);
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock(waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram[waitingTime]++;
  else
    waitingHistogramOverflow++;
  uthread_mutex_unlock(waitingHistogrammutex);
}

//
// TODO
// You will probably need to create some additional produres etc.
//

void* gateKeeper() {
  enum Endianness endianness = random() % 2;
  int startingValue;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    startingValue = entryTicker;
    enterWell(endianness);
    recordWaitingTime(entryTicker - startingValue - 1);
    for (int j = 0; j < NUM_PEOPLE; j++) {
      uthread_yield();
    }
    leaveWell();
    for (int k =  0; k < NUM_PEOPLE; k++) {
      uthread_yield();
    }
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  Well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO
  for (int i = 0; i < NUM_PEOPLE; i++) {
    pt[i] = uthread_create(gateKeeper, 0);
  }
  for (int j = 0; j < NUM_PEOPLE; j++) {
    uthread_join(pt[j], 0);
  }

  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
