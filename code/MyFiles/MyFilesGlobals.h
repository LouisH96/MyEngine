#pragma once

#ifdef MY_FILES_EXPORTS
#define MY_FILES_API __declspec(dllexport)
#else
#define MY_FILES_API __declspec(dllimport)
#endif