/* Copyright 2024 <faboussa>************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faboussa <faboussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:29:39 by yusengok          #+#    #+#             */
/*   Updated: 2024/11/05 11:18:10 by faboussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_SERVERCONFIG_HPP_
#define INCLUDES_SERVERCONFIG_HPP_

#include <string>

#include "./utils.hpp"

/* Server information */
#define SRV_NAME "ircserv.localhost"
#define SRV_VERSION "1.0.0"
#define NETWORK_NAME "42IRC"

/* Modes */
#define USER_MODES "ior"      // List of available modes for user (Example)
#define CHANNEL_MODES "itkl"  // List of available modes for channel

/* Prefix */
#define OP_PREFIX "@"  // Prefix for channel operator
#define GENERAL_CHAN_SYMBOL "="

/* ISUPPORT Parameters */
#define PREFIX "(o)@"  // List of available client statuses in channels
#define TOKEN_PREFIX (std::string("PREFIX=") + PREFIX)

#define NICKLEN 9
#define TOKEN_NICKLEN (std::string("NICKLEN=") + toString(NICKLEN))

#define CHANLIMIT_ 2 // Example of channel limit (10 channels max for a user)
#define CHANLIMIT ("#:" + toString(10))
#define TOKEN_CHANLIMIT (std::string("CHANLIMIT=") + CHANLIMIT)

#define CHANMODES "i,t,k,l"
#define TOKEN_CHANMODES (std::string("CHANMODES=") + CHANMODES)

#define CHAN_PREFIX_CHAR '#'
#define CHAN_PREFIX "#"
#define TOKEN_CHANTYPES (std::string("CHANTYPES=") + CHAN_PREFIX)

#define UNIQUE ""
#define TOKEN_UNIQUE (std::string("UNIQUE") + UNIQUE)

#define TOKENS \
  (TOKEN_PREFIX + std::string(" ") + TOKEN_NICKLEN + " " + TOKEN_CHANLIMIT + \
  " " + TOKEN_CHANMODES + " " + TOKEN_CHANTYPES + " " + TOKEN_UNIQUE)
// https://modern.ircdocs.horse/#rplisupport-parameters

#endif  // INCLUDES_SERVERCONFIG_HPP_
