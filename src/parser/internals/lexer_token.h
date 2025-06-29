#ifndef BIGINT_CALC_LEXER_TOKEN_H
#define BIGINT_CALC_LEXER_TOKEN_H

#include "string_ty.h"
#include "types.h"

/*!
 * @brief tags for all the components of an expression.
 */
enum lexer_token_type
{
	INVALID,        //!< an invalid token.
	ASSIGN_ADD,     //!< "+="
	ASSIGN_DIV,     //!< "/="
	ASSIGN_MOD,     //!< "%="
	ASSIGN_MUL,     //!< "*="
	ASSIGN_POW,     //!< "^="
	ASSIGN_SUB,     //!< "-="
	ASSIGN,         //!< "="
	COMMENT_BLOCK,  //!< a block comment.
	COMMENT_LINE,   //!< a line comment.
	ID,             //!< an identifier.
	KW_BREAK,       //!< "break"
	KW_DEF,         //!< "def"
	KW_ELIF,        //!< "elif"
	KW_ELSE,        //!< "else"
	KW_EXIT,        //!< "exit"
	KW_FALSE,       //!< "false"
	KW_FOR,         //!< "for"
	KW_IF,          //!< "if"
	KW_IN,          //!< "in"
	KW_RETURN,      //!< "return"
	KW_TRUE,        //!< "true"
	KW_WHILE,       //!< "while"
	NUM,            //!< a number.
	OP_ACCESS,      //!< "."
	OP_ADD,         //!< "+"
	OP_DIV,         //!< "/"
	OP_EQ,          //!< "=="
	OP_GE,          //!< ">="
	OP_GT,          //!< ">"
	OP_LE,          //!< "<="
	OP_LT,          //!< "<"
	OP_MOD,         //!< "%"
	OP_MUL,         //!< "*"
	OP_POW,         //!< "^"
	OP_SUB,         //!< "-"
	STRING,         //!< a string.
	SYM_BSLASH,     //!< "\"
	SYM_COMMA,      //!< ","
	SYM_CURLY_L,    //!< "{"
	SYM_CURLY_R,    //!< "}"
	SYM_PAREN_L,    //!< "("
	SYM_PAREN_R,    //!< ")"
	SYM_SEMICOLON,  //!< ";"
	SYM_SQUARE_L,   //!< "["
	SYM_SQUARE_R,   //!< "]"
};

/*!
 * @brief container with details about a program token.
 */
typedef struct lexer_token
{
	/*! @public slice of the the string where token was found. */
	string *restrict str;
	len_ty line, column, offset;
	/*! @public the type of the token. */
	enum lexer_token_type id;
} lexer_token;

#endif  // BIGINT_CALC_LEXER_TOKEN_H
