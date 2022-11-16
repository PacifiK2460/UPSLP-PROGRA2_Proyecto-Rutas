#include "../win.h"


void innit(){
  #ifdef _WIN32
    setConsoleMode(ENABLE_VIRTUAL_TERMINAL_PROCESSING); 
  #endif
  printf("\e[?1049h");
  setlocale(LC_ALL, "");
  srand(time(NULL));
}

void winprint(WINDOW* window,int X, int Y, wchar_t* text){
  X+= getx(window);
  Y+= gety(window);
  wprintf(L"\e[%i;%iH%ls"RESET,Y,X,text);
}

void getcolsrows(WINDOW* Window, int* COLS, int* ROWS){
  if(Window == STDOUTPUT){
    #ifdef _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      COLS = csbi.srWindow.Bottom - (int)csbi.srWindow.Top + 1;
      ROWS = csbi.srWindow.Right - (int)csbi.srWindow.Left + 1;
    #endif

    #ifdef __linux__
      struct winsize w;
      ioctl(1, 0x5413, &w);

      *COLS = (int) w.ws_col;
      *ROWS = (int) w.ws_row;
    #endif
    return;
  }

  *COLS = Window->COLS;
  *ROWS = Window->ROWS;
  return;
}

int getcols(WINDOW* Window){
  if(Window == STDOUTPUT){
    #ifdef _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      return csbi.srWindow.Bottom - (int)csbi.srWindow.Top + 1;
    #endif

    #ifdef __linux__
      struct winsize w;
      ioctl(1, 0x5413, &w);

      return (int) w.ws_col;
    #endif
  }

  return Window->COLS;
}

int getrows(WINDOW* Window){
  if(Window == STDOUTPUT){
    #ifdef _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      return csbi.srWindow.Right - (int)csbi.srWindow.Left + 1;
    #endif

    #ifdef __linux__
      struct winsize w;
      ioctl(1, 0x5413, &w);

      return (int) w.ws_row;
    #endif
  }
  return Window->ROWS;
}

void getxy(WINDOW* Window, int* X, int* Y){
  if(Window == STDOUTPUT){
    X = 0;
    Y = 0;
    return;
  }

  *X = getx(Window);
  *Y = gety(Window);
}

int getx(WINDOW* Window){
  if(Window == STDOUTPUT){
    return 0;
  }

  return Window->X;
}

int gety(WINDOW* Window){
  if(Window == STDOUTPUT){
    return 0;
  }

  return Window->Y;
}

void printinthemiddle(WINDOW* Window, int Y, wchar_t* texto){
  int X = getcols(Window);
  int tam = wcslen(texto);
  X = X - tam;
  X = X / 2;
  Y+=1;
  winprint(Window,X,Y, texto);
}

void printinthemiddlesize(WINDOW* Window, int Y, wchar_t* texto, int tam){
  int X = getcols(Window);
  X = X - tam;
  X = X / 2;
  Y+=1;
  winprint(Window,X,Y,texto);
}

void printMessage(wchar_t* msg){
  NEW_SCREEN();
  {
    clearerr(stdin);
    wchar_t text[1024] = DIM;
    wcscat(text, L" ");
    wcscat(text, msg);
    wcscat(text, L" " RESET);

    printinthemiddle(STDOUTPUT,getrows(STDOUTPUT)/2,text);
    winprint(STDOUTPUT,4,getrows(STDOUTPUT)-2,RESET FRGB(185, 251, 192)  "cualquier tecla"  RESET DIM  " aceptar ");
    cleanInput();
    getchar();
  }
  CLOSE_SCREEN();
}

void printHelp(wchar_t* text, int X, int Y){
  winprint(STDOUTPUT,X,Y,text);
}

Result initTUI()
{
    Result result;

    result.Error_state = OK;

    setlocale(LC_ALL, "C.UTF-8");
    setlocale(LC_CTYPE, "C.UTF-8");

    NEW_SCREEN();

    return result;
}