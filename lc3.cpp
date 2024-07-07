#include "lc3.h"
#include "ui_lc3.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QScrollBar>

LC3Registers registers;
LC3Instructions instructions;

int index;

lc3::lc3(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::lc3)
{
    ui->setupUi(this);
    memoryFill();
}

lc3::~lc3()
{
    delete ui;
}

void lc3::updateRegisters()
{
    ui->R0_in->setText("0x" + QString::number(registers.getR(0), 16).toUpper());
    ui->R1_in->setText("0x" + QString::number(registers.getR(1), 16).toUpper());
    ui->R2_in->setText("0x" + QString::number(registers.getR(2), 16).toUpper());
    ui->R3_in->setText("0x" + QString::number(registers.getR(3), 16).toUpper());
    ui->R4_in->setText("0x" + QString::number(registers.getR(4), 16).toUpper());
    ui->R5_in->setText("0x" + QString::number(registers.getR(5), 16).toUpper());
    ui->R6_in->setText("0x" + QString::number(registers.getR(6), 16).toUpper());
    ui->R7_in->setText("0x" + QString::number(registers.getR(7), 16).toUpper());
    ui->MAR_in->setText("0x" + QString::number(registers.getMAR(), 16).toUpper());
    ui->MDR_in->setText("0x" + QString::number(registers.getMDR(), 16).toUpper());
    ui->Negative_in->setText("0x" + QString::number((registers.getCC() >> 2) & 0x1, 16).toUpper());
    ui->PC_in->setText("0x" + QString::number(registers.getPC(), 16).toUpper());
    ui->Positive_in->setText("0x" + QString::number(registers.getCC() & 0x1, 16).toUpper());
    ui->Zero_in->setText("0x" + QString::number((registers.getCC() >> 1) & 0x1, 16).toUpper());
    ui->IR_in->setText("0x" + QString::number(registers.getIR(), 16).toUpper());
}


///////////////////////////////////////////////////////////////
void lc3::updateMemory(int index)
{
    if (index < ui->memoryTable->rowCount())
    {
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString("0x%1").arg(memory.read(index), 4, 16, QChar('0')).toUpper());
        valueItem->setData(Qt::ForegroundRole, QColor(Qt::blue));
        ui->memoryTable->setItem(index, 1, valueItem);
        // Scroll to the updated memory location
        ui->memoryTable->scrollToItem(valueItem, QAbstractItemView::PositionAtCenter);
    }
}

/////////////////////////////////////////////////////////////
void lc3::on_RUN_clicked()
{
    if(flag!=1){
        QMessageBox::warning(nullptr, "Error", "No file selected. Please upload a file before running.");
        return;
    }
    if (sc == -1)
    {
        // HALT
        if (instructions.IsHalt())
        {
            QMessageBox::information(nullptr, "Program Done", "The program has reached the HALT instruction and is done.");
        }
    }
    else if (sc == 1)
    {
        instructions.fetch(memory);
        if (instructions.IsHalt())
        {
            QMessageBox::information(nullptr, "Program Done", "The program has reached the HALT instruction and is done.");
            sc = -1;
        }
        else
        {
            updateRegisters();
            ui->Phase->setText("Fetch");
            sc++;
        }
    }
    else if (sc == 2)
    {
        instructions.decode();
        updateRegisters();
        ui->Phase->setText("Decode");
        sc++;
    }
    else if (sc == 3)
    {
        instructions.evaluateAddress(memory);
        updateRegisters();
        ui->Phase->setText("Evaluate Address");
        sc++;
    }
    else if (sc == 4)
    {
        instructions.fetchOperands(memory);
        updateRegisters();
        ui->Phase->setText("Fetch Operands");
        sc++;

    } else if(sc == 5){
        instructions.execute();
        updateRegisters();
        ui->Phase->setText("Execute");
        sc++;
    }
    else if (sc == 6)
    {
        instructions.store(memory);
        updateRegisters();
        ui->Phase->setText("Store");
        updateMemory(index);
        sc = 1;
    }
}

void lc3::on_Upload_code_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), "", tr("Assembly Files (*.asm)"));
    if (!fileName.isEmpty())
    {
        if (fileName.endsWith(".asm", Qt::CaseInsensitive))
        {
            QMessageBox::information(nullptr, tr("File Selected"), tr("File has been successfully selected."));

            startAssembly(fileName);
            globalFile.readFromFile(0x3000);
            registers.setPC(0x3000);
            index = 0x3000;
            memoryFill();
            updateMemory(index); // Ensure memory is filled and visible
            flag=1;
        }

        else
        {
            QMessageBox::warning(nullptr, tr("Invalid File"), tr("Please select a file with an .asm extension."));

        }
    }

    else
    {
        QMessageBox::warning(nullptr, tr("No File Selected"), tr("No file was selected."));
    }

}
/////////////////////////////////
void lc3::memoryFill()
{

    ui->memoryTable->setRowCount(0xFFFF);
    ui->memoryTable->setColumnCount(2);
    QStringList headers = {"Address", "Value"};
    ui->memoryTable->setStyleSheet(
        "QTableWidget { "
        "border: 1px solid #d0d0d0; "
        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(200, 158, 183, 255), stop:1 rgba(255, 255, 255, 255));"
        "} "
        "QTableWidget::item { "
        "padding: 5px; "
        "text-align: center; "
        "border: 1px solid #b5d8ff; "
        "border-radius: 5px; "
        "font: 18pt 'Simplified Arabic Fixed'; "
        "font-weight: bold;"
        "} "
        "QHeaderView::section { "
        "background-color: rgb(167, 255, 192); "
        "color: black; "
        "padding: 4px; "
        "border: 1px solid #d0d0d0; "
        "border-top-left-radius: 5px; "
        "border-top-right-radius: 5px; "
        "font: 12pt 'Courgette'; "
        "} "
        "QTableWidget::item:selected { "
        "background-color: rgb(167, 255, 192); "
        "color: white; "
        "}"
    );




    ui->memoryTable->setHorizontalHeaderLabels(headers);
    ui->memoryTable->verticalHeader()->setVisible(false); // Hide the vertical hea

    for (size_t i = 0; i < 0xFFFF; ++i)
    {

        QTableWidgetItem *addressItem = new QTableWidgetItem(QString("0x%1").arg(i, 4, 16, QChar('0')).toUpper());
        addressItem->setFlags(addressItem->flags() & ~Qt::ItemIsEditable);
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString("0x%1").arg(memory.read(i), 4, 16, QChar('0')).toUpper());
        //valueItem->setData(Qt::ForegroundRole, QColor(Qt::magenta));
        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);

        ui->memoryTable->setItem(i, 0, addressItem);
        ui->memoryTable->setItem(i, 1, valueItem);
    }
}


