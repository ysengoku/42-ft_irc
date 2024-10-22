/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:50:56 by faboussa          #+#    #+#             */
/*   Updated: 2024/10/17 11:53:10 by mbernard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include "../includes/colors.hpp"

bool Server::_signal = false;

Server::Server(int port) : _port(port) {
  _signal = false;
  _socketFd = -1;
}

void Server::runServer() {
  createSocket();
  std::cout << GREEN << "Server started on port " << _port << RESET
            << std::endl;
  while (!_signal) {
  }
  std::cout << "Server shutting down..." << std::endl;
}

void shrink_to_fit(std::vector<struct pollfd>& vec) {
    std::vector<struct pollfd>(vec).swap(vec);
}
void Server::closeServer() {
  for (size_t i = 0; i < _clients.size(); ++i) {
    std::cout << RED << "Client <" << _clients[i].getFd();
    std::cout << "> Disconnected" << RESET << std::endl;
    close(_clients[i].getFd());
  }
  _clients.clear();
  if (_socketFd != -1) {
    std::cout << RED << "Server <" << _socketFd;
    std::cout << "> Disconnected" << RESET << std::endl;
    close(_socketFd);
    _socketFd = -1;
  }
  for (size_t i = 0; i < _pollFds.size(); ++i) {
    if (_pollFds[i].fd != -1) {
      close(_pollFds[i].fd);
    }
  }
  _pollFds.clear();
  shrink_to_fit(_pollFds);
}

void Server::signalHandler(int signal) {
  if (signal == SIGINT || signal == SIGQUIT) {
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::_signal = true;
  }
}

void Server::clearClient(int fd) {
  for (size_t i = 0; i < _pollFds.size(); i++) {
    if (_pollFds[i].fd == fd) {
      _pollFds.erase(_pollFds.begin() + i);
      break;
    }
  }
  for (size_t i = 0; i < _clients.size(); i++) {
    if (_clients[i].getFd() == fd) {
      _clients.erase(_clients.begin() + i);
      break;
    }
  }
}

void Server::createSocket() {
  _address.sin_family = AF_INET;
  _address.sin_addr.s_addr = INADDR_ANY;
  _address.sin_port = htons(_port);

  _socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_socketFd == -1) {
    throw std::runtime_error("Failed to create socket");
  }

  int en = 1;
  if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    throw std::runtime_error("Failed to set option SO_REUSEADDR on socket");

  if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error("Failed to set option O_NONBLOCK on socket");

  if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
    throw std::runtime_error("Failed to bind socket");

  if (listen(_socketFd, 10) == -1)
    throw std::runtime_error("Failed to listen on socket");

  struct pollfd newPoll;
  newPoll.fd = _socketFd;
  newPoll.events = POLLIN;
  newPoll.revents = 0;
  _pollFds.push_back(newPoll);
}
