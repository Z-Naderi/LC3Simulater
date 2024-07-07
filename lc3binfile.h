#ifndef LC3BINFILE_H
#define LC3BINFILE_H
#include <QFile>
#include <QString>
#include <QDebug>
#include "lc3memory.h"

extern LC3Memory memory;

class lc3binFile
{
public:
    lc3binFile();
    lc3binFile(QString);
    void writeToFile(const LC3Memory&, uint16_t, uint16_t);
    bool readFromFile(uint16_t);

    QFile file;
};

#endif // LC3BINFILE_H
