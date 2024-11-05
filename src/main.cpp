/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:50:56 by mbernard          #+#    #+#             */
/*   Updated: 2024/11/05 11:36:11 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include "../includes/tests.hpp"

void checkArgs(int port, const std::string& password) {
  if (port <= 1080 || port > 65535) {
    std::cerr << "Invalid port number (ports accepted : 1081 to 65535)"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  if (password.length() > 1000) {
    std::cerr << "Password too long" << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main(int ac, char** argv) {
  {
  /* ------------------------ unitary tests ------------------------*/
/* join */
#ifdef TEST
  Test test;
  int port = std::atoi(argv[1]);
  std::string password = argv[2];
  Server ser(port, password);

  if (ac > 3 && std::string(argv[3]) == "join") {
    test.testJoinChannel(ser);
  } else
    test.testAll(ser);
  exit(EXIT_SUCCESS);

#endif
  /* ------------------------ end of unitary tests ------------------------*/
  }
  if (ac != 3) {
    std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
    exit(EXIT_FAILURE);
  }

  int port = std::atoi(argv[1]);
  std::string password = argv[2];
  checkArgs(port, password);
  Server ser(port, password);

  try {
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);
    ser.runServer();
  } catch (const std::exception& e) {
    std::cerr << RED << e.what() << RESET << '\n';
    ser.closeServer();
    exit(EXIT_FAILURE);
  }
  ser.closeServer();
  std::cout << "The Server is closed" << std::endl;
  exit(EXIT_SUCCESS);
}
