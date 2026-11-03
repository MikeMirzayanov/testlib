#include "testlib.h"

using namespace std;

/*
 * readLine(): LF and CRLF end a line, a CR not followed by LF is skipped,
 * EOF ends the last line without adding any byte to it.
 */

static void check(InStream &in, const string &expected, const char *what) {
    string line = in.readLine();
    if (line != expected)
        quitf(_fail, "%s: expected %d bytes '%s', found %d bytes '%s'", what,
              int(expected.size()), expected.c_str(), int(line.size()), line.c_str());
    if (!in.eof())
        quitf(_fail, "%s: not at EOF after the only line", what);
}

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);

    // String streams.
    InStream a(inf, "abc\r");
    check(a, "abc", "string 'abc<CR>'");
    InStream b(inf, "abc\rdef\n");
    check(b, "abcdef", "string 'abc<CR>def<LF>'");
    InStream c(inf, "abc\r\n");
    check(c, "abc", "string 'abc<CR><LF>'");
    InStream d(inf, "\r");
    check(d, "", "string '<CR>'");

    // Files: output lines must equal answer lines byte for byte.
    int lines = 0;
    while (!ans.eof()) {
        string j = ans.readLine();
        string p = ouf.readLine();
        lines++;
        if (j != p)
            quitf(_wa, "line %d: expected %d bytes '%s', found %d bytes '%s'", lines,
                  int(j.size()), j.c_str(), int(p.size()), p.c_str());
    }
    if (!ouf.eof())
        quitf(_wa, "output has extra data after %d lines", lines);

    quitf(_ok, "%d lines", lines);
}
