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
#include <map>
#include <vector>

#include <mustache-light.hpp>
using mustache::Mustache;

// -----------------------------------------------------------------------------

void run_test(const string& name, const string& view, const string& context) {
  const uint NUM_RUNS = 10;

  uint64_t tot = 0;
  cout << "Run " << std::left << std::setw(25) << name  << std::flush;

  Mustache m("./benchmark/fixtures/");
  string rendered;

  for (uint run = 0; run < NUM_RUNS; run++) {
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    rendered = m.render(m.fileRead(view), m.fileRead(context, "json"));
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    const string &error = m.error();
    if (error.size() > 0) {
      cout << "Rendering error:" << endl << error << endl;
      return;
    }

    uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    tot += duration;
    if (run % 100 == 0) {
      cout << "." << std::flush;
    }
  }

  cout << " [ " << std::right << std::setw(10) << tot / NUM_RUNS << " μs ]" << endl;
}

int main() {
  cout << "Mustache Benchmark Test: " << endl << endl;

  const std::map<std::string, std::array<std::string, 2>> TESTS = {
      {"Nested Views", {"nested", "nested"}},
      {"Small View Big Context", {"big-context", "big-context"}},
      {"Big View Big Context", {"big-view", "big-context"}}
  };

  for (const auto &pair : TESTS) {
    const std::string &key = pair.first;
    const std::array<std::string, 2> &arr = pair.second;

    const string& view = arr.at(0);
    const string& context = arr.at(1);
    run_test(key, view, context);
  }
}

////////////////////////////////////////////////////////////////////////////////
