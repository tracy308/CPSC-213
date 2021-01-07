#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int           entryTicker;
int           waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int           waitingHistogramOverflow;
uthread_sem_t waitingHistogramMutex;
int           occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness otherEndianness [] = {BIG, LITTLE};

struct Well {
  uthread_sem_t mutex;
  uthread_sem_t done;
  uthread_sem_t littleBigCond [2];
  int           entered;
  enum Endianness      endianness;
  int           endianWaiting [2];
};

struct Well* createWell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->mutex                 = uthread_sem_create (1);
  well->done                  = uthread_sem_create (0);
  well->littleBigCond [LITTLE]       = uthread_sem_create (0);
  well->littleBigCond [BIG]     = uthread_sem_create (0);
  well->entered         = 0;
  well->endianness           = 0;
  well->endianWaiting [LITTLE]   = 0;
  well->endianWaiting [BIG] = 0;
  return well;
}

void recordEntry (struct Well* well, enum Endianness Endianness) {
  assert (well->entered == 0 || well->endianness == Endianness);
  assert (well->entered < MAX_OCCUPANCY);
  well->entered  ++;
  well->endianness = Endianness;
  entryTicker++;
  occupancyHistogram [well->endianness] [well->entered] ++;
}

void enterWell (struct Well* well, enum Endianness Endianness) {
  uthread_sem_wait (well->mutex);
    int isEmpty         = well->entered                  == 0;
    int hasRoom         = well->entered                  <  MAX_OCCUPANCY;
    int sameEndianness         = well->endianness                    == Endianness;
    int otherEndiannessWaiting = well->endianWaiting [otherEndianness [Endianness]]  >  0;
    int canEnter        = (isEmpty || (hasRoom && sameEndianness && ! otherEndiannessWaiting));
    if (canEnter)
      recordEntry (well, Endianness);
    else
      well->endianWaiting [Endianness] ++;
  uthread_sem_signal (well->mutex);
  if (! canEnter)
    uthread_sem_wait (well->littleBigCond [Endianness]);
}

void leaveWell (struct Well* well) {
  uthread_sem_wait (well->mutex);
    well->entered -= 1;
    enum Endianness inEndianness  = well->endianness;
    enum Endianness outEndianness = otherEndianness [inEndianness];
    if (well->endianWaiting [outEndianness] > 0) {
      if (well->entered == 0) {
        for (int i = 0; i < well->endianWaiting [outEndianness] && i < MAX_OCCUPANCY; i++) {
          well->endianWaiting [outEndianness] --;
          recordEntry (well, outEndianness);
          uthread_sem_signal (well->littleBigCond [outEndianness]);
        }
      }
    } else if (well->endianWaiting [inEndianness]) {
      well->endianWaiting [inEndianness] --;
      recordEntry (well, inEndianness);
      uthread_sem_signal (well->littleBigCond [inEndianness]);
    }
  uthread_sem_signal (well->mutex);
}

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramMutex);
    if (waitingTime < WAITING_HISTOGRAM_SIZE)
      waitingHistogram [waitingTime] ++;
    else
      waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramMutex);
}

void* person (void* wellv) {
  struct Well* well = wellv;
  enum Endianness      Endianness   = random() & 1;
  
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int startTime = entryTicker;
    enterWell (well, Endianness);
    recordWaitingTime (entryTicker - startTime - 1);
    for (int i = 0; i < NUM_PEOPLE; i++) uthread_yield();
    leaveWell (well);
    for (int i = 0; i < NUM_PEOPLE; i++) uthread_yield();
  }
  uthread_sem_signal (well->done);
  return NULL;
}

void mysrandomdev() {
  unsigned long seed;
  int f = open ("/dev/random", O_RDONLY);
  read    (f, &seed, sizeof (seed));
  close   (f);
  srandom (seed);
}

int main (int argc, char** argv) {
  uthread_init (1);
  mysrandomdev();
  struct Well* well = createWell();
  waitingHistogramMutex     = uthread_sem_create (1);
  
  for (int i = 0; i < NUM_PEOPLE; i++)
    uthread_detach (uthread_create (person, well));
  
  for (int i = 0; i < NUM_PEOPLE; i++)
    uthread_sem_wait (well->done);
  
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