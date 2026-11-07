// Compile-only probe: the signatures and default arguments of the functions whose parameters
// were renamed to avoid shadowing stay the same.
#include "testlib.h"

int main() {
    bool (pattern::*matches1)(const std::string &) const = &pattern::matches;
    void (InStream::*init1)(std::string, TMode) = &InStream::init;
    void (InStream::*init2)(std::FILE *, TMode) = &InStream::init;
    void (*setter)(std::string) = &setAppesModeEncoding;
    (void)matches1;
    (void)init1;
    (void)init2;
    (void)setter;

    pattern p("[a-z]+");
    (void)p;

    ValidatorBoundsHit h0;
    ValidatorBoundsHit h1(true);
    ValidatorBoundsHit h2(true, false);
    bool hits = h0.minHit || h0.maxHit || h1.minHit || h2.maxHit;
    (void)hits;

    if (false) {
        FileInputStreamReader file(stdin, "stdin");
        BufferedFileInputStreamReader buffered(stdin, "stdin");
        (void)file.getName();
        (void)buffered.getName();
    }
    return 0;
}
