/**
* Redis C++11 wrapper.
*/

#include <vector>
#include <set>
#include <unordered_set>

#include "command.hpp"
#include "redox.hpp"

namespace redox {

template<class ReplyT>
bool Command<ReplyT>::is_error_reply() {

  if (reply_obj->type == REDIS_REPLY_ERROR) {
    logger.error() << cmd << ": " << reply_obj->str;
    return true;
  }
  return false;
}

template<class ReplyT>
bool Command<ReplyT>::is_nil_reply() {

  if (reply_obj->type == REDIS_REPLY_NIL) {
    logger.warning() << cmd << ": Nil reply.";
    return true;
  }
  return false;
}

template<>
void Command<redisReply*>::invoke_callback() {
  invoke(reply_obj);
}

template<>
void Command<std::string>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);
  else if(is_nil_reply()) invoke_error(REDOX_NIL_REPLY);

  else if(reply_obj->type != REDIS_REPLY_STRING && reply_obj->type != REDIS_REPLY_STATUS) {
    logger.error() << cmd << ": Received non-string reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    std::string s(reply_obj->str, reply_obj->len);
    invoke(s);
  }
}

template<>
void Command<char*>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);
  else if(is_nil_reply()) invoke_error(REDOX_NIL_REPLY);

  else if(reply_obj->type != REDIS_REPLY_STRING && reply_obj->type != REDIS_REPLY_STATUS) {
    logger.error() << cmd << ": Received non-string reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    invoke(reply_obj->str);
  }
}

template<>
void Command<int>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);
  else if(is_nil_reply()) invoke_error(REDOX_NIL_REPLY);

  else if(reply_obj->type != REDIS_REPLY_INTEGER) {
    logger.error() << cmd << ": Received non-integer reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    invoke((int) reply_obj->integer);
  }
}

template<>
void Command<long long int>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);
  else if(is_nil_reply()) invoke_error(REDOX_NIL_REPLY);

  else if(reply_obj->type != REDIS_REPLY_INTEGER) {
    logger.error() << cmd << ": Received non-integer reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    invoke(reply_obj->integer);
  }
}

template<>
void Command<std::nullptr_t>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);

  else if(reply_obj->type != REDIS_REPLY_NIL) {
    logger.error() << cmd << ": Received non-nil reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    invoke(nullptr);
  }
}


template<>
void Command<std::vector<std::string>>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);

  else if(reply_obj->type != REDIS_REPLY_ARRAY) {
    logger.error() << cmd << ": Received non-array reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    std::vector<std::string> v;
    size_t count = reply_obj->elements;
    for(size_t i = 0; i < count; i++) {
      redisReply* r = *(reply_obj->element + i);
      if(r->type != REDIS_REPLY_STRING) {
        logger.error() << cmd << ": Received non-array reply.";
        invoke_error(REDOX_WRONG_TYPE);
      }
      v.emplace_back(r->str, r->len);
    }
    invoke(v);
  }
}

template<>
void Command<std::unordered_set<std::string>>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);

  else if(reply_obj->type != REDIS_REPLY_ARRAY) {
    logger.error() << cmd << ": Received non-array reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    std::unordered_set<std::string> v;
    size_t count = reply_obj->elements;
    for(size_t i = 0; i < count; i++) {
      redisReply* r = *(reply_obj->element + i);
      if(r->type != REDIS_REPLY_STRING) {
        logger.error() << cmd << ": Received non-array reply.";
        invoke_error(REDOX_WRONG_TYPE);
      }
      v.emplace(r->str, r->len);
    }
    invoke(v);
  }
}

template<>
void Command<std::set<std::string>>::invoke_callback() {

  if(is_error_reply()) invoke_error(REDOX_ERROR_REPLY);

  else if(reply_obj->type != REDIS_REPLY_ARRAY) {
    logger.error() << cmd << ": Received non-array reply.";
    invoke_error(REDOX_WRONG_TYPE);

  } else {
    std::set<std::string> v;
    size_t count = reply_obj->elements;
    for(size_t i = 0; i < count; i++) {
      redisReply* r = *(reply_obj->element + i);
      if(r->type != REDIS_REPLY_STRING) {
        logger.error() << cmd << ": Received non-array reply.";
        invoke_error(REDOX_WRONG_TYPE);
      }
      v.emplace(r->str, r->len);
    }
    invoke(v);
  }
}

} // End namespace redox
