/**
 * Shadow Daemon -- High-Interaction Web Honeypot
 *
 *   Copyright (C) 2014 Hendrik Buchwald <hb@zecure.org>
 *
 * This file is part of Shadow Daemon. Shadow Daemon is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHARED_H
#define SHARED_H

#include <string>

#include "build_config.h"

#define STATUS_OK 1
#define STATUS_BAD_REQUEST 2
#define STATUS_BAD_SIGNATURE 3
#define STATUS_BAD_JSON 4
#define STATUS_ATTACK 5

#define STATUS_ACTIVATED 1
#define STATUS_DEACTIVATED 2
#define STATUS_PENDING 2

namespace swd {
	namespace exceptions {
		/**
		 * @brief Critical exception in on of the core components.
		 */
		struct core_exception : public std::exception {
			/**
			 * @brief Information about the exception.
			 */
			std::string s;

			/**
			 * @brief Constructs an exception.
			 */
			core_exception(std::string ss) : s(ss) {}

			/**
			 * @brief Destroys the exception.
			 */
			~core_exception() throw () {}

			/**
			 * @brief Return the information.
			 */
			const char* what() const throw() { return s.c_str(); }
		};

		/**
		 * @brief Critical exception in the configuration.
		 */
		struct config_exception : public std::exception {
			/**
			 * @brief Information about the exception.
			 */
			std::string s;

			/**
			 * @brief Constructs an exception.
			 */
			config_exception(std::string ss) : s(ss) {}

			/**
			 * @brief Destroys the exception.
			 */
			~config_exception() throw () {}

			/**
			 * @brief Return the information.
			 */
			const char* what() const throw() { return s.c_str(); }
		};

		/**
		 * @brief Uncritical database exception.
		 */
		struct database_exception : public std::exception {
			/**
			 * @brief Information about the exception.
			 */
			std::string s;

			/**
			 * @brief Constructs an exception.
			 */
			database_exception(std::string ss) : s(ss) {}

			/**
			 * @brief Destroys the exception.
			 */
			~database_exception() throw () {}

			/**
			 * @brief Return the information.
			 */
			const char* what() const throw() { return s.c_str(); }
		};

		/**
		 * @brief Uncritical connection exception.
		 */
		struct connection_exception : public std::exception {
			/**
			 * @brief Information about the exception.
			 */
			int code_;

			/**
			 * @brief Constructs an exception.
			 */
			connection_exception(int code) : code_(code) {}

			/**
			 * @brief Destroys the exception.
			 */
			~connection_exception() throw () {}

			/**
			 * @brief Return the information.
			 */
			int code() const throw() { return code_; }
		};
	}
}

#endif /* SHARED_H */