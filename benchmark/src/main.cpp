////////////////////////////////////////////////////////////////////////////////
///
/// @file       main.hpp
/// @author     Xelia snc <info@xelia.it>
/// @copyright  The code is licensed under the MIT License.
///
///             <http://opensource.org/licenses/MIT>:
///
///             Copyright (c) 2015 Xelia snc
///
///             Permission is hereby granted, free of charge, to any person
///             obtaining a copy of this software and associated documentation
///             files (the "Software"), to deal in the Software without
///             restriction, including without limitation the rights to use,
///             copy, modify, merge, publish, distribute, sublicense, and/or
///             sell copies of the Software, and to permit persons to whom
///             the Software is furnished to do so, subject to the following
///             conditions:
///
///             The above copyright notice and this permission notice shall be
///             included in all copies or substantial portions of the Software.
///
///             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
///             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
///             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
///             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
///             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
///             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
///             ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
///             THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///
/// @brief      Mustache benchmark test.
///
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <chrono>

#include <mustache-light.hpp>
using mustache::Mustache;

// -----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    const uint NUM_RUNS = 20000;

    string view_name = "nested";
    string context_name = "nested";

    if (argc == 1) {
        // OK
    } else if (argc == 2) {
        view_name = argv[1];
    } else if (argc == 3) {
        view_name = argv[1];
        context_name = argv[2];
    } else {
        std::cerr << "Usage: " << argv[0] << " view context" << std::endl;
    }

    Mustache m("./benchmark/fixtures/");
    string rendered;

    cout << "Open view: " << view_name << endl;
    const string& view = m.fileRead(view_name, "mustache");
    cout << "Open context file: " << context_name << endl;
    const string& context = m.fileRead(context_name, "json");

    uint64_t tot = 0;
    uint64_t tot_squared = 0;
    cout << "Run " << std::flush;
    for (uint run = 0; run < NUM_RUNS; run++) {
        // Evaluate the time for rendering a content
        std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
        rendered = m.render(view, context);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        const string& error = m.error();
        if (error.size() > 0) {
            cout << "Rendering error:" << endl << error << endl;
            return 1;
        }

        uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        tot += duration;
        tot_squared += duration * duration;
        if (run % 100 == 0) {
            cout << "." << std::flush;
        }
    }

    uint64_t mean = tot / NUM_RUNS;
    uint64_t variance = (tot_squared / NUM_RUNS) - (mean * mean);
    uint64_t stddev = static_cast<uint64_t>(std::sqrt(static_cast<double>(variance)));

    cout << endl;
    cout << "Results:" << endl;
    cout << "  Mean = " << mean << " us" << endl;
    cout << "  Standard deviation = " << stddev << " us" << endl;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
