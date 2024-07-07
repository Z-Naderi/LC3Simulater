#include "lc3binfile.h"
#include <QMessageBox>

LC3Memory memory(0xFFFF);

lc3binFile::lc3binFile()
{

}
lc3binFile::lc3binFile(QString filename)
{
    file.setFileName(filename);
}

// Function to write machine code to an output file
void lc3binFile::writeToFile(const LC3Memory& memory, uint16_t startAddress, uint16_t endAddress) {
    if (!file.open(QIODevice::WriteOnly)) {
        QString errorMessage = QString("Cannot open file for writing: %1").arg("MEMORY.bin");

        QMessageBox::warning(nullptr, "Warning", errorMessage);

        //qWarning() << "Cannot open file for writing:" << qPrintable("MEMORY.bin");

        return;
    }

    QDataStream out(&file);
    // Set the data stream version if necessary
    out.setVersion(QDataStream::Qt_5_0);

    for (uint16_t address = startAddress; address <= endAddress; ++address) {
        uint16_t value = memory.read(address);
        out << value;
    }

    file.close();
}

// Function to read instructions from a binary file and fill the memory
bool lc3binFile::readFromFile(uint16_t startAddress) {
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << qPrintable("MEMORY.bin");
        return false;
    }

    QDataStream in(&file);
    // Set the data stream version if necessary
    in.setVersion(QDataStream::Qt_5_0);

    uint16_t address = startAddress;
    while (!in.atEnd()) {
        uint16_t value;
        // Use the stream extraction operator to read a uint16_t value
        in >> value;
        memory.write(address, value);
        address++;
    }

    file.close();
    return true;
}
