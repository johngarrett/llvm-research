//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that mRISCV uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#include "mRISCVISelLowering.h"
#include "mRISCVSubtarget.h"
#include "mRISCVTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/Debug.h"
#include <cassert>

using namespace llvm;

#define DEBUG_TYPE "mRISCV-lower"

#include "mRISCVGenCallingConv.inc"

mRISCVTargetLowering::mRISCVTargetLowering(const TargetMachine &TM,
                                           const mRISCVSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {
  // Set up the register classes
  addRegisterClass(MVT::i32, &mRISCV::GPRRegClass);

  // compute all the derrived properties exposed by the register classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set scheduling preference
  // Availiable options: { None, Source, RegPressure, Hybrid, ILP, VLIW }
  // Setting this to source as we aren't doing anything special yet with
  // compilation
  setSchedulingPreference(Sched::Source);

  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  setOperationAction(ISD::BlockAddress, MVT::i32, Custom);
  setOperationAction(ISD::ConstantPool, MVT::i32, Custom);

  // Set minimum and preferred function alignment (log2)
  setMinFunctionAlignment(Align(1));
  setPrefFunctionAlignment(Align(1));

  // Set preferred loop alignment (log2)
  setPrefLoopAlignment(Align(1));

  // adding in for help
  setStackPointerRegisterToSaveRestore(mRISCV::X2);

  setBooleanContents(ZeroOrOneBooleanContent);
  setBooleanVectorContents(ZeroOrOneBooleanContent);
}

// for all the aliases/pseudoops defined in our register file
const char *mRISCVTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((mRISCVISD::NodeType)Opcode) {
  case mRISCVISD::Ret:
    return "mRISCVISD::Ret";
  default:
    return nullptr;
  }
}

void mRISCVTargetLowering::ReplaceNodeResults(SDNode *N,
                                              SmallVectorImpl<SDValue> &Results,
                                              SelectionDAG &DAG) const {
  /*
    example from RISCV source:
      case ISD::ADD:
        if the value is i32 and subtarget is 64 bit, there's an error
        if it's a constant, return
        else, legalize with sign extension
  */
  switch (N->getOpcode()) {
  default:
    llvm_unreachable("Don't know how to custom expand this!");
  }
}

/// FORMAL ARGUMENTS CONVENTION

// X0 - X3 are the arguments
static const MCPhysReg GPRArgRegs[] = {mRISCV::X0, mRISCV::X1, mRISCV::X2,
                                       mRISCV::X3};

// Transform physical registers into virtual registers.
SDValue mRISCVTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  switch (CallConv) {
  default:
    report_fatal_error("Unsupported calling convention");
  case CallingConv::C:
  case CallingConv::Fast:
    break;
  }

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, mRISCV_CCallingConv);

  SmallVector<SDValue, 16> ArgValues;
  SDValue ArgValue;
  Function::const_arg_iterator CurOrigArg = MF.getFunction().arg_begin();
  unsigned CurArgIdx = 0;

  // calculate how much space we need on the stack for the function arguments
  // being passed in by registers
  unsigned ArgRegBegin = mRISCV::X4; // TODO: check this -- should be X12?
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    // no more registers to check
    if (CCInfo.getInRegsParamsProcessed() >= CCInfo.getInRegsParamsCount())
      break;

    CCValAssign &VA = ArgLocs[i];
    unsigned Index = VA.getValNo();
    ISD::ArgFlagsTy Flags = Ins[Index].Flags;
    if (!Flags.isByVal()) // not by val, we don't need to allocate more space
      continue;

    assert(VA.isMemLoc() && "unexpected byval pointer in reg");
    unsigned RBegin, REnd;
    CCInfo.getInRegsParamInfo(CCInfo.getInRegsParamsProcessed(), RBegin, REnd);
    ArgRegBegin = std::min(ArgRegBegin, RBegin);

    CCInfo.nextInRegsParam();
  }
  CCInfo.rewindByValRegsInfo();

  int lastInsIndex = -1;
  // first unallocated general purpose register arg reg
  unsigned Idx = CCInfo.getFirstUnallocated(GPRArgRegs);
  if (isVarArg && MFI.hasVAStart()) {
    // GPRArgRegs array doesnt contain all allocated GPRs 
    if (Idx != array_lengthof(GPRArgRegs))
      ArgRegBegin = std::min(ArgRegBegin, (unsigned)GPRArgRegs[Idx]);
  }

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    // skip ahead to the right index
    if (Ins[VA.getValNo()].isOrigArg()) {
      std::advance(CurOrigArg,
                   Ins[VA.getValNo()].getOrigArgIndex() - CurArgIdx);
      CurArgIdx = Ins[VA.getValNo()].getOrigArgIndex();
    }
    // Arguments stored in registers.
    if (VA.isRegLoc()) {
      EVT RegVT = VA.getLocVT();

      if (VA.needsCustom()) {
        llvm_unreachable("Custom val assignment not supported by "
                         "FORMAL_ARGUMENTS Lowering");
      } else {
        const TargetRegisterClass *RC;

        if (RegVT == MVT::i32)
          RC = &mRISCV::GPRRegClass;
        else
          llvm_unreachable("RegVT not supported by FORMAL_ARGUMENTS Lowering");

        // Transform the arguments in physical registers into virtual ones.
        unsigned Reg = MF.addLiveIn(VA.getLocReg(), RC);
        ArgValue = DAG.getCopyFromReg(Chain, dl, Reg, RegVT);
      }

      // truncate the i8 and i16 values we promoted to i32
      // (see sourced TableGen file)
      switch (VA.getLocInfo()) {
      default:
        llvm_unreachable("Unknown loc info!");
      // full size: don't worry about it
      case CCValAssign::Full: 
        break;
      // the value has been bit converted
      case CCValAssign::BCvt:
        ArgValue = DAG.getNode(ISD::BITCAST, dl, VA.getValVT(), ArgValue);
        break;
      // value has been sign extended
      case CCValAssign::SExt:
        ArgValue = DAG.getNode(ISD::AssertSext, dl, RegVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));
        ArgValue = DAG.getNode(ISD::TRUNCATE, dl, VA.getValVT(), ArgValue);
        break;
      // value has been zero extended
      case CCValAssign::ZExt:
        ArgValue = DAG.getNode(ISD::AssertZext, dl, RegVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));
        ArgValue = DAG.getNode(ISD::TRUNCATE, dl, VA.getValVT(), ArgValue);
        break;
      }

      InVals.push_back(ArgValue);
    } else { 
      int index = VA.getValNo();

      if (index != lastInsIndex) {
        llvm_unreachable("Cannot retrieve arguments from the stack");
      }
    }
  }

  return Chain;
}

/// RETURN VALUE CALLING CONVENTION
/*
  Determine _if_ we can lower the return
*/
bool mRISCVTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, MF, RVLocs, Context);
  // default calling convention logic to check returns
  return CCInfo.CheckReturn(Outs, mRISCV_CRetConv);
}


/*
  Put the return values from a function into vectors
*/
SDValue
mRISCVTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                  bool isVarArg,
                                  const SmallVectorImpl<ISD::OutputArg> &Outs,
                                  const SmallVectorImpl<SDValue> &OutVals,
                                  const SDLoc &dl, SelectionDAG &DAG) const {
  // Stores the assignment of the return value to a location.
  SmallVector<CCValAssign, 16> RVLocs;

  // Info about the registers and stack slots.
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  // Analyze outgoing return values.
  CCInfo.AnalyzeReturn(Outs, mRISCV_CRetConv);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain); // stack slot size is 4

  // Copy the result values into the output registers.
  for (unsigned i = 0, realRVLocIdx = 0; i != RVLocs.size(); ++i, ++realRVLocIdx) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Expected return via registers");

    SDValue Arg = OutVals[realRVLocIdx];
    bool ReturnF16 = false;

    // breakout of the function convertValVTToLocVT
    switch (VA.getLocInfo()) {
    default:
      llvm_unreachable("Unexpected CCValAssign::LocInfo");
    case CCValAssign::Full:
      break;
    case CCValAssign::BCvt:
      if (!ReturnF16)
        Arg = DAG.getNode(ISD::BITCAST, dl, VA.getLocVT(), Arg);
      break;
    }

    if (VA.needsCustom()) {
      llvm_unreachable("Custom val assignment not supported by RETURN Lowering");
    } else {
      Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), Arg, Glue);
    }

    // Guarantee that all emitted copies are stuck together
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  // Update chain and glue
  RetOps[0] = Chain;
  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(mRISCVISD::Ret, dl, MVT::Other, RetOps);
}

/// LOWER OPERATION IMPL

SDValue mRISCVTargetLowering::LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("[mRISCVISelLowering]: Unsupported global address");
}

SDValue mRISCVTargetLowering::LowerBlockAddress(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("[mRISCVISelLowering]: Unsupported block address");
}

SDValue mRISCVTargetLowering::LowerConstantPool(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("[mRISCVISelLowering]: Unsupported constant pool");
}

SDValue mRISCVTargetLowering::LowerRETURNADDR(SDValue Op, SelectionDAG &DAG) const {
  // assume verifyReturnAddressArgumentIsConstant
  return SDValue();
}

SDValue mRISCVTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operand");
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  case ISD::BlockAddress:
    return LowerBlockAddress(Op, DAG);
  case ISD::ConstantPool:
    return LowerConstantPool(Op, DAG);
  case ISD::RETURNADDR:
    return LowerRETURNADDR(Op, DAG);
  }
}