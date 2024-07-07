#include "lc3assembler.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <bitset>
#include <QRegularExpression>
#include <QMessageBox>
lc3binFile globalFile("MEMORY.bin");

lc3Assembler::lc3Assembler()
{

}

// Trim function that removes leading and trailing whitespace from a QString
QString trim(const QString &str)
{
    return str.trimmed();
}

// Helper function to split a string into tokens (Separator character)
QVector<QString> split(const QString &str, QChar delimiter)
{
    return str.split(delimiter).toVector();
}

// Function to convert an integer to a binary string with a specified number of bits
QString ToBinary(int value, int bits)
{
    if (value < 0)
    {
        value = (1 << bits) + value;
        return QString::fromStdString(std::bitset<16>(value).to_string()).right(bits).rightJustified(bits, '1');
    }
    else
    {
        return QString::fromStdString(std::bitset<16>(value).to_string()).right(bits).rightJustified(bits, '0');
    }
}

// Function to read an assembly file and return its lines
QVector<QString> ReadFile(const QString &filename)
{
    QVector<QString> lines;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Cannot open file for reading: " + filename + "\nError: " + file.errorString());
        return lines;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        lines.append(trim(line));
    }
    file.close();
    return lines;
}

// First pass to identify labels and their corresponding memory addresses
QMap<QString, uint16_t> FirstPass(const QVector<QString> &lines)
{
    QMap<QString, uint16_t> labels;
    // Starting address
    uint16_t address = 0x3000;
    for (const QString &line : lines)
    {
        if (line.isEmpty() || line.startsWith(';'))
            continue;

        QRegularExpression re("\\s+");
            QVector<QString> tokens = line.split(re, Qt::SkipEmptyParts);
        if (tokens[0] == "ORG")
        {
            QString addrString = tokens[1];
            bool ok;
            uint16_t newAddress = addrString.toInt(&ok, 16);
            if (!ok)
            {
                QMessageBox::warning(nullptr, "Error", "Error converting address: " + addrString);
            }
            else
            {
                address = newAddress;
                //QMessageBox::information(nullptr, "Info", "Setting address to: " + QString::number(address, 16).toUpper());
            }
            continue;
        }
        else if (tokens[0].endsWith(','))
        {
            labels[tokens[0].chopped(1)] = address;
            if (tokens.size() > 1)
            {
                // If there's an instruction on the same line, increment address
                address++;
            }
        }
        else if (tokens[0] == "END")
        {
            break;
        }
        else
        {
            address++;
        }
    }
    return labels;
}

/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// Function to assemble a single instruction into binary
QString AssembleInstruction(const QString &instruction, const QMap<QString, uint16_t> &labels, uint16_t currentAddress)
{
    //QVector<QString> tokens = split(instruction, ' ');
    QRegularExpression re("\\s+");
        QVector<QString> tokens = instruction.split(re, Qt::SkipEmptyParts);
    for (int i = 0; i < tokens.size(); ++i)
    {
        tokens[i] = tokens[i].remove(',').trimmed();
    }
    QString opcode = tokens[0];

    if (opcode == "ADD" || opcode == "AND")
    {
        QString result="";
        if(opcode=="ADD"){
            result+="0001";
             }
            else{
               result+="0101";
            }

        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        QString sr1 = ToBinary(tokens[2].mid(1).toInt(), 3);
        if (tokens[3].startsWith('R'))
        {
            QString sr2 = ToBinary(tokens[3].mid(1).toInt(), 3);
            return result + dr + sr1 + "000" + sr2;
        }
        else
        {
            int imm5 = tokens[3].mid(1).toInt();
            return result + dr + sr1 + "1" + ToBinary(imm5, 5);
        }
    }

    if (opcode.startsWith("BR"))
    {
        // Check if 'n' condition flag is present
        QString n = tokens[0].contains('n') ? "1" : "0";
        // Check if 'z' condition flag is present
        QString z = tokens[0].contains('z') ? "1" : "0";
        // Check if 'p' condition flag is present
        QString p = tokens[0].contains('p') ? "1" : "0";

        int offset = labels.value(tokens[1]) - currentAddress - 1;
        QString offsetBinary = ToBinary(offset, 9);
        return "0000" + n + z + p + offsetBinary;
    }

    if (opcode == "JMP")
    {
        QString BaseR = tokens[1];
        // Remove 'R' and convert to int
        int BaseRNum = BaseR.mid(1).toInt();

        // Construct binary instruction
        QString binaryInstruction = "1100000";
        // Opcode 1100 and 000
        binaryInstruction += ToBinary(BaseRNum, 3);
        binaryInstruction += "000000";

        return binaryInstruction;
    }

    if (opcode == "JSR")
    {
        // Subroutine label
        QString label = tokens[1];
        uint16_t subroutineAddress = labels.value(label);

        // Calculate the offset from current address
        int16_t offset = subroutineAddress - currentAddress - 1;

        // Convert offset to binary with 11 bits
        QString offsetBinary = ToBinary(offset, 11);

        // Construct the binary instruction
        QString binaryInstruction = "0100";
        // JSR indirect bit
        binaryInstruction += "1";
        binaryInstruction += offsetBinary;

        return binaryInstruction;
    }

    if (opcode == "JSRR")
    {
        QString BaseR = tokens[1];
        // Remove 'R' and convert to int
        int BaseRNum = BaseR.mid(1).toInt();
        // Construct binary instruction
        QString binaryInstruction = "0100000";
        binaryInstruction +=ToBinary(BaseRNum, 3);
        binaryInstruction += "000000";
        return binaryInstruction;
    }

    if (opcode == "LD")
    {
        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        int offset = labels.value(tokens[2]) - currentAddress - 1;
        return "0010" + dr + ToBinary(offset, 9);
    }

    if (opcode == "LDI")
    {
        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        int offset = labels.value(tokens[2]) - currentAddress - 1;
        return "1010" + dr + ToBinary(offset, 9);
    }

    if (opcode == "LDR")
    {
        // Destination register
        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        // Base register
        QString baser = ToBinary(tokens[2].mid(1).toInt(), 3);
        // Offset value
        int offset = tokens[3].mid(1).toInt();
        return "0110" + dr + baser + ToBinary(offset, 6);
    }

    if (opcode == "LEA")
    {
        // Destination register
        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        int offset = labels.value(tokens[2]) - currentAddress - 1;
        return "1110" + dr + ToBinary(offset, 9);
    }

    if (opcode == "NOT")
    {
        QString dr = ToBinary(tokens[1].mid(1).toInt(), 3);
        QString sr = ToBinary(tokens[2].mid(1).toInt(), 3);
        return "1001" + dr + sr + "111111";
    }

    if (opcode == "RET")
    {
        return "1100000111000000";
    }

    if (opcode == "ST")
    {
        // Source register
        QString sr = ToBinary(tokens[1].mid(1).toInt(), 3);
        // Calculate offset
        int offsetValue = labels.value(tokens[2]) - currentAddress - 1;
        QString offset = ToBinary(offsetValue, 9);
        return "0011" + sr + offset;
    }

    if (opcode == "STI")
    {
        // Source register
        QString sr = ToBinary(tokens[1].mid(1).toInt(), 3);
        QString offset = ToBinary(labels.value(tokens[2]) - currentAddress - 1, 9);
        return "1011" + sr + offset;
    }

    if (opcode == "STR")
    {

        QString sr = ToBinary(tokens[1].mid(1).toInt(), 3);

        QString baseR = ToBinary(tokens[2].mid(1).toInt(), 3);
        // Offset (Remove the '#' and convert to integer)
        int offset6 = tokens[3].mid(1).toInt();
        return "0111" + sr + baseR + ToBinary(offset6, 6);
    }

    if (opcode == "HALT")
    {
        return "1111000000100101";
    }

    if (opcode == "BYTE")
    {
        QString result;
        for (int i = 1; i < tokens.size(); ++i)
        {
            bool ok;
            uint8_t value = tokens[i].toInt(&ok, 16);
            if (!ok || value > 0xFF)
            {
                QMessageBox::warning(nullptr, "Invalid .BYTE value", "Invalid .BYTE value: " + tokens[i]);
                continue;
            }
            result += ToBinary(value, 8).rightJustified(16, '0') + "\n";
        }
        return result.trimmed();
    }

    if (opcode == "DEC")
    {
        bool ok;
        int16_t value = tokens[1].toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(nullptr, "Invalid .DEC value", "Invalid .DEC value: " + tokens[1]);
            return "";
        }
        return ToBinary(value, 16);
    }

    if (opcode == "HEX")
       {
           bool ok;
           uint16_t value = tokens[1].toUInt(&ok, 16);
           if (!ok)
           {
               QMessageBox::warning(nullptr, "Invalid .DEC value", "Invalid .DEC value: " + tokens[1]);
               return "";
           }
           return ToBinary(value, 16);
       }

    return "";
}

QVector<QString> splitWithoutComments(const QString &line, const QChar &separator)
{
    QVector<QString> tokens;
    QString currentToken;
    bool commentFound = false;

    for (int i = 0; i < line.size(); ++i)
    {
        QChar ch = line.at(i);

        if (ch == ';')
        { // If comment symbol found, break the loop
            commentFound = true;
            break;
        }
        else if (ch == separator)
        { // If separator found, add current token to tokens
            if (!currentToken.isEmpty())
            {
                tokens.append(currentToken.trimmed());
                currentToken.clear();
            }
        }
        else
        { // Otherwise, add character to current token
            currentToken += ch;
        }
    }

    // Add the last token if not empty
    if (!currentToken.isEmpty())
    {
        tokens.append(currentToken.trimmed());
    }

    // If a comment was found, ignore the rest of the line

    return tokens;
}

bool ValidateInstruction(const QVector<QString> &tokens, const QMap<QString, uint16_t> &labels)
{
    if (tokens.isEmpty())
        return false;

    QString opcode = tokens[0].trimmed(); // Trim any leading or trailing whitespace

    auto isRegister = [](const QString &token)
    {
        int regNum = token.at(1).digitValue(); // Get the digit after 'R'

        bool hey = token.startsWith('R') && regNum >= 0 && regNum <= 7;
        return hey;
    };

    if (opcode == "ADD" || opcode == "AND")
    {
        if (tokens.size() != 4)
            return false;
        if (!isRegister(tokens[1]) || !isRegister(tokens[2]))
            return false;
        if (tokens[3].startsWith('R'))
        {
            if (!isRegister(tokens[3]))
                return false;
        }
        else
        {
            bool ok;
            int imm = tokens[3].mid(1).toInt(&ok);
            if (!ok || imm < -16 || imm > 15)
                return false;
        }
    }
    else if (opcode.startsWith("BR"))
    {
        if (tokens.size() != 2)
            return false;
        if (!labels.contains(tokens[1]))
            return false;
    }
    else if (opcode == "JMP" || opcode == "JSRR")
    {
        if (tokens.size() != 2)
            return false;
        if (!isRegister(tokens[1]))
            return false;
    }
    else if (opcode == "JSR")
    {
        if (tokens.size() != 2)
            return false;
        if (!labels.contains(tokens[1]))
            return false;
    }
    else if (opcode == "LD" || opcode == "LDI" || opcode == "LEA" || opcode == "ST" || opcode == "STI")
    {
        if (tokens.size() != 3)
            return false;
        if (!isRegister(tokens[1]))
            return false;
        if (!labels.contains(tokens[2]))
            return false;
    }
    else if (opcode == "LDR" || opcode == "STR")
    {
        if (tokens.size() != 4)
            return false;
        if (!isRegister(tokens[1]) || !isRegister(tokens[2]))
            return false;
        bool ok;
        int offset = tokens[3].toInt(&ok);
        if (!ok || offset < -32 || offset > 31)
            return false;
    }
    else if (opcode == "NOT")
    {
        if (tokens.size() != 3)
            return false;
        if (!isRegister(tokens[1]) || !isRegister(tokens[2]))
            return false;
    }
    else if (opcode == "RET" || opcode == "HALT" || opcode == "END")
    {
        if (tokens.size() != 1)
            return false;
    }
    else if (opcode == "WORD" || opcode == "BYTE")
    {
        if (tokens.size() != 2)
            return false;
        bool ok;
        tokens[1].toUInt(&ok);
        if (!ok)
            return false;
    }
    else if (opcode == "DEC")
    {
        if (tokens.size() != 2)
            return false;
        bool ok;
        tokens[1].toInt(&ok);
        if (!ok)
            return false;
    }

    else if (opcode == "HEX")
    {
        if (tokens.size() != 2)
            return false;
        bool ok;
        tokens[1].toUInt(&ok, 16); // Validate hex value
        if (!ok)
            return false;
    }
    else
    {
        qWarning() << "Invalid opcode:" << opcode;
        return false;
    }

    return true;
}

// Second pass to convert assembly instructions to machine code
void SecondPass(const QVector<QString> &lines, const QMap<QString, uint16_t> &labels, LC3Memory &memory)
{
    uint16_t address = 0x3000; // Starting address
    for (const QString &line : lines)
    {

        if (line.isEmpty() || line.startsWith(';'))
            continue; // Ignore empty lines and comments

        //QVector<QString> tokens = split(line, ' ');
        QRegularExpression re("\\s+");
            QVector<QString> tokens = line.split(re, Qt::SkipEmptyParts);

        if (tokens.size() > 1 && tokens[0] == "ORG")
        {
            QString addrString = tokens[1];
            bool ok;
            uint16_t newAddress = static_cast<uint16_t>(addrString.toInt(&ok, 16));
            if (!ok)
            {
                QMessageBox::warning(nullptr, "Error", "Error converting address: " + addrString);

            }
            else
            {
                address = newAddress; // Set starting address
                qDebug() << "Setting address to:" << address;
            }
        }

        else if (tokens.size() >= 1 && tokens[0] == "END")
        {
            break; // End of the program
        }

        else if (tokens[0].endsWith(','))
        {
            // This is a label, process the next token as an instruction if it exists
            if (tokens.size() > 1)
            {
                // Extract the instruction part after the label and comma
                QString instruction = line.mid(line.indexOf(',') + 1).trimmed();

                // Split the instruction into tokens excluding comments
                QVector<QString> tokens2 = splitWithoutComments(instruction, ' ');

                if (ValidateInstruction(tokens2, labels))
                {
                    QString binaryInstruction = AssembleInstruction(instruction, labels, address);
                    bool ok;
                    uint16_t machineCode = static_cast<uint16_t>(binaryInstruction.toUInt(&ok, 2));
                    if (!ok)
                    {
                        QMessageBox::warning(nullptr, "Warning", "Failed to convert binary instruction to machine code");

                        continue; // Skip writing invalid instruction to memory
                    }
                    memory.write(address, machineCode);
                    address++;
                }
                else
                {
                    QMessageBox::warning(nullptr, "Warning", "Skipping invalid instruction: " + line);

                }
            }
        }

        else
        {
            QVector<QString> tokens2 = splitWithoutComments(line, ' ');
            if (ValidateInstruction(tokens2, labels))
            { // validateInstruction(tokens, labels) fix
                // Regular instruction without label
                QString binaryInstruction = AssembleInstruction(line, labels, address);

                bool ok;
                uint16_t machineCode = static_cast<uint16_t>(binaryInstruction.toUInt(&ok, 2));
                if (!ok)
                {
                    QMessageBox::critical(nullptr, "Error", "Failed to convert binary instruction to machine code");

                    continue; // Skip writing invalid instruction to memory
                }

                memory.write(address, machineCode); // Write machine code to memory
                address++;                          // Increment address
            }
            else
            {
                QMessageBox::warning(nullptr, "Warning", "Skipping invalid instruction: " + line);

            }
        }
    }
}

// Main function to assemble and simulate LC-3 program
    int startAssembly(QString inputFilename)
    {

        // Assemble the code
        QVector<QString> assemblyCode = ReadFile(inputFilename);
        if (assemblyCode.isEmpty())
        {
            qWarning() << "Failed to assemble. Exiting...";
            return 1; // Return error code
        }

        QMap<QString, uint16_t> labels = FirstPass(assemblyCode);

        LC3Memory tempMemory(0xFFFF); // Create memory with size 0xFFFF (64KB)

        SecondPass(assemblyCode, labels, tempMemory);
        // Write assembled code to output file
        globalFile.writeToFile(tempMemory, 0x3000, 0x3000 + assemblyCode.size() - 1);

        //QMessageBox::information(nullptr, "Assembly Completed", "Assembly completed. Output written to MEMORY.bin");
    }
///////////////////////////////////////////////////////////////////////////////////
