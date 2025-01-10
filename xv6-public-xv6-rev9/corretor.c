#include "types.h"
#include "user.h"
#include "date.h"

#define TRUE  1
#define FALSE 0
#define N     10

int stdout = 1;
int stderr = 2;

const int GLOBAL1_RO = 0xdcc605;
int GLOBAL2_RW = 0xdcc606;


// testa o fork normal. garante que não quebramos nada
// N forks e waits consecutivos. existe um limite de 64
// processos no xv6, então temos que garantir que N <= 64
int caso2(void) {

  int curr_pages;
  int last_pages;

  // Número inicial de páginas
  last_pages = num_pages();

  // Alocar mais memória
  sbrk(4096);
  curr_pages = num_pages();
  if (curr_pages != last_pages + 1) {
    return FALSE;
  }
  last_pages = curr_pages;

  sbrk(8192);
  curr_pages = num_pages();
  if (curr_pages != last_pages + 2) {
    return FALSE;
  }

  return TRUE;
}

int caso1(void) {
  printf(2, "################## INICIO PRINTS DO TESTE DE VIRT2REAL ##################\n");
  for (int i = 1; i <= 12; i++) {
    char *va = sbrk(4096);  // Allocate 4 KB
    if (va == (char *)-1) {
      printf(2, "Erro: sbrk falhou.\n");
      return FALSE;
    }

    // Translate virtual to physical
    int pa = virt2real(va);

    if (pa == -1) {
      printf(stdout, "Erro pois argptr(0, (void*)&va, sizeof(char*)) < 0 eh verdadeiro.\n");
      return FALSE;
    } else if (pa == -2) {
      printf(stdout, "Erro pois !pte || !(*pte & PTE_P) eh verdadeiro.\n");
      return FALSE;
    }
    printf(stdout, "Endereço virtual: %p, Endereço físico: %p\n", va, pa);
    printf(stdout, "################## FIM PRINTS DO TESTE DE VIRT2REAL ##################\n");
  }
  return TRUE;
}

// testa a chamada date
int get_date(struct rtcdate *r) {
  if (date(r)) {
    printf(stderr, "[ERROR] Erro na chamada de sistema date\n");
    return FALSE;
  }
  return TRUE;
}


void print_date(struct rtcdate *r) {
  printf(stdout, "%d/%d/%d %d:%d:%d\n", r->day,
                                        r->month,
                                        r->year,
                                        r->hour,
                                        r->minute,
                                        r->second);
}


int main(int argc, char *argv[]) {
  struct rtcdate r;
  int call_ok = 1;

  printf(stdout, "[Caso 0] Testando o date\n");
  call_ok = get_date(&r);
  if (call_ok == FALSE) {
    printf(stdout, "[Caso 0 - ERROR] Falhou!\n");
    exit();
  }
  print_date(&r);
  printf(stdout, "[Caso 0] OK\n");

  get_date(&r);
  print_date(&r);
  printf(stdout, "[Caso 1] Testando o virt2real\n");
  call_ok = caso1();
  if (call_ok == FALSE) {
    printf(stdout, "[Caso 1 - ERROR] Falhou!\n");
    exit();
  }
  printf(stdout, "[Caso 1] OK\n");

  get_date(&r);
  print_date(&r);
  printf(stdout, "[Caso 2] Testando o num_pages\n");
  call_ok = caso2();
  if (call_ok == FALSE) {
    printf(stdout, "[Caso 2 - ERROR] Falhou!\n");
    exit();
  }
  printf(stdout, "[Caso 2] OK\n");

  printf(stdout, "@@@@@@@@@@@@@@@@@@ Todos os casos passaram!!! @@@@@@@@@@@@@@@@@@\n");
  
  exit();
}