#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <stdarg.h>
#include <time.h>
#include "io.h"

#include <wchar.h>

#include "../core/core.h"
#include <wchar.h>

#include <assert.h>
#define assertm(exp, msg) assert(((void)msg, exp))


#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __linux__
  #include <sys/ioctl.h>
  #include <unistd.h>
  #include <termios.h>

  #include <string.h>
  #include <sys/select.h>
  #include <sys/time.h>
#endif

#define NEW_SCREEN() wprintf(L"\e[?1049h")
#define CLOSE_SCREEN() wprintf(L"\e[?1049l")

#define VLINE  L"│"
#define HLINE  L"─"
#define TLLINE L"╭"
#define TRLINE L"╮"
#define BLLINE L"╰"
#define BRLINE L"╯"

#define MENUVLINE L"▏"

#define RESET     L"\e[0m"
// #define NONE      L""

// Tipos
#define BOLD      L"\e[1m"
#define DIM       L"\e[2m"
#define ITALIC    L"\e[3m"
#define INVERSE   L"\e[7m"

// Colores
#define STR(X) #X
#define BRGB(R,G,B) L"\e[48;2;" STR(R) STR(;) STR(G) STR(;) STR(B) STR(m)
#define FRGB(R,G,B) L"\e[38;2;" STR(R) STR(;) STR(G) STR(;) STR(B) STR(m)

// MISC
#define HIDE_CURSOR L"\e[?25l"
#define SHOW_CURSOR L"\e[?25h"
#define CLEAR L"\e[2J"

#define STDOUTPUT NULL

// Posición del Cursor
void noEcho();
void echo();

// Window.h
typedef struct _WINDOW
{
  int X;
  int Y;
  int COLS;
  int ROWS;

  struct WINDOW* Parent;
}WINDOW;

typedef int (*Funciones)(void*);

typedef struct _MENU{
    WINDOW* Parent;
    int X;
    int Y;
    int ROWS;
    int PAGE;
    int PAGES;
    wchar_t** opciones;
    wchar_t** descripcion;
    Funciones __before;
    void* __before_args;
    int selected;
} MENU;

typedef struct _TABLE{
    int columas, filas;
    int currentFilledRow;
    //Bidimencional: texto mas largo por columna
    int* textoMasLargo;
    //Tridimencional: columna, fila y texto
    int total;
    int toalTeorico;
    char** headers;
    struct FILA* data;
} TABLE;

void setMenuData(MENU* Destination,WINDOW* Parent, int x, int y,int pages, int rows,wchar_t* opciones[], wchar_t* descripciones[],Funciones __before, void *__before_args);

void innit();

WINDOW* newWin(int y, int x, int COLS, int ROWS, WINDOW* Parent);
void winprint(WINDOW* window,int X, int Y, wchar_t* text);
void printinthemiddle(WINDOW* Window, int Y,wchar_t* texto);
void printinthemiddlesize(WINDOW* Window, int Y, wchar_t* texto, int tam);
void box(WINDOW* Window);
void getcolsrows(WINDOW* Window, int* COLS, int* ROWS);
void getxy(WINDOW* Window, int* X, int* Y);
int getcols(WINDOW* Window);
int getrows(WINDOW* Window);
int getx(WINDOW* Window);
int gety(WINDOW* Window);

void printMessage(wchar_t* text);
void printHelp(wchar_t* text, int X, int Y);

// Menu.h
//MENU* newMenu(WINDOW* Parent, int x, int y, int COLS, int ROWS,char** opciones,char** descripciones, int cant);
void focusMenu(MENU* menu); 
void updateMenu(MENU* menu); 

// Table.h
// TABLE* newTable(int columnas, int filas);
void tableSetHeaders(TABLE *src,char** headers);
void tableAppendRow(TABLE *src, ...);
void tablePrepareDataAling(TABLE *src);
void printTable(TABLE* table, int x, int y);

void prepareTableData(TABLE* src, int columnas, int filas);
void setTableData(char* dest, char* src);
int getTotal(TABLE* src);
int getTotalToerico(TABLE* src);
void freeTable(TABLE *src);


/**
 * @brief The TUI class
 * This class is the driver for the TUI in this proyect.
 */

Result initTUI();