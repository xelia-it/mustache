////////////////////////////////////////////////////////////////////////////////
///
/// @file       mustache-light.hpp
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
/// @brief      Mustache template parser.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_MUSTACHE_LIGHT_HPP_
#define	SRC_MUSTACHE_LIGHT_HPP_

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>

// Please download from:
//   https://github.com/nlohmann/json/releases/download/v1.0.0-rc1/json.hpp
#include <json.hpp>
// For convenience use a namespace alias
using json = nlohmann::json;

using std::string;
using std::vector;
using std::map;
using std::stack;

namespace mustache {

/// An exception specific for Mustache.
/// It's used to signal to application:
/// * Variables not found
/// * Files (or partials) not found
/// * Wrong characters in tags
/// * Syntax errors
///
class RenderException : public std::runtime_error {
  public:
    // Public part

    /// Construct a RenderException object.
    ///
    /// @param what
    ///      Error message
    ///
    explicit RenderException(const string& what) :
            std::runtime_error(what) {
    }
};

/// Tokens:
///   sv = start variable = {{
///   sb = start begin of section = {{#
///   se = start end of section = {{/
///   si = start if = {{=
///   su = start unless = {{^
///   sp = start partial = {{>
///   st = start template = {{<
///   ee = end = }}
///   txt = (sequence of txt)
///
/// Grammar:
///   MESSAGE  := VARIABLE MESSAGE | SECTION MESSAGE |
///               IF MESSAGE | UNLESS MESSAGE |
///               PARTIAL MESSAGE | txt MESSAGE | (empty)
///   VARIABLE := sv txt ee
///   IF       := sv txt ee
///   UNLESS   := sv txt ee
///   SECTION  := sb txt ee MESSAGE se txt ee | sbi txt ee MESSAGE se txt ee
///   PARTIAL  := sp txt ee | st txt ee
///
class Mustache {
  public:
    // Public part

    // Some types (useful for readability purposes)
    // basically we uses sting and string containers.

    typedef map<string, string> Variables;
    typedef Variables::iterator VariableIterator;
    typedef Variables::const_iterator VariableConstIterator;
    typedef string Token;
    typedef vector<Token> Tokens;
    // typedef Tokens::iterator TokenIterator;
    typedef long unsigned int TokenIndex;

    /// Construct a new Mustache object, using basePath as base partial
    /// search path.
    ///
    /// @param basePath
    ///     Base path for file searching. Must be an absolute path.
    ///
    explicit Mustache(const string& basePath);

    /// Construct a new Mustache object, using basePath as base partial
    /// search path.
    ///
    /// @param basePath
    ///     Base path for file searching. Must be an absolute path.
    /// @param partialExtension
    ///     Default extension for partials.
    ///
    Mustache(const string& basePath, const string& partialExtension);

    /// Destructor for Mustache object.
    ~Mustache();

    /// Renders a template.
    ///
    /// @param view
    ///      The HTML file with {{ ... }} tags
    /// @param context
    ///      The context (Eg: a string containing JSON data)
    ///
    /// @return
    ///     The rendered template.
    ///
    string render(const string& view, const string& context);

    /// Renders a template.
    ///
    /// @param view
    ///      The HTML file with {{ ... }} tags
    /// @param context
    ///      The context (Eg: the JSON object)
    ///
    /// @return
    ///     The rendered template.
    ///
    string render(const string& view, const json& context);

    /// Returns error message (if any) or a blank string if no error occured.
    ///
    /// @return
    ///      The error message.
    ///
    string error() const;

    string renderFilenames(const string& viewFileName, const string& contextFileName);

    string fileRead(const string& fileName, const string& fileExtension);
    string fileRead(const string& fileName);

  private:
    /// List of valid characters for an identifier.
    static const string VALID_CHARS_FOR_ID;

    /// List of all valid characters for partial tag.
    static const string VALID_CHARS_FOR_PARTIALS;
    static const string TOKEN_START_VARIABLE;
    static const string TOKEN_START_BEGIN_SECTION;
    static const string TOKEN_START_END_SECTION;
    static const string TOKEN_START_IF;
    static const string TOKEN_START_UNLESS;
    static const string TOKEN_START_PARTIAL;
    static const string TOKEN_START_TEMPLATE;
    static const string TOKEN_END;
    static const string DEFAULT_PARTIAL_EXTENSION;

    /// Base path is added to file name each time a file must be opened.
    string basePath_;

    string partialExtension_;

    /// The HTML template containing tags to evaluate {{ ... }}
    string view_;

    /// The context
    string context_;

    json data_;

    /// Stores render result
    string rendered_;

    /// Stores error message
    string error_;

    Tokens tokens_;

    TokenIndex currentToken_;

    std::size_t currentListCounter_;

    /// Used to manage sections.
    /// When a block {{#var}} ... {{/var}} is found the parser should
    /// iterate inside it.
    stack<json> stack_;

    /// Used to hide/view a section
    bool visible_;

    /// Starts the rendering process.
    /// This is the first method called after parameter read.
    string render();

    /// Used for debugging purposes
    Tokens tokenize(const string& view);

    // Productions
    void produceMessage();
    void produceVariable();
    void produceSection();
    void produceIf();
    void produceUnless();
    void producePartial();

    void partialSubstitute(const Tokens partialParams, Tokens& newTokens);

    /// Searches a variable inside a Variable list.
    ///
    /// @param  variables     The variable list
    /// @param  valueToSearch The key to search for
    ///
    /// @return               A VariableConstIterator pointer to pair found,
    ///                       or variable_.end() if nothing is found.
    ///
    VariableConstIterator partialSearchVariable(const Variables& variables,
        const string& valueToSearch);

    /// Throws an exception and stops rendering.
    void error(const string& message);

    /// Variable get in the current context
    /// TODO: refactor name..
    json searchContext(const string& key);


    /// A valid identifier <em>must</em> contain only lowercase characters,
    /// decimal digits and minus (-). No space or other characters allowed.
    ///
    /// @param id
    ///     The string to check.
    ///
    /// @throws RenderError
    ///     If the identifier is not valid.
    void ensureValidIdentifier(const string& id, const string& validChars = VALID_CHARS_FOR_ID);

    vector<string>& split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);

#ifdef DEBUG
    // Dump tokens.
    void dumpTokens();

    // Dump partial variables.
    void dumpVariables();
#endif

    // trim from start
    string& ltrim(string& s);

    // trim from end
    string &rtrim(string& s);

    // trim from both ends
    string& trim(string& s);

    // Disallow default constructor, copy constructor and assign operator
    Mustache();
    Mustache(const Mustache&);
    void operator=(const Mustache&);
};

} // namespace mustache

#endif  // SRC_MUSTACHE_LIGHT_HPP_

////////////////////////////////////////////////////////////////////////////////
