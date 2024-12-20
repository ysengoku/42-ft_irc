/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleServerMessage.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:59:38 by yusengok          #+#    #+#             */
/*   Updated: 2024/12/03 20:02:01 by yusengok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <string>

#include "../../includes/Bot.hpp"
#include "../../includes/Log.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/utils.hpp"

void Bot::handleServerMessage(void) {
  std::string requestBuffer = readMessageFromServer();
  if (requestBuffer.empty()) {
    Log::printLog(ERROR_LOG, BOT_L, "Server connection closed");
    close(_botSocketFd);
    _signal = true;
    return;
  }
  Log::printLog(DEBUG_LOG, BOT_L, "Received from Server: " + requestBuffer);
  if (requestBuffer[0] != ':' ||
      requestBuffer.find("PRIVMSG") == std::string::npos)
    return;
  BotRequest newRequest = parseRequest(requestBuffer);
#ifdef DEBUG
  debugLogParsedMessage(newRequest);
#endif
  switch (newRequest.command) {
    case HELLO:
      sendAsciiCatByCommand(&newRequest, HELLO);
      break;
    case WEATHER:
      weather(&newRequest);
      break;
    case JOKE:
      joke(&newRequest);
      break;
    case INSULTME:
      insultMe(&newRequest);
      break;
    case ADVICE:
      advice(&newRequest);
      break;
    case UNKNOWN_BOT_COMMAND:
      unknownCommand(&newRequest);
      break;
    default:
      break;
  }
}

/*============================================================================*/
/*       Parse requests                                                       */
/*============================================================================*/

static eBotCommand selectCommand(const std::string& command) {
  std::srand(std::time(NULL));

  if (command == "HELLO") {
    return (HELLO);
  } else if (command == "JOKE") {
    return (JOKE);
  } else if (command == "INSULTME") {
    return (INSULTME);
  } else if (command == "ADVICE") {
    return (ADVICE);
  } else if (command == "WEATHER") {
    return (WEATHER);
  } else if (command == "RANDOM") {
    // Randomly select JOKE, ADVICE, or INSULTME
    int randomIndex = std::rand() % 3;
    switch (randomIndex) {
      case 0:
        return (JOKE);
      case 1:
        return (ADVICE);
      case 2:
        return (INSULTME);
    }
  }
  return (UNKNOWN_BOT_COMMAND);
}

BotRequest Bot::parseRequest(const std::string& requestBuffer) {
  Log::printLog(INFO_LOG, BOT_L, "Handling a new message from Server...");
  std::stringstream ss(requestBuffer);
  std::string clientNickname;
  std::string commandStr;
  std::string arg;
  ss >> clientNickname >> commandStr >> commandStr >> commandStr >> arg;

#ifdef DEBUG
  debugLogServerMessageSplit(clientNickname, commandStr, arg);
#endif
  if (commandStr[0] != '!')
    return (BotRequest(clientNickname.substr(1), UNKNOWN_BOT_COMMAND, ""));
  commandStr = commandStr.substr(1);
  strToUpper(&commandStr);
  BotRequest newRequest(clientNickname.substr(1), selectCommand(commandStr),
                        arg);
  return (newRequest);
}
