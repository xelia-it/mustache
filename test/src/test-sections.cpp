////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-sections.cpp
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
                "</ul>\n";
        string res = m.renderFilenames("sections/list", "sections/list");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with special variables") {
        string html = "<ul>\n"
                "<li class=\"active\">0 - someone@somewhere.com</li>"
                "<li>1 - another.one@somewhere.com</li>"
                "<li>2 - a.third.person@somewhere.com</li>\n"
                "</ul>";
        string res = m.renderFilenames("sections/list-special-variables",
            "sections/list-special-variables");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with indexes") {
        string html = "<ul>\n"
                "<li>person_at_position_1@somewhere.com</li>\n"
                "<li>person_at_position_2@somewhere.com</li>\n"
                "\n"
                "<li>person_at_position_4@somewhere.com</li>\n"
                "<li>person_at_position_14@somewhere.com</li>\n"
                "</ul>\n";
        string res = m.renderFilenames("sections/list-with-indexes",
            "sections/list-with-indexes");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with missing indexes") {
        string html = "<p>John</p>\n\n";
        string res = m.renderFilenames("sections/list-with-missing-index",
                                       "sections/list-with-missing-index");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with not existing keys") {
        string html = "Simple string: <p>A simple string</p>\n"
                      "Empty string: <p></p>\n"
                      "Zero integer: <p>0</p>\n"
                      "Zero float: <p>0.0</p>\n"
                      "Number integer: <p>23</p>\n"
                      "Number float: <p>23.02</p>\n"
                      "Boolean true: <p>true</p>\n"
                      "Boolean false: <p></p>\n"
                      "Null: <p></p>\n"
                      "Object empty: <p></p>\n"
                      "Object full: <p></p>\n"
                      "Array empty: <p></p>\n"
                      "Array full: <p></p>\n"
                      "Not existing: \n";
        string res = m.renderFilenames("sections/sections-exists-test",
                                       "sections/sections-exists-test");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    // The JSON used for test is:
    //
    // {
    //     "names": [
    //         "John",
    //         "",
    //         23,
    //         0,
    //         45.7,
    //         0.0,
    //         { "name": "Margot" },
    //         {},
    //         [ 1, 2, 7 ],
    //         [],
    //         true,
    //         false,
    //         null
    //     ]
    // }

    SECTION("Section with not existing index") {
        string html = "Array index  0: <p>John</p>\n"
                      "Array index  1: <p></p>\n"
                      "Array index  2: <p>23</p>\n"
                      "Array index  3: <p>0</p>\n"
                      "Array index  4: <p>45.7</p>\n"
                      "Array index  5: <p>0.0</p>\n"
                      "Array index  6: <p></p>\n"
                      "Array index  7: <p></p>\n"
                      "Array index  8: <p></p>\n"
                      "Array index  9: <p></p>\n"
                      "Array index 10: <p>true</p>\n"
                      "Array index 11: <p></p>\n"
                      "Array index 12: <p></p>\n"
                      "Array index not exists: \n";
        string res = m.renderFilenames("sections/sections-exists-test-array",
                                       "sections/sections-exists-test-array");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Compare exists test with value test") {
        string html = "Array index  0: <p>John</p>\n"
                      "Array index  1: \n"
                      "Array index  2: <p>23</p>\n"
                      "Array index  3: \n"
                      "Array index  4: <p>45.7</p>\n"
                      "Array index  5: \n"
                      "Array index  6: <p></p>\n"
                      "Array index  7: \n"
                      "Array index  8: <p></p>\n"
                      "Array index  9: \n"
                      "Array index 10: <p>true</p>\n"
                      "Array index 11: \n"
                      "Array index 12: \n"
                      "Array index not exists: \n";
        string res = m.renderFilenames("sections/sections-exists-test-vs-value-test",
                                       "sections/sections-exists-test-array");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
