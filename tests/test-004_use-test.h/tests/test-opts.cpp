TEST(opts) {
    {
        const char* args[] = {"test"};
        prepareOpts(sizeof(args) / sizeof(const char*), (char **)args);
        ensure(!has_opt("a"));
        ensure_exit(3, [](){ opt<int>(1);});
    }
    {
        const char* args[] = {"test", "-a", "-2147483648", "-f1", "0", "-f2", "1", "-f3", "true", "-f4", "false", "--test=me=me", "--d=-17.125", "--ex=3e6"};
        prepareOpts(sizeof(args) / sizeof(const char*), (char **)args);
        ensure(has_opt("a"));
        ensure(has_opt("f1"));
        ensure(has_opt("f2"));
        ensure(has_opt("f3"));
        ensure(has_opt("f4"));
        ensure(has_opt("test"));
        ensure(has_opt("d"));
        ensure(has_opt("ex"));
        ensure(!has_opt("f"));
        ensure(!has_opt("f5"));
        ensure(!has_opt("true"));
        ensure(!has_opt("0"));
        ensure(!has_opt("1"));
        ensure(!has_opt("-a"));
        ensure(!has_opt("--test"));
        ensure(opt<int>("a") == INT_MIN);
        ensure(!opt<bool>("f1"));
        ensure(opt<bool>("f2"));
        ensure(opt<bool>("f3"));
        ensure(!opt<bool>("f4"));
        ensure(opt<string>("test") == "me=me");
        ensure(opt<double>("d") == -17.125);
        ensure(opt<int>("ex") == 3000000);
        ensure(opt<long long>("ex") == 3000000);
        ensure_exit(3, [](){opt<short>("ex");});
        ensure(opt<int>(2) == INT_MIN);
        ensure(opt<string>(13) == "--ex=3e6");
        ensure_exit(3, [](){ opt<string>(-1);});
        ensure("test" == opt<string>(0));
        ensure_exit(3, [](){ opt<bool>(2);});
        ensure_exit(3, [](){ opt<int>(3);});
        ensure_exit(3, [](){ opt<int>(14);});
        ensure_exit(3, [](){ opt<unsigned long long>("a");});
    }
}
