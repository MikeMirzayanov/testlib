#include "testlib.h"

using namespace std;

/* Byte 0xFF is data, not EOF: in file streams, in string streams and in isEof(curChar()). */

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);

    const char FF = char(255);

    // String stream: "x<FF>y z".
    string content = "x";
    content += FF;
    content += "y z";
    InStream s(inf, content);

    ensuref(!s.eof(), "string stream: eof() before reading");
    ensuref(!isEof(s.curChar()), "string stream: isEof(curChar()) before reading");

    string a = s.readToken();
    ensuref(a.size() == 3, "string stream: first token has %d bytes, expected 3", int(a.size()));
    ensuref(a[1] == FF, "string stream: byte 0xFF is lost in the first token");
    ensuref(!s.seekEof(), "string stream: seekEof() between tokens");

    string b = s.readToken();
    ensuref(b == "z", "string stream: second token is '%s', expected 'z'", b.c_str());
    ensuref(s.seekEof(), "string stream: seekEof() at the end");
    ensuref(isEof(s.curChar()), "string stream: isEof(curChar()) at the end");

    // String stream of a single 0xFF byte is not empty.
    InStream f(inf, string(1, FF));
    ensuref(!f.eof(), "string stream: eof() on a single 0xFF byte");
    ensuref(f.readToken() == string(1, FF), "string stream: a single 0xFF byte is not a token");
    ensuref(f.eof(), "string stream: eof() after a single 0xFF byte");

    // File streams: the files contain "a<FF>b".
    string j = ans.readToken();
    string p = ouf.readToken();
    ensuref(j.size() == 3 && j[1] == FF, "answer: token has %d bytes, expected 3", int(j.size()));
    ensuref(p.size() == 3 && p[1] == FF, "output: token has %d bytes, expected 3", int(p.size()));
    ensuref(j == p, "tokens differ");
    ensuref(ouf.seekEof(), "output: seekEof() at the end");
    ensuref(isEof(ouf.curChar()), "output: isEof(curChar()) at the end");

    quitf(_ok, "byte 0xFF is kept in %d-byte tokens", int(p.size()));
}
