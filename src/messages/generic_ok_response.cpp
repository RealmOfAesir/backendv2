/*
    Realm of Aesir
    Copyright (C) 2019 Michael de Lang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "generic_ok_response.h"
#include <spdlog/spdlog.h>
#include <rapidjson/writer.h>

using namespace lotr;
using namespace rapidjson;

string const generic_ok_response::type = "error_response";

generic_ok_response::generic_ok_response(string message) noexcept
        : message(move(message)) {

}

string generic_ok_response::serialize() const {
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);

    writer.StartObject();

    writer.String(KEY_STRING("type"));
    writer.String(type.c_str(), type.size());

    writer.String(KEY_STRING("message"));
    writer.String(message.c_str(), message.size());

    writer.EndObject();
    return sb.GetString();
}

optional<generic_ok_response> generic_ok_response::deserialize(rapidjson::Document const &d) {
    if (!d.HasMember("type") ||
        !d.HasMember("message")) {
        spdlog::warn("[generic_ok_response] deserialize failed");
        return nullopt;
    }

    if(d["type"].GetString() != type) {
        spdlog::warn("[generic_ok_response] deserialize failed wrong type");
        return nullopt;
    }

    return generic_ok_response(
            d["message"].GetString()
    );
}
