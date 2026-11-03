TEST(appes_mode_encoding) {
    ensure(appesModeEncoding == "windows-1251");

    setAppesModeEncoding("UTF-8");
    ensure(appesModeEncoding == "utf-8");

    setAppesModeEncoding("koi8-r");
    ensure(appesModeEncoding == "koi8-r");

    // A rejected encoding fails and leaves the current value unchanged.
    ensure_exit(3, [](){setAppesModeEncoding("Bad-Enc");});
    ensure(appesModeEncoding == "koi8-r");

    setAppesModeEncoding("windows-1251");
    ensure(appesModeEncoding == "windows-1251");
}
