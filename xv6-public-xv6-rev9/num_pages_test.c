#include "types.h"
#include "user.h"

int
main(void)
{
  int pages;

  // Número inicial de páginas
  pages = num_pages();
  printf(1, "Páginas usadas inicialmente: %d\n", pages);

  // Alocar mais memória
  sbrk(4096);  // Alocar 1 página
  pages = num_pages();
  printf(1, "Páginas usadas após alocar 1 página: %d\n", pages);

  sbrk(8192);  // Alocar mais 2 páginas
  pages = num_pages();
  printf(1, "Páginas usadas após alocar mais 2 páginas: %d\n", pages);

  exit();
}
