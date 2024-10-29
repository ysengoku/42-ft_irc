/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:50:56 by faboussa          #+#    #+#             */
/*   Updated: 2024/10/29 17:14:16 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_CLIENT_HPP_
#define INCLUDES_CLIENT_HPP_

#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <sstream>
#include <string>

typedef struct UserModes {
  bool invisible;
  bool operatorOfServer;
  bool registered;
} _userModes;

class Client {
 private:
  int _fd;
  std::string _ip;
  std::string _nickName;
  std::string _userName;
  std::string _realName;
  UserModes _uModes;

 public:
  explicit Client(int fd = -1, const std::string& ip = "");

  /* Getters */
  int getFd() const { return _fd; }
  std::string getIp() const { return _ip; }
  const std::string &getNickName() const;
  const std::string &getUserName() const;
  const std::string &getRealName() const;
  const UserModes &getUserModes() const;
  const std::string getUserModesFlag() const;

  /* Setters */
  void setFd(int fd) { _fd = fd; }
  void setIp(const std::string& ip) { _ip = ip; }
  void setNickName(const std::string& nickName);
  void setUserName(const std::string& userName);
  void setRealName(const std::string& realName);
  void setUInvisibleMode(bool isInvisible);
  void setUOperatorMode(bool isOperator);
  void setURegisteredMode(bool isRegistered);

  /* Messages handling */
  void receiveMessage(const std::string& message);
  std::string shareMessage();
};

#endif  // INCLUDES_CLIENT_HPP_
