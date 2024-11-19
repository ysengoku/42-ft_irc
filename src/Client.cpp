/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:50:56 by faboussa          #+#    #+#             */
/*   Updated: 2024/11/19 12:24:31 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "../includes/Config.hpp"
#include "../includes/colors.hpp"
#include "../includes/Server.hpp"

extern Config* gConfig;

/*============================================================================*/
/*       Constructors                                                         */
/*============================================================================*/

Client::Client(int fd, const std::string& ip, const std::string& hostName)
    : _fd(fd),
      _ip(ip),
      _hostName(hostName),
      _nickname("*"),
      _userName("*"),
      _realName("*"),
      _nicknameSet(false),
      _usernameSet(false),
      _realnameSet(false),
      _passwordGiven(false),
      _accepted(false),
      _nbPassAttempts(0),
      _channelsCount(0) {}

/*============================================================================*/
/*       Getters                                                              */
/*============================================================================*/

std::string const& Client::getNickname(void) const { return (_nickname); }

std::string const& Client::getUserName(void) const { return (_userName); }

uint8_t Client::getChannelsCount() const { return (_channelsCount); }

int Client::getFd(void) const { return (_fd); }

std::string Client::getIp(void) const { return (_ip); }

std::string const& Client::getHostName(void) const { return (_hostName); }

std::string const& Client::getRealName(void) const { return (_realName); }

bool Client::isNicknameSet(void) const { return (_nicknameSet); }

bool Client::isUsernameSet(void) const { return (_usernameSet); }

bool Client::isRealnameSet(void) const { return (_realnameSet); }

bool Client::isPasswordGiven(void) const { return (_passwordGiven); }

bool Client::isAccepted(void) const { return (_accepted); }

uint8_t Client::getNbPassAttempts(void) const { return (_nbPassAttempts); }

/*============================================================================*/
/*       Setters                                                              */
/*============================================================================*/

void Client::setNickname(const std::string& nickname) {
  _nickname = nickname;
  _nicknameSet = true;
}

void Client::setUserName(const std::string& username) {
  _userName = username;
  _usernameSet = true;
}

void Client::setRealName(const std::string& realname) {
  _realName = realname;
  _realnameSet = true;
}

void Client::setFd(int fd) { _fd = fd; }

void Client::setIp(const std::string& ip) { _ip = ip; }

void Client::setHostName(const std::string& hostname) { _hostName = hostname; }

void Client::declareAccepted(void) { _accepted = true; }

void Client::declarePasswordGiven(void) { _passwordGiven = true; }

void Client::incrementNbPassAttempts(void) { ++_nbPassAttempts; }

/*============================================================================*/
/*       Messages handling                                                    */
/*============================================================================*/

void Client::receiveMessage(const std::string& message) const {
  if (_fd != -1) {
    std::ostringstream oss;
    if (send(_fd, message.c_str(), message.length(), MSG_NOSIGNAL) == -1) {
      // std::cerr << RED "Error while sending message to fd " << _fd << ": "
      //           << strerror(errno) << RESET << std::endl;
      oss << "Error while sending message to fd " << _fd << ": "
          << strerror(errno);
      Server::printLog(ERROR_LOG, CLIENT, oss.str());
    } else {
      std::string trimed = message;
      trimed.erase(trimed.find_last_not_of("\r\n") + 1);
      oss << "Sent to " << _nickname << ": " << trimed;
      Server::printLog(INFO_LOG, REPLY, oss.str());
    }
  } else {
    // std::cerr << RED "Invalid file descriptor" RESET << std::endl;
    Server::printLog(ERROR_LOG, SYSTEM, "Invalid file descriptor");
  }
}

std::string Client::shareMessage(void) {
  char buffer[1024] = {0};
  ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);
  // std::cout << MAGENTA << "Received message from client " << _fd << ": " <<
  // buffer
  //           << RESET << std::endl;
  if (bytesRead == -1) {
    // std::cerr << RED "Error while receiving message: " RESET << std::endl;
    std::ostringstream oss;
    oss << _nickname << _fd << ": Error while receiving message";
    Server::printLog(ERROR_LOG, CLIENT, oss.str());
    return ("");
  } else if (bytesRead == 0) {
    // std::cerr << RED "Connection closed by peer" RESET << std::endl;
    std::ostringstream oss;
    oss << _nickname << _fd << ": Connection closed by peer";
    Server::printLog(ERROR_LOG, CLIENT, oss.str());
    return ("");
  }
  buffer[bytesRead] = '\0';
  return (std::string(buffer));
}

/*============================================================================*/
/*       Channel handling                                                     */
/*============================================================================*/

void Client::incrementChannelsCount(void) {
// #ifdef DEBUG
//   std::cout << std::endl << std::endl;
//   std::cout << "increment _channelsCount " << _channelsCount << std::endl;
// #endif
  if (_channelsCount <= gConfig->getLimit(CHANLIMIT)) {
    ++_channelsCount;

#ifdef DEBUG
    std::ostringstream oss;
    oss << _nickname << ": currently in " << _channelsCount << " channel(s)";
    Server::printLog(DEBUG_LOG, CLIENT, oss.str());
#endif
  }
}

void Client::decrementChannelsCount(void) {
// #ifdef DEBUG
//   std::cout << std::endl << std::endl;

//   std::cout << "decrement _channelsCount " << _channelsCount << std::endl;
// #endif
  if (_channelsCount > 0) {
    --_channelsCount;

#ifdef DEBUG
    std::ostringstream oss;
    oss << _nickname << ": currently in " << _channelsCount << " channel(s)";
    Server::printLog(DEBUG_LOG, CLIENT, oss.str());
#endif
  }
}
