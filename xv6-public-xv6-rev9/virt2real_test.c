#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  // Allocate a page
  char *va = sbrk(4096);  // Allocate 4 KB
  if (va == (char *)-1) {
    printf(2, "Erro: sbrk falhou.\n");
    exit();
  }

  // Translate virtual to physical
  int pa = virt2real(va);

  if (pa == -1) {
    printf(2, "Erro pois argptr(0, (void*)&va, sizeof(char*)) < 0 eh verdadeiro.\n");
  } else if (pa == -2) {
    printf(2, "Erro pois !pte || !(*pte & PTE_P) eh verdadeiro.\n");
  } else {
    printf(1, "Endereço virtual: %p, Endereço físico: %p\n", va, pa);
  }

  exit();
}
