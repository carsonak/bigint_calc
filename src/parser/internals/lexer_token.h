#ifndef BIGINT_CALC_LEXER_TOKEN_H
#define BIGINT_CALC_LEXER_TOKEN_H

#include "string_ty.h"

/*!
 * @brief tags for all the components of an expression.
 */
enum lexer_token_type
{
	ID,                //!< an identifier.
	KW_BREAK,          //!< "break"
	KW_DEF,            //!< "def"
	KW_ELIF,           //!< "elif"
	KW_ELSE,           //!< "else"
	KW_EXIT,           //!< "exit"
	KW_FALSE,          //!< "false"
	KW_IF,             //!< "if"
	KW_RETURN,         //!< "return"
	KW_TRUE,           //!< "true"
	KW_WHILE,          //!< "while"
	NUMBER,            //!< a number.
	OP_ADD,            //!< "+"
	OP_DIV,            //!< "/"
	OP_EQ,             //!< "=="
	OP_GE,             //!< ">="
	OP_GT,             //!< ">"
	OP_LE,             //!< "<="
	OP_LT,             //!< "<"
	OP_MOD,            //!< "%"
	OP_MUL,            //!< "*"
	OP_POW,            //!< "^"
	OP_SUB,            //!< "-"
	STRING,            //!< a string.
	SYM_ACCESS,        //!< "."
	SYM_ASSIGN_ADD,    //!< "+="
	SYM_ASSIGN_DIV,    //!< "/="
	SYM_ASSIGN_MOD,    //!< "%="
	SYM_ASSIGN_MUL,    //!< "*="
	SYM_ASSIGN_POW,    //!< "^="
	SYM_ASSIGN_SUB,    //!< "-="
	SYM_ASSIGN,        //!< "="
	SYM_BSLASH,        //!< "\"
	SYM_COMMA,         //!< ","
	SYM_COMMENT_BLCK,  //!< a block comment.
	SYM_COMMENT_LINE,  //!< a line comment.
	SYM_CURLY_L,       //!< "{"
	SYM_CURLY_R,       //!< "}"
	SYM_PAREN_L,       //!< "("
	SYM_PAREN_R,       //!< ")"
	SYM_SEMICOLON,     //!< ";"
	SYM_SQUARE_L,      //!< "["
	SYM_SQUARE_R,      //!< "]"
	INVALID,           //!< an invalid token.
};

/*!
 * @brief container with details about a program token.
 */
typedef struct lexer_token
{
	/*! @public slice of the the string where token was found. */
	string *restrict str;
	/*! @public the type of the token. */
	enum lexer_token_type id;
} lexer_token;

#endif  // BIGINT_CALC_LEXER_TOKEN_H
