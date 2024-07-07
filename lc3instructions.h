#ifndef LC3INSTRUCTIONS_H
#define LC3INSTRUCTIONS_H

#include "lc3registers.h"
#include "lc3memory.h"

extern uint16_t ir;
extern uint16_t nzp;
extern uint16_t dr;
extern uint16_t sr1;
extern uint16_t sr2;
extern uint16_t v_sr1;
extern uint16_t v_sr2;
extern uint16_t imm_flag;
extern uint16_t imm5;
extern int16_t offset9;
extern int16_t offset6;
extern int16_t offset11;
extern uint16_t base_r;
extern uint16_t flag;
extern uint16_t opcode;
extern uint16_t address;
extern uint16_t GateALU;
extern uint16_t value;
extern uint16_t sr;
extern uint16_t R;
class LC3Instructions
{
public:
    static void fetch( LC3Memory& memory);
    static void decode();
    static void BR_decode();
    static void ADD_decode();
    static void LD_decode();
    static void AND_decode();
    static void NOT_decode();
    static void LDI_decode();
    static void LDR_decode();
    static void LEA_decode();
    static void ST_decode();
    static void STI_decode();
    static void STR_decode();
    static void JSR_decode();

    static void evaluateAddress( LC3Memory& memory);
    static void fetchOperands(LC3Memory& memory);
    static void execute();
    static void ADD_execute();
    static void AND_execute();
    static void store(LC3Memory &memory);
    static bool IsHalt();


};

#endif // LC3INSTRUCTIONS_H
