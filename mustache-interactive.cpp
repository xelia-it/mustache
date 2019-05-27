////////////////////////////////////////////////////////////////////////////////
///
/// @file       mustache-light.hpp
/// @author     Xelia snc <info@xelia.it>
/// @copyright  The code is licensed under the MIT License.
///
///             <http://opensource.org/licenses/MIT>:
///
///             Copyright (c) Xelia snc
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
/// @brief      Mustache template parser.
///
////////////////////////////////////////////////////////////////////////////////

#include "./src/mustache-light.hpp"

#include <string>
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

using namespace mustache;
// for convenience
using json = nlohmann::json;

// -----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    string view = "basic/empty";
    string context = "basic/empty";

    if (argc == 1) {
        // OK
    } else if (argc == 2) {
        view = argv[1];
    } else if (argc == 3) {
        view = argv[1];
        context = argv[2];
    } else {
        std::cerr << "Usage: test-mustache view context" << endl;
    }

    cout << "Open view: " << view << endl;
    cout << "Open context file: " << context << endl;

    Mustache m("./test/fixtures/");
    string rendered = m.render(m.fileRead(view), m.fileRead(context, "json"));

    cout << "Rendered:" << endl
            << "------------------------------------------------------" << endl
            << rendered << endl
            << "------------------------------------------------------" << endl;
    string error = m.error();
    if (error.size() > 0) {
        cout << "Error:" << endl
                << error << endl
                << "------------------------------------------------------" << endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
