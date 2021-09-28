/*
  Describe the mRISCV registers

  sourced from: https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf
*/

let Namespace = "mRISCV" in {
class mRISCVReg<bits<5> Enc, string n, list<string> alt = []> : Register<n> {
  let HWEncoding{4-0} = Enc;
  let AltNames = alt;
}
} // end Namespace

// zero register
def X0  : mRISCVReg<0, "x0", ["zero"]>, DwarfRegNum<[0]>;
// return address
def X1  : mRISCVReg<1, "x1", ["ra"]>, DwarfRegNum<[1]>;
// stack pointer
def X2  : mRISCVReg<2, "x2", ["sp"]>, DwarfRegNum<[2]>;
// global pointer
def X3  : mRISCVReg<3, "x3", ["gp"]>, DwarfRegNum<[3]>;
// thread pointer
def X4  : mRISCVReg<4, "x4", ["tp"]>, DwarfRegNum<[4]>;
// Temporary Registers
def X5  : mRISCVReg<5, "x5", ["t0"]>, DwarfRegNum<[5]>;
def X6  : mRISCVReg<6, "x6", ["t1"]>, DwarfRegNum<[6]>;
def X7  : mRISCVReg<7, "x7", ["t2"]>, DwarfRegNum<[7]>;
// saved registers
def X8  : mRISCVReg<8, "x8", ["s0", "fp"]>, DwarfRegNum<[8]>;
def X9  : mRISCVReg<9, "x9", ["s1"]>, DwarfRegNum<[9]>;
// function arguments
def X10 : mRISCVReg<10,"x10", ["a0"]>, DwarfRegNum<[10]>;
def X11 : mRISCVReg<11,"x11", ["a1"]>, DwarfRegNum<[11]>;
def X12 : mRISCVReg<12,"x12", ["a2"]>, DwarfRegNum<[12]>;
def X13 : mRISCVReg<13,"x13", ["a3"]>, DwarfRegNum<[13]>;
def X14 : mRISCVReg<14,"x14", ["a4"]>, DwarfRegNum<[14]>;
def X15 : mRISCVReg<15,"x15", ["a5"]>, DwarfRegNum<[15]>;
def X16 : mRISCVReg<16,"x16", ["a6"]>, DwarfRegNum<[16]>;
def X17 : mRISCVReg<17,"x17", ["a7"]>, DwarfRegNum<[17]>;
// saved registers continued
def X18 : mRISCVReg<18,"x18", ["s2"]>, DwarfRegNum<[18]>;
def X19 : mRISCVReg<19,"x19", ["s3"]>, DwarfRegNum<[19]>;
def X20 : mRISCVReg<20,"x20", ["s4"]>, DwarfRegNum<[20]>;
def X21 : mRISCVReg<21,"x21", ["s5"]>, DwarfRegNum<[21]>;
def X22 : mRISCVReg<22,"x22", ["s6"]>, DwarfRegNum<[22]>;
def X23 : mRISCVReg<23,"x23", ["s7"]>, DwarfRegNum<[23]>;
def X24 : mRISCVReg<24,"x24", ["s8"]>, DwarfRegNum<[24]>;
def X25 : mRISCVReg<25,"x25", ["s9"]>, DwarfRegNum<[25]>;
def X26 : mRISCVReg<26,"x26", ["s10"]>, DwarfRegNum<[26]>;
def X27 : mRISCVReg<27,"x27", ["s11"]>, DwarfRegNum<[27]>;
// temp registers
def X28 : mRISCVReg<28,"x28", ["t3"]>, DwarfRegNum<[28]>;
def X29 : mRISCVReg<29,"x29", ["t4"]>, DwarfRegNum<[29]>;
def X30 : mRISCVReg<30,"x30", ["t5"]>, DwarfRegNum<[30]>;
def X31 : mRISCVReg<31,"x31", ["t6"]>, DwarfRegNum<[31]>;

// The order of registers represents the preferred allocation sequence.
// Registers are listed in the order caller-save, callee-save, specials.
def GPR : RegisterClass<"mRISCV", [i32], 32, (add
    (sequence "X%u", 10, 17),
    (sequence "X%u", 5, 7),
    (sequence "X%u", 28, 31),
    (sequence "X%u", 8, 9),
    (sequence "X%u", 18, 27),
    (sequence "X%u", 0, 4)
  )>;

def SP : RegisterClass<"mRISCV", [i32], 32, (add X2)>;