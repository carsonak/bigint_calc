#ifndef BIGINT_CALC_LEXER_TOKEN_H
#define BIGINT_CALC_LEXER_TOKEN_H

#include "string_view.h"

/*!
 * @brief tags for all the components of an expression.
 */
enum lexer_token_type
{
	ID,  //!< an identifier, starts with `[_a-zA-Z]` followed by `[_a-zA-Z0-9]*`.
	KW_BREAK,   //!< "break"
	KW_DEF,     //!< "def"
	KW_ELSE,    //!< "else"
	KW_EXIT,    //!< "exit"
	KW_IF,      //!< "if"
	KW_PRINT,   //!< "print"
	KW_RETURN,  //!< "return"
	KW_WHILE,   //!< "while"
	NUMBER,     //!< a number, starts with `[0-9]` followed by `[_a-zA-Z0-9]*`.
	OP_ADD,     //!< "+"
	OP_DEC,     //!< "--"
	OP_DIV,     //!< "/"
	OP_EQ,      //!< "=="
	OP_GTEQ,    //!< ">="
	OP_GT,      //!< ">"
	OP_INC,     //!< "++"
	OP_LTEQ,    //!< "<="
	OP_LT,      //!< "<"
	OP_MOD,     //!< "%"
	OP_MUL,     //!< "*"
	OP_POW,     //!< "^"
	OP_SUB,     //!< "-"
	STRING,     //!< a string, starts with '"' till the next '"'.
	SYM_ASSIGN,     //!< "="
	SYM_BCOMMENT,   //!< a block comment, starts with "/*" till the next "*/".
	SYM_BSLASH,     //!< "\"
	SYM_COMMA,      //!< ","
	SYM_CURLY_L,    //!< "{"
	SYM_CURLY_R,    //!< "}"
	SYM_LCOMMENT,   //!< a line comment, starts with "#" till the next "\n".
	SYM_NEWLINE,    //!< "\n"
	SYM_PAREN_L,    //!< "("
	SYM_PAREN_R,    //!< ")"
	SYM_SEMICOLON,  //!< ";"
	SYM_SQUARE_L,   //!< "["
	SYM_SQUARE_R,   //!< "]"
	INVALID,        //!< an invalid token.
};

/*!
 * @brief container with details about a program token.
 */
typedef struct lexer_token
{
	/*! @public slice of the the string where token was found. */
	string_view str;
	/*! @public line number where token occurred in the file. */
	len_ty line_num;
	/*! @public the type of the token. */
	enum lexer_token_type id;
} lexer_token;

#endif  // BIGINT_CALC_LEXER_TOKEN_H
