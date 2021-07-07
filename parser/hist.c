#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

// ALL CHECKS OMMITTED!

int main()
{
  char *s;
  char *p = getenv("USER");
  char *host = getenv("HOSTNAME");
  int count = 1;
  char *ps;

  printf("USER: %s\n", p);
  // HOSTNAME does not return anything here, so I skipped it
  printf("HOST: %s\n", host);

  // restricting to 32/64 bit ints for simplicity
  size_t size_int = (sizeof(int) == 4) ? 10 : 20;
  //         env(USER)    env(HOSTN...)   log_10(MAX_INT)  " @:\0"  angst-allowance
  //ps = malloc(strlen(p) + strlen(host) +      size_int       + 4          + 1);
  //sprintf(ps, "%d %s@%s:", count, p, host);
  ps = malloc(strlen(p) + size_int + 4 + 1);
  sprintf(ps, "%d %s:", count, p);

  while ((s = readline(ps)) != NULL) {
    add_history(s);
    free(s);
    count++;
    sprintf(ps, "%d %s:", count, p);
  }

  printf("\nlastline:\n");
  // That's the line you were looking for, I think.
  // int where_history(void) returns the current (that is: last) line
  // HIST_ENTRY *history_get (int) returns the entry data set
  HIST_ENTRY *entry = history_get(where_history());

  printf("%s\n", entry->line);
  free(ps);

  exit(EXIT_SUCCESS);
}