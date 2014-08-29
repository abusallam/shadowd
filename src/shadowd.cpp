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

#include <string>
#include <iostream>

#include "shadowd.h"
#include "config.h"
#include "log.h"
#include "database.h"

void swd::shadowd::init(int argc, char** argv) {
	/**
	 * Since this is a daemon for Unix-like operating system environments the
	 * command line interface is the heart of the user control. So the first
	 * thing we have to do is to process the command line and generate a
	 * configuration based on the parameters.
	 * Some parameters cause the program to exit here (help, version...).
	 */
	swd::config::i()->parse_command_line(argc, argv);

	/**
	 * Additional settings can be loaded via a config file. They get mixed up
	 * with the existing settings, but they do not overwrite them. This way the
	 * command line parameters have a higher priority.
	 */
	if (swd::config::i()->defined("config")) {
		swd::config::i()->parse_config_file(swd::config::i()->get<std::string>("config"));
	}

	/**
	 * Validate the configuration values. If an important value is missing or
	 * incorrect there is no way we can continue. In this cases there will be
	 * a swd::exceptions::config_exception.
	 */
	swd::config::i()->validate();

	/* First things first: Set log file. */
	if (swd::config::i()->defined("log")) {
		swd::log::i()->open_file(swd::config::i()->get<std::string>("log"));
	}

	/**
	 * Detach the process, close handlers and make this a real daemon. It is also
	 * possible to chroot the process for the case that there is a coding bug.
	 * Nobody is perfect, take precautions and limit access to resources. Of course
	 * changing the root directory requires root privileges.
	 */
	if (swd::config::i()->defined("daemonize")) {
		daemon_.detach();

		if (swd::config::i()->defined("chroot")) {
			daemon_.change_root(swd::config::i()->get<std::string>("chroot"));
		}

		daemon_.write_pid(swd::config::i()->get<std::string>("pid"));
	}

	/* Connect to the database. */
	swd::database::i()->connect(
		swd::config::i()->get<std::string>("db-driver"),
		swd::config::i()->get<std::string>("db-host"),
		swd::config::i()->get<std::string>("db-port"),
		swd::config::i()->get<std::string>("db-user"),
		swd::config::i()->get<std::string>("db-password"),
		swd::config::i()->get<std::string>("db-name"),
		swd::config::i()->get<std::string>("db-encoding")
	);

	/**
	 * Initialize the server. It is not possible to connect yet, since we haven't
	 * assigned threads to the threadpool. This is good, because (maybe) this code
	 * is still running with root privileges and this would be a big no-no.
	 * This method requires root privileges to bind to ports below 1024.
	 */
	server_.init();

	/* Now drop the privileges. They are not required and very dangerous. */
	if (swd::config::i()->defined("group")) {
		daemon_.set_group(swd::config::i()->get<std::string>("group"));
	}

	if (swd::config::i()->defined("user")) {
		daemon_.set_user(swd::config::i()->get<std::string>("user"));
	}
}

void swd::shadowd::start() {
	/* This adds threads to the threadpool and keeps everything running. */
	server_.start(swd::config::i()->get<int>("threads"));
}

int main(int argc, char** argv) {
	try {
		swd::shadowd shadowd;

		shadowd.init(argc, argv);
		shadowd.start();
	} catch (swd::exceptions::core_exception& e) {
		swd::log::i()->send(swd::critical_error, e.what());
		return 1;
	} catch (swd::exceptions::config_exception& e) {
		std::cerr << "Configuration error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
