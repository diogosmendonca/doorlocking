#ifndef __MODEL_H__
#define __MODEL_H__

enum StatusEnum {INACTIVE, ACTIVE};

enum UserTypeEnum {NORMAL_USER, ADMIN};

struct user{
  String username;
  String accessCode;
  StatusEnum userStatus;
  UserTypeEnum type; 
};

typedef struct user User;

enum EventsEnum {CONNECT, OPEN_DOOR, REGISTER_USER, ACTIVATE_USER, DEACTIVATE_USER, LOGIN_SUCCESS, LOGIN_FAIL, LOGOUT};

struct log_register{
  time_t datetime;
  EventsEnum event;
  String payload;
};

typedef struct log_register LogRegister;

struct session{
  String username;
  String sessionid;
  time_t startTime;
  time_t expires;
  StatusEnum sessionStatus;
};

typedef struct session UserSession;

#endif
