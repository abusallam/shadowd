/**
 * Shadow Daemon -- Web Application Firewall
 *
 *   Copyright (C) 2014-2020 Hendrik Buchwald <hb@zecure.org>
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
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#include "whitelist_filter.h"

void swd::whitelist_filter::set_id(const int& id) {
    id_ = id;
}

int swd::whitelist_filter::get_id() const {
    return id_;
}

void swd::whitelist_filter::set_regex(const std::string& regex) {
    regex_.set_expression(regex, boost::regex::icase | boost::regex::mod_s);
}

bool swd::whitelist_filter::matches(const std::string& input) const {
    return regex_search(input, regex_);
}
