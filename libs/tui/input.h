#ifndef INPUT_H
#define INPUT_H 1

extern struct Result;
extern enum input_type;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13

#define KEY_BACKSPACE 8
#define KEY_DELETE 127

#define KEY_ESC 27
#define KEY_TAB 9
#define KEY_SHIFT_n_TAB 353

struct Result input(enum Input_Type type, wchar_t *buffer, int buffer_size);

#endif