#include <stdio.h>
#include <stdlib.h> 


//this branche i will comment the code and sclarify it 
using Byte = unsigned char ; 

using Word = unsigned short ;

using u32 = unsigned int ; 


struct Memory 
{ 
    static constexpr u32 MAX_MEM = 1024*64 ;
    Byte Data[MAX_MEM] ;

    void initialze() 
    {

    }

    Byte operator[](u32 adress) const 
        {
            // assert here adress is < Max_Mem 
            return Data[adress] ; 

        }
};

struct Cpu
{
    Word Program_counter ; 
    Word Stack_pointer ; 
    Byte  Accumulator_register, Index_register_x, Index_register_y ; 

    Byte Carry_flag : 1; 
    Byte Zero_flag : 1; 
    Byte Interrupt_flag : 1; 
    Byte Interrupt_disable : 1;
    Byte Decimal_Mode : 1; 
    Byte Break_command : 1;
    Byte Overflow_flag ; 
    Byte Negative_flag ; 

    void Reset(Memory& mem)
    {
        Program_counter = 0xFFFC ; 
        Stack_pointer = 0X0100 ; 
        // all the flags to 0 
        Decimal_Mode = 0 ;

        // registers to 0

        mem.initialze() ;
    }
    
    Byte Fetch_Bytes (u32& Cycles, Memory& mem)
    {
        Byte Data = mem [Program_counter] ;
        Program_counter ++ ; 
        Cycles -- ; 
        return Data ; 
    }

    // opcodes 
    static constexpr Byte 
    load_accumulator_immediate = 0xA9 ;

    void Execute (u32 Cycles, Memory& mem)
    {

        while(Cycles > 0 )
        {
            Byte Instruction  = Fetch_Bytes(Cycles, mem) ;
            switch (Instruction) 
            {
                case load_accumulator_immediate:
                {
                    Byte value = Fetch_Bytes(Cycles, mem) ;
                    Accumulator_register = value ;
                    Zero_flag = (Accumulator_register == 0) ; 
                    Negative_flag = (Accumulator_register & 0b1000000) ;
                } break;
                // default: 
                // {
                //     printf("Instruction not handled %d " ,Instruction ) ; 
                // } break ; 
            }
        }
    }

    
    void Decode (Memory& mem)
    {

    }
};

int main ()
{
    Memory mem ; 
    Cpu cpu ;

    cpu.Reset(mem);
    cpu.Execute(2,mem);

 return 0 ; 
}