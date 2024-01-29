#pragma GCC optimize("O3","unroll-loops")

#include "../utils/ArgParser.h"
#include "../utils/sandpile.h"


int main(int argc, char* argv[]){
    Sandpile Abelsandpile;
    ArgParser parser;
    parser.Parse(argv, argc);
    Abelsandpile.GetValues(parser.arglist);
    Abelsandpile.FromTSV(Abelsandpile.tsv_path);
    Abelsandpile.Spilling();
    return 0;
}
