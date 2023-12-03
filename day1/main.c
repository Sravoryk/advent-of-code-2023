#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *control_strings[] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
};

struct range {
  int start;
  int end;
};

struct digit {
  struct range *r;
  int value;
  int pos;
};

void init_range(struct range *r) {
  r->start = -1;
  r->end = -1;
}

void init_digit(struct digit *d) {
  init_range(d->r);
  d->pos = -1;
  d->value = -1;
}

void set_digit(struct digit *d, int start, int end, int value, int pos) {
  d->r->start = start;
  d->r->end = end;
  d->value = value;
  d->pos = pos;
}

void reverse_str(const char *str, char *buff) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++) {
    buff[i] = str[len - (i + 1)];
  }
  buff[len] = '\0';
}

int min_digit(struct digit a, struct digit b) {
  if (a.value != -1 && b.value == -1)
    return a.value;
  else if (a.value == -1 && b.value != -1)
    return b.value;
  else
    return a.pos < b.pos ? a.value : b.value;
}

int max_digit(struct digit a, struct digit b) {
  if (a.value != -1 && b.value == -1)
    return a.value;
  else if (a.value == -1 && b.value != -1)
    return b.value;
  else
    return a.pos > b.pos ? a.value : b.value;
}

struct digit get_left_most_digit_str(const char *str) {
  struct digit d = {
      .r = &(struct range){.start = -1, .end = -1}, .value = -1, .pos = -1};

  size_t len = strlen(str);

  for (int i = 0; i < 9; i++) {
    char *ptr = strstr(str, control_strings[i]);
    if (ptr != NULL) {
      int pos = ptr - str;
      if (d.r->start == -1 || d.r->start > pos) {
        set_digit(&d, pos, pos + strlen(control_strings[i]) - 1, i + 1, pos);
      }
    }
  }

  printf("left STR value %d | start %d | end %d\n", d.value, d.r->start,
         d.r->end);

  return d;
}

struct digit get_right_most_digit_str(const char *str) {
  struct digit d = {
      .r = &(struct range){.start = -1, .end = -1}, .value = -1, .pos = -1};

  size_t line_len = strlen(str);
  char rev_line[line_len];
  reverse_str(str, rev_line);

  for (int i = 0; i < 9; i++) {
    char *current_digit = control_strings[i];

    size_t digit_len = strlen(current_digit);
    char rev_digit[digit_len];
    reverse_str(current_digit, rev_digit);

    char *ptr = strstr(rev_line, rev_digit);
    if (ptr != NULL) {
      int pos = ptr - rev_line;
      int pos2 = pos + digit_len - 1;
      if (d.r->start == -1 || d.r->start < line_len - pos2 - 1) {
        set_digit(&d, line_len - pos2 - 1, line_len - pos - 1, i + 1,
                  line_len - pos2 - 1);
      }
    }
  }

  printf("right STR value %d | start %d | end %d\n", d.value, d.r->start,
         d.r->end);

  return d;
}

struct digit get_left_most_digit(const char *str) {

  struct digit d = {
      .r = &(struct range){.start = -1, .end = -1}, .value = -1, .pos = -1};

  char left_most_digit = '\0';
  size_t len = strlen(str);

  for (int j = 0; j < len; j++) {
    char c = str[j];
    if (c >= '0' && c <= '9') {
      set_digit(&d, j, j, atoi(&c), j);
      break;
    }
  }
  printf("left INT value %d | start %d | end %d\n", d.value, d.r->start,
         d.r->end);
  return d;
}

struct digit get_right_most_digit(const char *str) {

  struct digit d = {
      .r = &(struct range){.start = -1, .end = -1}, .value = -1, .pos = -1};

  char right_most_digit = '\0';
  size_t len = strlen(str);

  for (int j = len - 1; j >= 0; j--) {
    char c = str[j];
    if (c >= '0' && c <= '9') {
      set_digit(&d, j, j, atoi(&c), j);
      break;
    }
  }
  printf("right INT value %d | start %d | end %d\n", d.value, d.r->start,
         d.r->end);
  return d;
}

int get_value_from_digits(char left_digit, char right_digit) {
  char val[] = {left_digit, right_digit, '\0'};
  return atoi(val);
}

int main(int argc, char *argv[]) {

  FILE *f = fopen("input", "r");
  if (f == NULL) {
    perror(__func__);
    exit(EXIT_FAILURE);
  }

  int sum = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, f)) != EOF) {
    int left_most, right_most;
    left_most =
        min_digit(get_left_most_digit_str(line), get_left_most_digit(line));
    right_most =
        max_digit(get_right_most_digit_str(line), get_right_most_digit(line));

    sum += (left_most * 10) + right_most;
  }

  printf("%d", sum);

  return 0;
}
