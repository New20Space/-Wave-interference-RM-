#ifndef OGLDEV_UTIL_H
#define OGLDEV_UTIL_H


#ifndef _WIN64
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <GL/glew.h>


using namespace std;

bool ReadFile1(const char* fileName, string& outFile);

#endif 