#include "types.h"
#include "user.h"
#include "date.h"

int stdout = 1;
int stderr = 2;

int
main(int argc, char *argv[])
{
  struct rtcdate r;

  if (date(&r)) {
    printf(stderr, "Erro na chamada de sistema\n");
    exit();
  }

  printf(1, "Current DateTime: %d-%d-%d %d:%d:%d\n",
         r.year, r.month, r.day,
         r.hour, r.minute, r.second);

  exit();
}