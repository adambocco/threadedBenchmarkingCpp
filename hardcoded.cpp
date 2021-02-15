#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <ctime>


int main(int argc, char const *argv[])
{
    std::ofstream out;
    out.open("staticInput.cpp");
    out << "#include <string>\n" <<
            "#include <string.h>\n" <<
            "#include <iostream>\n" <<
            "#include <fstream>\n" <<
            "#include <chrono>\n" <<
            "#include <ctime>\n" <<
            "#include <ratio>\n" <<
            "#include <utility>\n\n" <<
            "int main(int argc, char const *argv[]) {\n" <<
            "\tauto start = std::chrono::steady_clock::now();" <<
            "\tdouble long x = 0;" <<
            "\tfor (double long i = 0; i < 100000; i++) {\n";
    for (int i = 0; i < 1e5; i++) {
        out << "\t\t x += 1L;\n";
    }
    out << "\t}";
    out << "\tauto end = std::chrono::steady_clock::now();\n";
    out << "\tstd::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;";
    out << "\tauto totalSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/1e9;\n";
    out << "\tdouble long gigaflops = (10000000000L/totalSeconds/1e9);\n";
    out << "\tstd::cout << std::fixed << gigaflops << std::endl;\n";
    out << "\treturn 0;\n}";
    out.close();
    return 0;
}


