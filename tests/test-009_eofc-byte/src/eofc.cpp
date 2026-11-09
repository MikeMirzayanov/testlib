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

    char s1 = s.nextChar();
    char s2 = s.nextChar();
    ensuref(s1 == 'x' && s2 == FF, "string stream: unexpected initial bytes");
    s.unreadChar(s2);
    ensuref(!s.eof() && s.reader->curChar() == 255, "string stream: unreadChar(0xFF) failed");
    s.unreadChar(s1);
    ensuref(!s.eof() && s.curChar() == 'x', "string stream: unreadChar('x') failed");

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
    char o1 = ouf.nextChar();
    char o2 = ouf.nextChar();
    ensuref(o1 == 'a' && o2 == FF, "output: unexpected initial bytes");
    ouf.unreadChar(o2);
    ensuref(!ouf.eof() && ouf.reader->curChar() == 255, "output: unreadChar(0xFF) turned byte into EOF");
    ouf.unreadChar(o1);
    ensuref(!ouf.eof() && ouf.curChar() == 'a', "output: unreadChar('a') failed");

    string j = ans.readToken();
    string p = ouf.readToken();
    ensuref(j.size() == 3 && j[1] == FF, "answer: token has %d bytes, expected 3", int(j.size()));
    ensuref(p.size() == 3 && p[1] == FF, "output: token has %d bytes, expected 3", int(p.size()));
    ensuref(j == p, "tokens differ");
    ensuref(ouf.seekEof(), "output: seekEof() at the end");
    ensuref(isEof(ouf.curChar()), "output: isEof(curChar()) at the end");

    // Standard input stream (FileInputStreamReader): stdin receives files/output.01 ("a<FF>b").
    InStream in;
    in.init(stdin, _input);
    ensuref(in.readChar() == 'a', "stdin: expected 'a'");
    char inFF = in.nextChar();
    ensuref(inFF == FF, "stdin: expected byte 0xFF");
    in.unreadChar(inFF);
    ensuref(!in.eof() && in.reader->curChar() == 255, "stdin: unreadChar(0xFF) turned byte into EOF");
    string inRest = in.readToken();
    ensuref(inRest.size() == 2 && inRest[0] == FF && inRest[1] == 'b',
            "stdin: token after unreadChar(0xFF) has %d bytes, expected 2", int(inRest.size()));
    ensuref(in.seekEof(), "stdin: seekEof() at the end");
    ensuref(isEof(in.curChar()), "stdin: isEof(curChar()) at the end");

    quitf(_ok, "byte 0xFF is kept in %d-byte tokens", int(p.size()));
}
