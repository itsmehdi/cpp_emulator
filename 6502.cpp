#include <stdio.h>
#include <stdlib.h>

// Define data size that will be exchanged beetween the registers
using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

/*
 * Mmemory struct define the memory location
 *
 */
struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void initialze()
    {
    }

    Byte operator[](u32 adress) const
    {
        // assert here adress is < Max_Mem
        return Data[adress];
    }
};

/**
 * @brief  Define the cpu struct with the defferent fonction that manipulate it.
 *
 */
struct Cpu
{
    Word Program_counter;
    Word Stack_pointer;
    Byte Accumulator_register, Index_register_x, Index_register_y;

    Byte Carry_flag : 1;
    Byte Zero_flag : 1;
    Byte Interrupt_flag : 1;
    Byte Interrupt_disable : 1;
    Byte Decimal_Mode : 1;
    Byte Break_command : 1;
    Byte Overflow_flag : 1 ;
    Byte Negative_flag : 1 ;

    /**
     * @brief
     * The reset is the first function called by the CPU it initialize the cpu registers and 
     * the memory (see boot sequence to do the right register operation)
     *
     * @param mem : te reset function initialize the memory
     */
    void Reset(Memory &mem)
    {
        Program_counter = 0xFFFC;
        Stack_pointer = 0X0100;
        // all the flags to 0
        Carry_flag = 0 ; 
        Zero_flag = 0 ; 
        Interrupt_flag = 0 ; 
        Interrupt_disable = 0 ; 
        Decimal_Mode = 0 ; 
        Break_command = 0 ; 
        Overflow_flag = 0 ;
        Negative_flag = 0 ; 
        
        // registers to 0
        mem.initialze();
    }

    /**
     * @brief Fetch consist of getting an instruction from memory.
     *
     * @param Cycles: Number of cycles to do the fetch
     * @param mem: The memory.
     * @return Byte
     */
    Byte Fetch_Bytes(u32 &Cycles, Memory &mem)
    {
        Byte Data = mem[Program_counter];
        Program_counter++;
        Cycles--;
        return Data;
    }

    // opcodes
    static constexpr Byte
        load_accumulator_immediate = 0xA9;

    /**
     * @brief The execute operation consist of executing the instructions fetched from the memory.
     *
     * @param Cycles
     * @param mem
     */
    void Execute(u32 Cycles, Memory &mem)
    {

        while (Cycles > 0)
        {
            Byte Instruction = Fetch_Bytes(Cycles, mem);
            switch (Instruction)
            {
            case load_accumulator_immediate:
            {
                Byte value = Fetch_Bytes(Cycles, mem);
                Accumulator_register = value;
                Zero_flag = (Accumulator_register == 0);
                Negative_flag = (Accumulator_register & 0b1000000);
            }
            break;
            default:
            {
                printf("Instruction not handled %d ", Instruction);
            }
            break;
            }
        }
    }

    /**
     * @brief
     *
     * @param mem
     */
    void Decode(Memory &mem)
    {
    }
};

int main()
{
    Memory mem;
    Cpu cpu;

    cpu.Reset(mem);
    cpu.Execute(2, mem);

    return 0;
}