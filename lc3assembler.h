#ifndef LC3ASSEMBLER_H
#define LC3ASSEMBLER_H

#include <QString>
#include "lc3memory.h"
#include "lc3binfile.h"


extern lc3binFile globalFile;
class lc3Assembler
{
public:
    lc3Assembler();
};

#endif // LC3ASSEMBLER_H

int startAssembly( QString inputFilename);

