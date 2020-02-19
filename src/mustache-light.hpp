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

#ifndef SRC_MUSTACHE_LIGHT_HPP_
#define	SRC_MUSTACHE_LIGHT_HPP_

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>

// Please download from:
//   https://github.com/nlohmann/json/releases/download/v1.0.0-rc1/json.hpp
#include "../json.hpp"

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
    explicit RenderException(const std::string& what) :
            std::runtime_error(what) {
    }
};

/// Tokens:
///   sv  = start variable = {{
///   svu = start variable unescaped= {{{
///   sc  = start comment = {{!
///   sb  = start begin of section = {{#
///   se  = start end of section = {{/
///   si  = start if = {{=
///   s0  = start existing test = {{0
///   su  = start unless = {{^
///   sp  = start partial = {{>
///   st  = start template = {{<
///   ee  = end = }}
///   eeu = end = }}}
///   txt = (sequence of txt)
///
/// Grammar:
///   MESSAGE            := VARIABLE MESSAGE | VARIABLE_UNESCAPED MESSAGE |
///                         COMMENT MESSAGE | SECTION MESSAGE |
///                         IF MESSAGE | UNLESS MESSAGE | EXISTS_TEST MESSAGE |
///                         PARTIAL MESSAGE | txt MESSAGE | (empty)
///   VARIABLE           := sv  txt ee
///   VARIABLE_UNESCAPED := svu txt eeu
///   COMMENT            := sc  txt ee
///   IF                 := si  txt ee
///   EXISTS_TEST        := s0  txt ee
///   UNLESS             := sv  txt ee
///   SECTION            := sb  txt ee MESSAGE se txt ee | sbi txt ee MESSAGE se txt ee
///   PARTIAL            := sp  txt ee | st txt ee
///
class Mustache {
  public:
    // Public part

    // Some types (useful for readability purposes)
    // basically we uses sting and string containers.

    typedef std::map<std::string, std::string> Variables;
    typedef Variables::iterator VariableIterator;
    typedef Variables::const_iterator VariableConstIterator;
    typedef std::string Token;
    typedef std::vector<Token> Tokens;
    // typedef Tokens::iterator TokenIterator;
    typedef long unsigned int TokenIndex;

    /// Construct a new Mustache object, using basePath as base partial
    /// search path.
    ///
    /// @param
    ///     Base path for file searching. Must be an absolute path.
    ///
    explicit Mustache(const std::string& basePath);

    /// Construct a new Mustache object, using basePath as base partial
    /// search path.
    ///
    /// @param basePath
    ///     Base path for file searching. Must be an absolute path.
    /// @param partialExtension
    ///     Default extension for partials.
    ///
    Mustache(const std::string& basePath, const std::string& partialExtension);

    /// Destructor for Mustache object.
    ~Mustache();

    /// Renders a template.
    ///
    /// @param view
    ///      The HTML file with {{ ... }} tags
    /// @param context
    ///      The context (Eg: a std::string containing JSON data)
    ///
    /// @return
    ///     The rendered template.
    ///
    std::string render(const std::string& view, const std::string& context);

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
    std::string render(const std::string& view, const nlohmann::json& context);

    /// Returns error message (if any) or a blank std::string if no error occured.
    ///
    /// @return
    ///      The error message.
    ///
    std::string error() const;

    std::string renderFilenames(const std::string& viewFileName, const std::string& contextFileName);

    std::string fileRead(const std::string& fileName, const std::string& fileExtension);
    std::string fileRead(const std::string& fileName);

  private:
    /// List of valid characters for an identifier.
    static const std::string VALID_CHARS_FOR_ID;

    /// List of all valid characters for partial tag.
    static const std::string VALID_CHARS_FOR_PARTIALS;

    // List of valid tokens
    static const std::string TOKEN_START_VARIABLE;
    static const std::string TOKEN_START_VARIABLE_UNESCAPED;
    static const std::string TOKEN_START_COMMENT;
    static const std::string TOKEN_START_BEGIN_SECTION;
    static const std::string TOKEN_START_END_SECTION;
    static const std::string TOKEN_START_IF;
    static const std::string TOKEN_START_EXISTS_TEST;
    static const std::string TOKEN_START_UNLESS;
    static const std::string TOKEN_START_PARTIAL;
    static const std::string TOKEN_START_TEMPLATE;
    static const std::string TOKEN_END;
    static const std::string TOKEN_END_UNESCAPED;

    /// Default extension to be used for partials
    static const std::string DEFAULT_PARTIAL_EXTENSION;

    /// Base path is added to file name each time a file must be opened.
    std::string basePath_;

    std::string partialExtension_;

    /// The HTML template containing tags to evaluate {{ ... }}
    std::string view_;

    /// The context
    std::string context_;

    nlohmann::json data_;

    /// Stores render result
    std::string rendered_;

    /// Stores error message
    std::string error_;

    Tokens tokens_;

    TokenIndex currentToken_;

    std::size_t currentListCounter_;

    /// Used to manage sections.
    /// When a block {{# var }} ... {{/ var }} is found the parser should
    /// iterate inside it.
    std::stack<nlohmann::json> stack_;

    /// Used to hide/view a section
    bool visible_;

    /// Starts the rendering process.
    /// This is the first method called after parameter read.
    std::string render();

    /// Used for debugging purposes
    Tokens tokenize(const std::string& view);

    // Productions
    void produceMessage();
    void produceVariable();
    void produceVariableUnescaped();
    void produceComment();
    void produceSection();
    void produceIf();
    void produceUnless();
    void produceNullTest();
    void producePartial();

    void printVariable(bool escape_html);

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
        const std::string& valueToSearch);

    /// Throws an exception and stops rendering.
    [[noreturn]] void error(const std::string& message);

    /// Variable get in the current context
    ///
    /// @param key
    ///     The key to serch in the current context.
    ///
    /// @returns
    ///     The value of the corresponding key (as json object)
    ///
    /// @throws std::out_of_range
    ///     If the context is an array and the index is out of bounds.
    /// @throws std::invalid_argument
    ///     If the the key is not found in the key.
    ///
    nlohmann::json searchVariableInContext(const std::string& key);

    std::string getTemplateNameFromContext(const std::string& key);

    /// A valid identifier <em>must</em> contain only lowercase characters,
    /// decimal digits and minus (-). No space or other characters allowed.
    ///
    /// @param id
    ///     The std::string to check.
    ///
    /// @throws RenderError
    ///     If the identifier is not valid.
    void ensureValidIdentifier(const std::string& id, const std::string& validChars = VALID_CHARS_FOR_ID);

    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);

#ifdef DEBUG
    // Dump tokens.
    void dumpTokens();

    // Dump partial variables.
    void dumpVariables();
#endif

    // trim from start
    std::string& ltrim(std::string& s);

    // trim from end
    std::string &rtrim(std::string& s);

    // trim from both ends
    std::string& trim(std::string& s);

    // Escapes dangerous characters
    void htmlEscape(std::string& stringToEscape);

    // Disallow default constructor, copy constructor and assign operator
    Mustache();
    Mustache(const Mustache&);
    void operator=(const Mustache&);
};

} // namespace mustache

#endif  // SRC_MUSTACHE_LIGHT_HPP_

////////////////////////////////////////////////////////////////////////////////
