/**
 * DESCRIPTION
 * 
 */
#ifndef _STRINGOPS_H_
#define _STRINGOPS_H_
#include <iostream>
#include <cstring>
#include <string>

char *StringToLower(char *str);
void writeInto(unsigned char *dest, unsigned char *src);
void writeInto(char *dest, char *src);
int readFile(unsigned char **buf);

#endif