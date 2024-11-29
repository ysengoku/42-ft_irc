/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:04:35 by yusengok          #+#    #+#             */
/*   Updated: 2024/11/28 18:41:14 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"

#ifdef DEBUG
#define EACH_CHANNEL "WHO: Listing all clients for each Channel"
static void printDebugWho(const std::string &channelName);
#endif

void Server::who(const Client &client, const std::string &arg) {
  if (arg.empty()) {
#ifdef DEBUG
    // {
    // std::ostringstream oss;
    // oss << "WHO: Listing all clients for each Channel";
    printLog(DEBUG_LOG, COMMAND, EACH_CHANNEL);
    // }
#endif
    channelsMap::iterator itEnd = _channels.end();
    for (channelsMap::iterator it = _channels.begin(); it != itEnd; ++it) {
#ifdef DEBUG
      printDebugWho(it->first);
      // {
      //   std::ostringstream oss;
      //   oss << FOR_CHANNEL << it->first;
      //   printLog(DEBUG_LOG, COMMAND, oss.str());
      // }
#endif
      sendClientsListInChannel(client, it->second);
    }
  } else if (arg[0] != '#') {
    send476BadChanMask(client, arg);
  } else {
    if (channelExists(arg)) {
#ifdef DEBUG
      printDebugWho(arg);
      // {
      //   std::ostringstream oss;
      //   oss << FOR_CHANNEL << arg;
      //   printLog(DEBUG_LOG, COMMAND, oss.str());
      // }
#endif
      Channel &channel = _channels.at(arg.substr(1));
      sendClientsListInChannel(client, channel);
    } else {
      send403NoSuchChannel(client, arg);
    }
  }
}

void Server::sendClientsListInChannel(const Client &client,
                                      const Channel &channel) {
  const clientPMap &clientsInChannel = channel.getChannelClients();
  clientPMap::const_iterator itEnd = clientsInChannel.end();
#ifdef DEBUG
  {
    std::ostringstream oss;
    oss << "WHO: Number of clients in " << channel.getNameWithPrefix() << ": "
        << clientsInChannel.size();
    printLog(DEBUG_LOG, COMMAND, oss.str());
  }
#endif
  for (clientPMap::const_iterator it = clientsInChannel.begin(); it != itEnd;
       ++it) {
    send352Whoreply(client, *it->second, channel);
  }
  send315EndOfWho(client, channel);
}

#ifdef DEBUG
void printDebugWho(const std::string &channelName) {
  std::ostringstream oss;
  oss << "WHO: Listing all clients for " << channelName;
  Server::printLog(DEBUG_LOG, COMMAND, oss.str());
}
#endif
