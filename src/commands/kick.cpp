/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:20:03 by yusengok          #+#    #+#             */
/*   Updated: 2024/11/15 18:44:25 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>

#include "../../includes/Server.hpp"

void Server::kick(int fd, const std::string &param) {
  Client &client = _clients.at(fd);
  if (param.empty()) {
    send461NeedMoreParams(client, "KICK");
    return;
  }
  std::istringstream iss(param);
  std::string channelName, targetNick, reason;
  std::getline(iss, channelName, ' ');
  std::getline(iss, targetNick, ' ');
  if (targetNick.empty()) {
    send461NeedMoreParams(client, "KICK");
    return;
  }
  if (iss.rdbuf()->in_avail() > 0) {
    std::getline(iss >> std::ws, reason);
  }
  if (reason.length() >= gConfig->getLimit(KICKLEN)) {
    reason = reason.substr(0, gConfig->getLimit(KICKLEN));
  }
  if (reason[0] == ':') {
    reason = reason.substr(1);
  }
#ifdef DEBUG
  std::cout << "[KICK] Channel: " << channelName << " / Target: " << targetNick
            << " / Reason: " << reason << std::endl;
#endif
  if (!channelExists(channelName)) {
    send403NoSuchChannel(client, channelName);
    return;
  }
  Channel &channel = _channels.at(channelName.substr(1));
  if (!channel.isOperator(client.getFd())) {
    send482ChanOPrivsNeeded(client, channel);
    return;
  }
  Client *targetClient = findClientByNickname(targetNick);
  if (targetClient == NULL) {
    send401NoSuchNick(client, targetNick);
    return;
  }
  int fdTarget = targetClient->getFd();
  if (!channel.isClientInChannel(fdTarget)) {
    send441UserNotInChannel(client, targetNick, channel);
    return;
  }
  std::string message = targetNick + " " + reason + "\r\n";
  broadcastInChannel(client, channel, "KICK", message);
  quitChannel(fdTarget, &channel, targetClient);
}
