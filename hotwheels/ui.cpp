#include <stdio.h>
#include <vector>

#include "ansi.hpp"
#include "ui.hpp"

char dashes[81] = "--------------------------------------------------------------------------------";
char spaces[81] = "                                                                                ";

const char* hotwheelschess[] {
  "* * * **** ** ********  ***    *** **********",
  "**** * * * ****** ** *  **    *  ***** ** ** ",
  "* ** * * **** **  *  *   **   *  * **   ** **",
  "* * *  * **** *************    *** **********"
};

namespace ui {

  GameState prevState;
  GameState curState;
  std::vector<Move> historyVector;

  static void updateChar(int r, int c, int hlRow, int hlCol, const GameState* state) {
    U64 bit = 1ULL << (r * 7 + c);
    int basebg = (1&(r^c)) ? 252 : 230;
    if (r == hlRow && (c-hlCol)*(c-hlCol) <= 0) {
      basebg = 109;
    }
    textbg_256(basebg);
    if (state->pieces & bit) {
      if (state->teams & (1ULL<<(r*7+c))) {
        textfg_256(88);
      }
      else {
        textfg_256(18);
      }
      if (state->teams & bit) {
        printf("_");
      }
      else {
        printf(".");
      }
      textbg_256((c == hlCol && (r-hlRow)*(r-hlRow) <= 1) ? 109 : basebg);
      if (state->cars & bit) {
        printf("C");
      }
      else if (state->knights & bit) {
        printf("N");
      }
      else if (state->bishops & bit) {
        printf("B");
      }
      else if (state->rooks & bit) {
        printf("R");
      }
      else if (state->pawns & bit) {
        printf("P");
      }
      textbg_256(basebg);
      if (state->teams & bit) {
        printf("_");
      }
      else {
        printf(".");
      }
    }
    else if (c == hlCol){
      printf(" ");
      textbg_256((c == hlCol && (r-hlRow)*(r-hlRow) <= 1) ? 109 : basebg);
      printf(" ");
      textbg_256(basebg);
      printf(" ");
    }
    else {
      printf("   ");
    }
  }

  static void printBoardRow(int r) {
    int fromRow = -1, fromCol = -1, toRow = -1, toCol = -1;
    if (historyVector.size() > 0) {
      Move lastMove = historyVector.at(historyVector.size() - 1);
      fromRow = lastMove.FromRow();
      fromCol = lastMove.FromCol();
      toRow = lastMove.ToRow();
      toCol = lastMove.ToCol();
    }
    printf(" ");
    for (int c = 0; c < 7; ++c) {
      textattr(RESET);
      textattr(BRIGHT);
      updateChar(r, c, fromRow, fromCol, &prevState);
      textattr(RESET);
    }
    printf(" %d ", r + 1);
    for (int c = 0; c < 7; ++c) {
      textattr(RESET);
      textattr(BRIGHT);
      updateChar(r, c, toRow, toCol, &curState);
      textattr(RESET);
    }
    printf(" ");
  }

#ifdef _WIN32
#define boxchar "#"
#else
#define boxchar "\u2588"
#endif
  void displayAll() {
    printf("\x1B[1;1H\x1B[2J\x1B[1;1H%s\n%s\n", dashes, spaces);
#ifndef NO_ANSI
    printf("%s", adversareval_googly_txt);
#else
    printHeader(adversarevalBlocks, 3*12, BLACK + 30, WHITE + 30, boxchar, 80);
#endif
    printf("\n");
    printHeader(hotwheelschess, 3 * 15, RED + 30, YELLOW + 30, boxchar, 80);
    printf("%s\n", spaces);
    printf("%47.47s+%32.32s\n", dashes, dashes);
    unsigned int historyBase = historyVector.size() > 33 ? historyVector.size() - 33 : 0;
    for (int row = 0; row < 13; ++row) {
      switch (row) {
        case 0:
          printf(" Prev                 ");
          textfg(RED);
          printf("_C_");
          textattr(RESET);
          printf("                  Now ");
          break;

        case 1:
        case 11:
          printf("                                               ");
          break;

        case 10:
          printf("  A  B  C  D  E  F  G     A  B  C  D  E  F  G  ");
          break;

        case 12:
          textfg(BLUE);
          printf("                      .H.                      ");
          textattr(RESET);
          break;

        default:
          printBoardRow(9 - row);
          break;
      }
      printf("| ");
      switch (row) {
        case 0:
          printf("History:\n");
          break;

        case 1:
          printf(historyBase > 0 ? "...\n" : "\n");
          break;

        default:
          for (int i = 0; i < 3; ++i) {
            if ((row-2) * 3 + i + historyBase < historyVector.size()) {
              historyVector.at((row-2) * 3 + i + historyBase).Print();
              printf("/");
              historyVector.at((row-2) * 3 + i + historyBase).Invert().Print();
              printf(" ");
            }
          }
          printf("\n");
          break;
      }
    }
    printf("%47.47s+%32.32s\n", dashes, dashes);
  }
}

#ifdef _WIN32
unsigned char adversareval_googly_txt[] = {
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x20, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0x20, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x23, 0x23,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23,
  0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31,
  0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x20, 0x23,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23,
  0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31,
  0x6d, 0x23, 0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x20, 0x20,
  0x20, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x31, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x33, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x23, 0x20, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23, 0x23, 0x23, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23,
  0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x31, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x33, 0x6d, 0x23, 0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23, 0x20, 0x23, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x20,
  0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x20, 0x20, 0x20, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x23, 0x23,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23,
  0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33,
  0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0x23, 0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23, 0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x20, 0x23, 0x23, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x23, 0x23,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23,
  0x23, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33,
  0x6d, 0x23, 0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x32, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x20, 0x20, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x20, 0x20, 0x20, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x0a, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20,
  0x20, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23, 0x23, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0x20, 0x23,
  0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x32, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0x23, 0x20, 0x23, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0x23, 0x23,
  0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0x20, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x32, 0x6d, 0x23, 0x20, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x31, 0x6d, 0x23, 0x23, 0x23, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x20, 0x20, 0x20, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a, 0x00,
};
#else
unsigned char adversareval_googly_txt[] = {
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x20, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88,
  0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x33, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88,
  0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0xe2,
  0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96, 0x88,
  0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88,
  0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2,
  0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d,
  0x1b, 0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x20, 0x20, 0x20, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x0a, 0x2e, 0x20, 0x4d, 0x61, 0x74, 0x74, 0x68, 0x65, 0x77,
  0x20, 0x4d, 0x65, 0x72, 0x72, 0x69, 0x6c, 0x6c, 0x20, 0x2e, 0x20, 0x20,
  0x20, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96,
  0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d,
  0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96,
  0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,
  0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20,
  0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96,
  0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x20, 0x20, 0x20, 0x2e, 0x20, 0x67, 0x6f, 0x2e, 0x6d,
  0x61, 0x74, 0x74, 0x6d, 0x65, 0x72, 0x72, 0x2e, 0x63, 0x6f, 0x6d, 0x20,
  0x2e, 0x0a, 0x2e, 0x20, 0x32, 0x30, 0x31, 0x38, 0x20, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x20,
  0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2,
  0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31,
  0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31,
  0x6d, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x1b, 0x5b,
  0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96, 0x88,
  0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34,
  0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b,
  0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x1b, 0x5b, 0x30, 0x3b,
  0x3b, 0x6d, 0x20, 0x20, 0x20, 0x2e, 0x2e, 0x2e, 0x20, 0x2f, 0x61, 0x64,
  0x76, 0x65, 0x72, 0x73, 0x61, 0x72, 0x65, 0x76, 0x61, 0x6c, 0x20, 0x2e,
  0x0a, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x20, 0x1b,
  0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d,
  0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30,
  0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d, 0xe2, 0x96, 0x88, 0x20,
  0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33,
  0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0xe2, 0x96,
  0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b,
  0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b,
  0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x33, 0x6d, 0xe2, 0x96,
  0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x34, 0x6d, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96,
  0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x1b, 0x5b, 0x33, 0x32, 0x6d,
  0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b,
  0x6d, 0x1b, 0x5b, 0x33, 0x31, 0x6d, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,
  0xe2, 0x96, 0x88, 0x1b, 0x5b, 0x30, 0x3b, 0x3b, 0x6d, 0x20, 0x20, 0x20,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a, 0x00
};
#endif
