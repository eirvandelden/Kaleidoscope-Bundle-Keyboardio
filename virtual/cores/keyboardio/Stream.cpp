#include "Stream.h"

namespace {

bool looksLikeDigit(int c) {
  return c >= '0' && c <= '9';
}

bool startsANumber(int c) {
  return looksLikeDigit(c) || c == '-';
}

bool maySkip(int c, LookaheadMode lookahead) {
  switch (lookahead) {
  case SKIP_ALL:
    return true;
  case SKIP_WHITESPACE:
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
  default:
    return false;
  }
}

}  // namespace

// A virtual keyboard is handed its whole input up front, so running out of
// characters ends the parse where the Arduino implementation would start
// waiting for more to arrive.
long
Stream::parseInt(LookaheadMode lookahead, char ignore) {
  while (available() && !startsANumber(peek())) {
    if (peek() != ignore && !maySkip(peek(), lookahead)) {
      return 0;
    }
    read();
  }

  if (!available()) {
    return 0;
  }

  bool negative = (peek() == '-');
  if (negative) {
    read();
  }

  long value = 0;
  while (available() && (looksLikeDigit(peek()) || peek() == ignore)) {
    int c = read();
    if (c != ignore) {
      value = value * 10 + (c - '0');
    }
  }

  return negative ? -value : value;
}
