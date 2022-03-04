#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <math.h>
#include <chrono>
#include "AVLTree.cpp"


void printOutput(std::string file){

    printf("Processing %s\n", file.c_str());

    clock_t start, end;
    bool found;

    AVLTree<std::string> tree;

    // Start the timer for insertion
    start = clock();

    std::ifstream infile(file);
    std::string line, value;

    while (getline(infile, line)) {
        tree.insert(line.c_str());
    }

    printf("Duration to insert into AVL Tree: %fs", double(clock() - start)/double(CLOCKS_PER_SEC));


    // Searching from the found list
    start = clock();

    std::ifstream foundfile("found.txt");
    while (getline(foundfile, line)) {
        value = line.c_str();
        found = tree.find(value);

        if (!found) {
            std::cout << "Couldn't find: " << value << std::endl;
        }
    }

    printf("\nTotal duration to search from AVL Tree in found.txt: %fs", double(clock() - start)/double(CLOCKS_PER_SEC));

    // Searching from the notfound list
    start = clock();

    std::ifstream notfoundfile("notFound.txt");
    while (getline(notfoundfile, line)) {
        value = line.c_str();
        found = tree.find(line.c_str());

        if (found) {
            std::cout << "Could find: " << value << std::endl;
        }
    }

    printf("\nTotal duration to search from AVL Tree in notfound.txt: %fs\n\n", double(clock() - start)/double(CLOCKS_PER_SEC));


}

int main(int argc, const char *argv[]) {

    printOutput("setA.txt");
    printOutput("setB.txt");
    printOutput("setC.txt");

    return 0;
}
