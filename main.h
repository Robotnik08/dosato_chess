#ifndef MAIN_H
#define MAIN_H

#include "dosato_source/dosato.h" // Include the dosato library API


// each library must have a function called init that initializes the functions
void init(void* vm);

// each library must have a dosatoFunctionMapList called functions
DosatoFunctionMapList functions;

// function forward declaration
Value chessInit(ValueArray args, bool debug);
Value chessLoadFEN(ValueArray args, bool debug);
Value chessGetFEN(ValueArray args, bool debug);
Value chessPrintBoard(ValueArray args, bool debug);
Value chessGetFromLocation(ValueArray args, bool debug);
Value chessGetFriendly(ValueArray args, bool debug);
Value chessGetPieceMask(ValueArray args, bool debug);
Value chessGetIndex(ValueArray args, bool debug);
Value chessGenerateMoves(ValueArray args, bool debug);
Value chessGetAttackedMap(ValueArray args, bool debug);
Value chessgetAttackedMapOnlyPawn(ValueArray args, bool debug);
Value chessUpdateBoardState(ValueArray args, bool debug);
Value chessGetMoveNotation(ValueArray args, bool debug);
Value chessMakeMove(ValueArray args, bool debug);
Value chessUnMakeMove(ValueArray args, bool debug);
Value chessGetZobristHash(ValueArray args, bool debug);
Value chessGetBoardState(ValueArray args, bool debug);

#endif // MAIN_H