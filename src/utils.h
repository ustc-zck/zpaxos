#pragma once
#include <chrono>
#include <time.h> 
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


//trim space...
const std::string WHITESPACE = " \n\r\t\f\v";
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

uint64_t GetCurrentMillSeconds();
uint64_t GetCurrentMicroSeconds();

std::string ReadFromFile(std::string filePath);

void WriteIntoFile(std::string filePath, std::string content);

std::vector<std::string> SplitStr(std::string str, char delimiter);

float GenerateRandomNumber();

std::vector<std::string> SplitStr(std::string str, char delimiter);



