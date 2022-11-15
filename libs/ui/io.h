#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "sys.h"

#include <wchar.h>

typedef int (*delimitador)(void*);

//IO
// int evaluarNombreDeProducto(char* Src);
int evaluarNombreDelCliente(char* Src);
// int evaluarExistencia(int* Src);
// int evaluarPrecio(char* Src);
int evaluarInt(int* Dest);
int evaluarDouble(double* Dest);
int evaluarUbicacion(char* Src);
int evaluarNumeroTelefonico(char* Src);
// int evaluarNumero(char* Dest);
int evaluarCorreo(char* Des);
int evaluarText(char* Dest, int lenght);

void cleanInput();

//STR
// int stringlen(const char* text);
// char* cp(char* dest, char* src);
// int cmp(char* dest, char* src);
// char* cat(char* dest, char* src);

//Conversiones
void int2str(int src, char* dest);
void double2str(double src, char* dest);

//File Manager
int getFileLines(char* filename);

//TEMP
<<<<<<< HEAD:libs/ui/io.h
int input(wchar_t* bg_titulo, wchar_t* titulo, void* dest, int (*funcion)(void*));
#endif
=======
int input(wchar_t* bg_titulo, wchar_t* titulo, void* dest, int (*funcion)(void*));
>>>>>>> ff6fdf7ba94ff897ebd530da505ff1f7c5ef1ec6:libs/UI/io.h
