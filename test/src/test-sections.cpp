////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-sections.cpp
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
/// @brief      Mustache test suite (test sections).
///
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"

#include <string>
#include <mustache-light.hpp>

using std::string;
using mustache::Mustache;

TEST_CASE("Sections") {
    Mustache m("./test/fixtures/");

    SECTION("Section with data") {
        string html = "<div>\n"
                "<p></p>\n"
                "\n"
                "<p>Data 3</p>\n"
                "</div>";
        string res = m.renderFilenames("sections/sections-with-data",
                "sections/sections-with-data");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with list") {
        string html = "<ul>\n"
                "<li>someone@somewhere.com</li>"
                "<li>another.one@somewhere.com</li>\n"
                "</ul>";
        string res = m.renderFilenames("sections/list", "sections/list");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with list and indexes") {
        string html = "<ul>\n"
                "<li class=\"active\">0 - someone@somewhere.com</li>"
                "<li>1 - another.one@somewhere.com</li>"
                "<li>2 - a.third.person@somewhere.com</li>\n"
                "</ul>";
        string res = m.renderFilenames("sections/list-with-indexes", "sections/list-with-indexes");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
