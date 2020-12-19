#pragma once

#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include "sandpile.h"

class make_sandpile{
public:
// forward declare a function for generating a random sandpile given a size and count
Sandpile generateSandpile(int S,int N);
// forward declare a function for reading a sandpile from a file given its filename
Sandpile readASandpile(ifstream &file);

};

