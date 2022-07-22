TEST(split) {
    ensure(split(" ab  b rrrr", ' ') == vector<string>({"", "ab", "", "b", "rrrr"}));
    ensure(split(" ab  b rrrr ", ' ') == vector<string>({"", "ab", "", "b", "rrrr", ""}));
    ensure(split(" ab  b rrrr  ", ' ') == vector<string>({"", "ab", "", "b", "rrrr", "", ""}));
    ensure(split("", ',') == vector<string>(1, ""));
}
