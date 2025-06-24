#include "main.h"

#include "src/chess.h"

extern Board board; // The main board

DosatoFunctionMapList functions;
void* main_vm;

void init(void* vm) {
    main_vm = vm;

    init_DosatoFunctionMapList(&functions);

    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessInit", chessInit});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessLoadFEN", chessLoadFEN});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetFEN", chessGetFEN});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessPrintBoard", chessPrintBoard});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetFromLocation", chessGetFromLocation});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetFriendly", chessGetFriendly});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetPieceMask", chessGetPieceMask});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetIndex", chessGetIndex});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGenerateMoves", chessGenerateMoves});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetAttackedMap", chessGetAttackedMap});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessgetAttackedMapOnlyPawn", chessgetAttackedMapOnlyPawn});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessUpdateBoardState", chessUpdateBoardState});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetMoveNotation", chessGetMoveNotation});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessMakeMove", chessMakeMove});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessUnMakeMove", chessUnMakeMove});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetZobristHash", chessGetZobristHash});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessGetBoardState", chessGetBoardState});
    write_DosatoFunctionMapList(&functions, (DosatoFunctionMap){"chessStringToMove", chessStringToMove});
}


Value chessInit(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    initChess();

    return BUILD_BOOL(true);
}

Value chessLoadFEN(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value fen_value = GET_ARG(args, 0);
    CAST_SAFE(fen_value, TYPE_STRING);

    char* fen = AS_STRING(fen_value);
    parseFEN(fen);

    return BUILD_BOOL(true);
}

Value chessGetFEN(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    char* fen = generateFEN();

    return RETURN_STRING(fen);
}

Value chessPrintBoard(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value display_side_value = GET_ARG(args, 0);
    CAST_SAFE(display_side_value, TYPE_BOOL);

    printBoard(AS_BOOL(display_side_value) ? WHITE : BLACK);
    
    return BUILD_BOOL(true);
}

Value chessGetFromLocation(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value index_value = GET_ARG(args, 0);
    CAST_SAFE(index_value, TYPE_UBYTE);

    byte from_location = getFromLocation(AS_UBYTE(index_value));

    return BUILD_BYTE(from_location);
}

Value chessGetFriendly(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value color_value = GET_ARG(args, 0);
    CAST_SAFE(color_value, TYPE_BOOL);

    Bitboard friendly = getFriendly(AS_BOOL(color_value) ? WHITE : BLACK);

    return BUILD_ULONG(friendly);
}

Value chessGetPieceMask(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Bitboard piece_mask = getPieceMask();

    return BUILD_ULONG(piece_mask);
}

Value chessGetIndex(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value piece_value = GET_ARG(args, 0);
    CAST_SAFE(piece_value, TYPE_BYTE);

    int index = getIndex(AS_BYTE(piece_value));

    return BUILD_INT(index);
}

Value chessGenerateMoves(ValueArray args, bool debug) {
    if (args.count >= 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }
    bool only_captures = false;
    if (args.count == 1) {
        Value only_captures_value = GET_ARG(args, 0);
        CAST_SAFE(only_captures_value, TYPE_BOOL);
        only_captures = AS_BOOL(only_captures_value);
    }

    Move move_list[256];
    int move_count = generateMoves(move_list, only_captures);

    ValueArray* move_array = malloc(sizeof(ValueArray));
    init_ValueArray(move_array);
    for (int i = 0; i < move_count; i++) {
        Move move = move_list[i];
        Value move_value = BUILD_USHORT(move);
        write_ValueArray(move_array, move_value);
    }

    return RETURN_ARRAY(move_array);
}

Value chessGetAttackedMap(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Bitboard attacked_map = getAttackedMap();

    return BUILD_ULONG(attacked_map);
}

Value chessgetAttackedMapOnlyPawn(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Bitboard attacked_map = getAttackedMapOnlyPawn();

    return BUILD_ULONG(attacked_map);
}

Value chessUpdateBoardState(ValueArray args, bool debug) {
    if (args.count >= 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    bool check_insufficient_material = false;
    if (args.count == 1) {
        Value check_insufficient_material_value = GET_ARG(args, 0);
        CAST_SAFE(check_insufficient_material_value, TYPE_BOOL);
        check_insufficient_material = AS_BOOL(check_insufficient_material_value);
    }

    updateBoardState(check_insufficient_material);

    return BUILD_NULL();
}

Value chessGetMoveNotation(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value move_value = GET_ARG(args, 0);
    CAST_SAFE(move_value, TYPE_USHORT);

    Move move = AS_USHORT(move_value);
    char* notation = getNotation(move);

    return RETURN_STRING(notation);
}

Value chessMakeMove(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value from_value = GET_ARG(args, 0);
    CAST_SAFE(from_value, TYPE_USHORT);

    Move move = AS_USHORT(from_value);
    makeMove(move);

    return BUILD_NULL();
}

Value chessUnMakeMove(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value from_value = GET_ARG(args, 0);
    CAST_SAFE(from_value, TYPE_USHORT);

    Move move = AS_USHORT(from_value);
    unmakeMove(move);

    return BUILD_NULL();
}

Value chessGetZobristHash(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    unsigned long long hash = getZobristHash();

    return BUILD_ULONG(hash);
}

Value chessGetBoardState(ValueArray args, bool debug) {
    if (args.count != 0) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    // turn board struct into object
    ValueObject* board_object = malloc(sizeof(ValueObject));
    init_ValueObject(board_object);
    
    // bitboards
    ValueArray* bitboards_array = malloc(sizeof(ValueArray));
    init_ValueArray(bitboards_array);
    for (int i = 0; i < BB_MAXVAL; i++) {
        Bitboard bb = board.bitboards[i];
        Value bb_value = BUILD_ULONG(bb);
        write_ValueArray(bitboards_array, bb_value);
    }
    write_ValueObject(board_object, BUILD_STRING("bitboards"), BUILD_ARRAY(bitboards_array));

    // side to move
    Value side_to_move_value = BUILD_BOOL(board.side_to_move == WHITE);
    write_ValueObject(board_object, BUILD_STRING("side_to_move"), side_to_move_value);

    // castling rights
    Value castling_rights_value = BUILD_BYTE(board.castling_rights);
    write_ValueObject(board_object, BUILD_STRING("castling_rights"), castling_rights_value);

    // en passant file
    Value en_passant_file_value = BUILD_BYTE(board.en_passant_file);
    write_ValueObject(board_object, BUILD_STRING("en_passant_file"), en_passant_file_value);

    // halfmove clock
    Value halfmove_clock_value = BUILD_SHORT(board.halfmove_clock);
    write_ValueObject(board_object, BUILD_STRING("halfmove_clock"), halfmove_clock_value);

    // fullmove number
    Value fullmove_number_value = BUILD_SHORT(board.fullmove_number);
    write_ValueObject(board_object, BUILD_STRING("fullmove_number"), fullmove_number_value);    

    // state
    Value state_value = BUILD_BYTE(board.state);
    write_ValueObject(board_object, BUILD_STRING("state"), state_value);

    // return the board object
    return RETURN_OBJECT(board_object);
}

Value chessStringToMove(ValueArray args, bool debug) {
    if (args.count != 1) {
        return BUILD_EXCEPTION(E_WRONG_NUMBER_OF_ARGUMENTS);
    }

    Value move_string_value = GET_ARG(args, 0);
    CAST_SAFE(move_string_value, TYPE_STRING);

    char* move_string = AS_STRING(move_string_value);
    Move move = stringToMove(move_string);

    return BUILD_USHORT(move);
}