#include "lib.h"
#include <iostream>

char *createBoard(std::size_t xdim, std::size_t ydim) {
  // create board array + set values to 0
  char *board = new char[xdim * ydim]{};

  return board;
}

void cleanBoard(char *board) {
  delete[] board;
  board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim) {

  // declare + define MASKs
  int marked = 0b00010000, hidden = 0b00100000;

  // go through array and print corresponding symbol + bitshift MASK to
  // corresponding bit and compare with the byte
  for (size_t x = 0; x < (xdim * ydim); x++) {

    // if marked
    if ((marked & *(board + x)) == marked) {
      std::cout << "M";
    }

    // if hidden, print *
    else if ((hidden & *(board + x)) == hidden) {
      std::cout << "*";
    }

    // if revealed print geese number
    else {

      int mask = 0b0001, count = 0;

      for (int y = 0; y < 4; y++) {
        mask <<= y;

        if ((*(board + x) & mask) == mask) {

          if (y == 0) {
            count += 1;
          }

          else if (y == 1) {
            count += 2;
          }

          else if (y == 2) {
            count += 4;
          }

          else {
            count += 8;
          }
        }

        mask = 0b001;
      }

      std::cout << count;
    }

    if (((x + 1) % (xdim)) == 0) {
      std::cout << std::endl;
    }
  }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {
  int nine = 0b1001;
  size_t x = 0;

  for (; x < (xdim * ydim); x++) {

    int count_geese_adj = 0;

    // if goose, do not count geese beside it
    if ((*(board + x) & nine) == nine) {
    }

    else {
      // count field directly to the left
      if ((x % xdim != 0) && (((*(board + x - 1) & nine) == nine))) {
        ++count_geese_adj;
      }

      // count field directly to the right
      if (((x + 1) % xdim != 0) && (((*(board + x + 1) & nine) == nine))) {
        ++count_geese_adj;
      }

      // count field directly above
      if ((x >= xdim) && (((*(board + x - xdim) & nine) == nine))) {
        ++count_geese_adj;
      }

      // count field directly below
      if (x <= ((xdim * ydim) - 1 - xdim) &&
          ((*(board + x + xdim) & nine) == nine)) {
        ++count_geese_adj;
      }

      // count field above and to the left
      if ((x >= xdim) && (x % xdim != 0) &&
          (*(board + x - xdim - 1) & nine) == nine) {
        ++count_geese_adj;
      }

      // count field above and to the right
      if ((x >= xdim) && ((x + 1) % xdim != 0) &&
          ((*(board + x - xdim + 1) & nine) == nine)) {
        ++count_geese_adj;
      }

      // count field below and to the left
      if (x <= ((xdim * ydim) - 1 - xdim) && (x % xdim != 0) &&
          ((*(board + x + xdim - 1) & nine) == nine)) {
        ++count_geese_adj;
      }

      // count field below and to the right
      if (x <= ((xdim * ydim) - 1 - xdim) && ((x + 1) % xdim != 0) &&
          ((*(board + x + xdim + 1) & nine) == nine)) {
        ++count_geese_adj;
      }
    }

    *(board + x) += count_geese_adj;
  }
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {
  for (size_t x = 0; x < (xdim * ydim); x++) {
    // declare + define MASK
    int hidden = 0b00100000;

    // set to 1 if it is not hidden
    if ((*(board + x) & hidden) != hidden) {
      *(board + x) ^= hidden;
    }
  }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
           std::size_t yloc) {
  // find coordinate on array
  int x = xloc + (yloc * xdim);

  // declare + define MASK
  int marked = 0b00010000, hidden = 0b00100000;

  // if revealed
  if ((hidden & *(board + x)) == 0) {
    return 2;
  }

  // if marked
  else if (((marked & *(board + x))) == marked) {
    return 1;
  }

  // if not revealed and no goose beside it
  else {

    // if goose
    if ((*(board + x) & 0b1001) == 0b1001) {
      *(board + x) ^= hidden;
      return 9;
    }

    else {
      // reveal specified field
      *(board + x) ^= hidden;

      // if there is a goose beside it and not revealed
      if (*(board + x) != 0b00000000) {
        return 0;
      }

      else {
        // reveal if not already revealed - do not reveal goose
        // reveal field directly to the left
        if ((x % xdim != 0) && (*(board + x - 1) & hidden) == hidden) {
          if ((*(board + x - 1) & 0b1001) != 0b1001) {
            *(board + x - 1) ^= hidden;
          }
        }

        // reveal field directly to the right
        if (((x + 1) % xdim != 0) && (*(board + x + 1) & hidden) == hidden) {
          if ((*(board + x + 1) & 0b1001) != 0b1001) {
            *(board + x + 1) ^= hidden;
          }
        }

        // reveal field directly above
        if ((x >= xdim) && (*(board + x - xdim) & hidden) == hidden) {
          if ((*(board + x - xdim) & 0b1001) != 0b1001) {
            *(board + x - xdim) ^= hidden;
          }
        }

        // reveal field directly below
        if ((x <= ((xdim * ydim) - 1 - xdim)) &&
            (*(board + x + xdim) & hidden) == hidden) {
          if ((*(board + x + xdim) & 0b1001) != 0b1001) {
            *(board + x + xdim) ^= hidden;
          }
        }

        // reveal field above and to the left
        if ((x >= xdim) && (x % xdim != 0) &&
            (*(board + x - xdim - 1) & hidden) == hidden) {
          if ((*(board + x - xdim - 1) & 0b1001) != 0b1001) {
            *(board + x - xdim - 1) ^= hidden;
          }
        }

        // reveal field above and to the right
        if ((x >= xdim) && ((x + 1) % xdim != 0) &&
            (*(board + x - xdim + 1) & hidden) == hidden) {
          if ((*(board + x - xdim + 1) & 0b1001) != 0b1001) {
            *(board + x - xdim + 1) ^= hidden;
          }
        }

        // reveal field below and to the left
        if ((x <= ((xdim * ydim) - 1 - xdim)) && (x % xdim != 0) &&
            ((*(board + x + xdim - 1) & hidden) == hidden)) {
          if ((*(board + x + xdim - 1) & 0b1001) != 0b1001) {
            *(board + x + xdim - 1) ^= hidden;
          }
        }

        // reveal field below and to the right
        if ((x <= ((xdim * ydim) - 1 - xdim)) && ((x + 1) % xdim != 0) &&
            ((*(board + x + xdim + 1) & hidden) == hidden)) {
          if ((*(board + x + xdim + 1) & 0b1001) != 0b1001) {
            *(board + x + xdim + 1) ^= hidden;
          }
        }

        return 0;
      }
    }
  }
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
         std::size_t yloc) {
  // find coordinate on array
  int location = xloc + (yloc * xdim);

  // declare and define mask
  int marked = 0b00010000, hidden = 0b00100000;

  // if revealed
  if ((hidden & *(board + location)) != hidden) {
    return 2;
  }

  // if not marked or marked
  else {
    *(board + location) ^= marked;
    return 0;
  }
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) {
  int nine = 0b1001, hidden = 0b00100000, counter = 0, marked = 0b00010000;

  for (size_t x = 0; x < (xdim * ydim); x++) {

    // if goose and hidden
    if (((*(board + x) & nine) == nine) &&
        (((hidden & *(board + x)) == hidden) ||
         ((hidden & *(board + x)) == marked))) {
      ++counter;
    }

    // if not goose and revealed
    else if (((*(board + x) & nine) != nine) &&
             ((hidden & *(board + x)) != hidden)) {
      ++counter;
    }

    // if not goose and not revealed
    else {
      return false;
    }
  }

  // if all fields meet criteria, return true for won
  if (counter == (xdim * ydim)) {
    return true;
  }

  else {
    return false;
  }
}