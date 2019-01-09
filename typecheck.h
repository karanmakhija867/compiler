#ifndef TYPECHK_H
#define TYPECHK_H

#include "symboltable.h"
#include <stdio.h>


HashTree * typeCheck(AstNode * astRoot, HashTree * parent);

#endif