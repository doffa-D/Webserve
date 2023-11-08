#ifndef	LOGGER_HPP
#define LOGGER_HPP

#include "String.hpp"

class	Logger
{
	public :
		template <class Streamer, typename Value>
		static void info(Streamer& stream , const String& message, const Value& value)
		{
			stream << "\033[0;36m[INFO]\033[0m : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void error(Streamer& stream , const String& message, const Value& value)
		{
			stream << "\033[0;31m[ERROR]\033[0m : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void success(Streamer& stream , const String& message, const Value& value)
		{
			stream << "\033[0;32m[SUCCESS]\033[0m : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void warn(Streamer& stream , const String& message, const Value& value)
		{
			stream << "\033[0;33m[WARN]\033[0m : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void debug(Streamer& stream , const String& message, const Value& value)
		{
			stream << "\033[0;37m[DEBUG]\033[0m : " << message << value << "\033[0m" << std::endl;
		}
};

#endif
