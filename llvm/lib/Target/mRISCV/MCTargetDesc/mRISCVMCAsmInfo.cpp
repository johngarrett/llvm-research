#include "mRISCVMCAsmInfo.h"

#include "llvm/ADT/Triple.h"

namespace llvm {

void mRISCVMCAsmInfo::anchor() {

}

mRISCVMCAsmInfo::mRISCVMCAsmInfo(const Triple &TheTriple) {
    IsLittleEndian = false;

    AlignmentIsInBytes          = false;
    Data16bitsDirective         = "\t.hword\t";
    Data32bitsDirective         = "\t.word\t";
    Data64bitsDirective         = "\t.dword\t";

    PrivateGlobalPrefix         = ".L";
    PrivateLabelPrefix          = ".L";

    LabelSuffix                 = ":";

    CommentString               = ";";

    ZeroDirective               = "\t.zero\t";

    UseAssignmentForEHBegin     = true;

    SupportsDebugInformation    = true;
    ExceptionsType              = ExceptionHandling::DwarfCFI;
    DwarfRegNumForCFI           = true;
}
} // end namespace llvm