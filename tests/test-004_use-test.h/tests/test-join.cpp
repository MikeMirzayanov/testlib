TEST(join) {
    ensure(join(vector<long long>({LLONG_MIN, 2, LLONG_MAX})) == "-9223372036854775808 2 9223372036854775807");
    ensure(join(vector<long long>({LLONG_MIN, 2, LLONG_MAX}), ',') == "-9223372036854775808,2,9223372036854775807");
    ensure(join(vector<int>()) == "");
    ensure(join(vector<string>({"ok"})) == "ok");
    ensure(join(vector<string>({"ok", "ok", "ok", "ok"})) == "ok ok ok ok");
    ensure(join(vector<string>({"ok", "test"})) == "ok test");
}
