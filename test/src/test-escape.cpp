////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-escape.cpp
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
/// @brief      Mustache test suite (test sections).
///
////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;

#include "../../catch.hpp"

#include "../../src/mustache-light.hpp"
using mustache::Mustache;

TEST_CASE("Escape") {
    Mustache m("./test/fixtures/");

    SECTION("Escape HTML tags") {
        string html = "<p>&lt;div&gt;to be escaped&lt;/div&gt;</p>";
        string view = "<p>{{name}}</p>";
        string context = "{ \"name\": \"<div>to be escaped</div>\" }";
        string res = m.render(view, context);
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Escape harmful chars") {
        string html = "<p>&quot;Tom&quot; &amp; &quot;Jerry&quot; - &apos;The new adventures&apos;</p>";
        string view = "<p>{{name}}</p>";
        json context;
        context["name"] = "\"Tom\" & \"Jerry\" - 'The new adventures'";
        string res = m.render(view, context);
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Escape symbols") {
        string html = "<p>100&percnt;</p>";
        string view = "<p>{{name}}</p>";
        string context = "{ \"name\": \"100%\" }";
        string res = m.render(view, context);
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
