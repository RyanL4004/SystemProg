// protocol.hpp
// Kyle Mather
// Shared protocol definitions for the Musical Chairs socket game.
// Included by both mom and kid modules.
//
// NOTE: No objects are defined here (only constants and an enum) to avoid
// linker errors when included in multiple compilation units.

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#define PORT     1234
#define MAX_KIDS 25
#define MAX_MSG  256

// All messages are ASCII strings terminated with '\n'.
// Messages with data:
//   HELLO <name>     kid->mom  : kid announces itself
//   GETUP <nchairs>  mom->kid  : round starting, <nchairs> chairs available
//   WANT  <n>        kid->mom  : kid requests chair number <n>
//   NACK  <chairs>   mom->kid  : chair taken; <chairs> is a string where
//                                '1'=available and '0'=occupied
// Messages with no data:
//   SIT              mom->kid  : stop marching, pick a seat
//   ACK              mom->kid  : chair granted
//   QUIT             mom->kid  : you are eliminated
//   PRIZE            mom->kid  : you won!

enum MsgType { MSG_HELLO, MSG_GETUP, MSG_SIT, MSG_WANT,
               MSG_ACK,   MSG_NACK,  MSG_QUIT, MSG_PRIZE };

#endif
