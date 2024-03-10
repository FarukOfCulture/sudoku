#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Multiple board on the same or seperate files
// TODO: Make gui for all this

char getcol(char x) { return x % 9; }

char getrow(char x) { return x / 9; }

char getnextavail(signed char x, char *board) {
  bool arr[9] = {false};
  for (signed char i = getcol(x); i < 81; i += 9) {
    if (board[i] > 0) {
      arr[board[i] - 1] = true;
    }
  }
  char row = getrow(x) * 9;
  for (signed char i = 0; i < 9; i++) {
    if (board[i + row] > 0) {
      arr[board[i + row] - 1] = true;
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      char a = board[(getrow(x) / 3 * 3 + i) * 9 + j + getcol(x) / 3 * 3];
      if (a > 0) {
        arr[a - 1] = true;
      }
    }
  }
  for (int i = 0; i < 9; i++) {
    if (!arr[i] && i >= board[x]) {
      return i + 1;
    }
  }
  return -1;
}

void board_from_string(char *str, char *board) {
  for (signed char i = 0; i < 81; i++) {
    char n = str[i];
    assert(n != 0);
    if (n == ' ') {
      n = '0';
    }
    assert('0' <= n && n <= '9');
    board[i] = n - '0';
  }
}

void board_from_file(char *file_name, char *board) {
  FILE *f = fopen(file_name, "r");
  assert(f);
  char n = getc(f);
  for (unsigned char i = 0; i < 81; i++) {
    assert(n != EOF);
    if (n == '\r' || n == '\n') {
      i--;
      n = getc(f);
      continue;
    }
    if (n == ' ')
      n = '0';
    assert('0' <= n && n <= '9');
    board[i] = n - '0';

    n = getc(f);
  }
}

bool check_board(char *board, char *oboard) {
  for (signed char i = 0; i < 81; i++) {
    if (oboard[i] != 0 && oboard[i] != board[i])
      return false;
  }

  for (signed char x = 0; x < 9; x++) {
    bool arr[9] = {false};
    for (signed char i = getcol(x); i < 81; i += 9) {
      if (board[i] > 0) {
        arr[board[i] - 1] = true;
      }
    }
    for (signed char i = 0; i < 9; i++) {
      if (arr[i] == false)
        return false;
    }
  }
  for (signed char x = 0; x < 9; x++) {
    bool arr[9] = {false};
    char row = getrow(x) * 9;
    for (signed char i = 0; i < 9; i++) {
      if (board[i + row] > 0) {
        arr[board[i + row] - 1] = true;
      }
    }
    for (signed char i = 0; i < 9; i++) {
      if (arr[i] == false)
        return false;
    }
  }

  for (signed char x = 0; x < 9; x++) {
    bool arr[9] = {false};
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        char a = board[(getrow(x) / 3 * 3 + i) * 9 + j + getcol(x) / 3 * 3];
        if (a > 0) {
          arr[a - 1] = true;
        }
      }
    }
    for (signed char i = 0; i < 9; i++) {
      if (arr[i] == false)
        return false;
    }
  }
  return true;
}

// TODO: Implement usage()
void usage(void) { assert(0 && "`usage()` is unimplemented!!"); }

int main(int argc, char **argv) {
  char *file_name = NULL;

  argc--;
  argv++;
  while (argc > 0) {
    if (**argv == '-') {
      // TODO: Implement flags if needed
    } else {
      file_name = *argv;
    }
    argc--;
    argv++;
  }

  if (file_name == NULL) {
    usage();
    exit(1);
  }

  char *oboard = malloc(sizeof(char[81]));
  assert(oboard);
  board_from_file(file_name, oboard);

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      printf("%i", oboard[i * 9 + j]);
    }
    printf("\n");
  }
  printf("\n");

  char *board = malloc(sizeof(char[81]));
  assert(board);
  memcpy(board, oboard, sizeof(char[81]));
  for (signed char i = 0; i < 81; i++) {
    if (oboard[i] == 0) {
      board[i] = getnextavail(i, board);
      if (board[i] < 0) {
        bool canpass = false;
        while (!canpass) {
          i--;
          assert(i >= 0);
          if (oboard[i] == 0) {
            board[i] = getnextavail(i, board);
            if (board[i] > 0) {
              canpass = true;
            }
          }
        }
      }
    }
  }
  assert(check_board(board, oboard));

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      printf("%i", board[i * 9 + j]);
    }
    printf("\n");
  }
  return 0;
}
