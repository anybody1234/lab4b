/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab *symtab;
extern Token *currentToken;

/*
 * Look up an identifier in the current scope chain, then in the list of
 * predeclared global objects (READC/READI/WRITEI/WRITEC/WRITELN).
 */
Object *lookupObject(char *name)
{
  Scope *scope = symtab->currentScope;
  Object *obj = NULL;

  while (scope != NULL)
  {
    obj = findObject(scope->objList, name);
    if (obj != NULL)
      return obj;
    scope = scope->outer;
  }

  return findObject(symtab->globalObjectList, name);
}

void checkFreshIdent(char *name)
{
  Object *obj = findObject(symtab->currentScope->objList, name);
  if (obj != NULL)
  {
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }

  /* Prevent duplicates with predeclared global names at the program scope. */
  if (symtab->currentScope->outer == NULL)
  {
    obj = findObject(symtab->globalObjectList, name);
    if (obj != NULL)
    {
      error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
    }
  }
}

Object *checkDeclaredIdent(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredConstant(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL || obj->kind != OBJ_CONSTANT)
  {
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredType(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL || obj->kind != OBJ_TYPE)
  {
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredVariable(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL || (obj->kind != OBJ_VARIABLE && obj->kind != OBJ_PARAMETER))
  {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredFunction(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL || obj->kind != OBJ_FUNCTION)
  {
    error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredProcedure(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL || obj->kind != OBJ_PROCEDURE)
  {
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredLValueIdent(char *name)
{
  Object *owner = symtab->currentScope->owner;

  /* In a function body, "F := ..." assigns the function's return value. */
  if (owner != NULL && owner->kind == OBJ_FUNCTION && strcmp(owner->name, name) == 0)
  {
    return owner;
  }

  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }

  if (obj->kind == OBJ_VARIABLE || obj->kind == OBJ_PARAMETER)
  {
    return obj;
  }

  error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
  return NULL;
}
