/*
    Phil Schatzmann 2021 https://www.pschatzmann.ch/

    https://github.com/pschatzmann/pico-arduino/tree/main/Arduino/
    
*/

#pragma once

#include "Stream.h"

/**
 * @brief A simple Logger that writes messages dependent on the log level
 * 
 */

class PicoLogger
{
public:
    /**
         * @brief Supported log levels
         * 
         */
    enum LogLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };

    PicoLogger() {}
    ~PicoLogger() {}

    // activate the logging
    virtual void begin(Stream &out, LogLevel level = Error)
    {
        this->log_stream_ptr = &out;
        this->log_level = level;
    }

    // checks if the logging is active
    virtual bool isLogging()
    {
        return log_stream_ptr != nullptr;
    }

    virtual void error(const char *str, const char *str1 = nullptr, const char *str2 = nullptr)
    {
        log(Error, str, str1, str2);
    }

    virtual void info(const char *str, const char *str1 = nullptr, const char *str2 = nullptr)
    {
        log(Info, str, str1, str2);
    }

    virtual void warning(const char *str, const char *str1 = nullptr, const char *str2 = nullptr)
    {
        log(Warning, str, str1, str2);
    }

    virtual void debug(const char *str, const char *str1 = nullptr, const char *str2 = nullptr)
    {
        log(Debug, str, str1, str2);
    }

    // write an message to the log
    virtual void log(LogLevel current_level, const char *str, const char *str1 = nullptr, const char *str2 = nullptr)
    {
        if (log_stream_ptr != nullptr)
        {
            if (current_level >= log_level)
            {
                log_stream_ptr->print((char *)str);
                if (str1 != nullptr)
                {
                    log_stream_ptr->print(" ");
                    log_stream_ptr->print(str1);
                }
                if (str2 != nullptr)
                {
                    log_stream_ptr->print(" ");
                    log_stream_ptr->print(str2);
                }
                log_stream_ptr->println();
                log_stream_ptr->flush();
            }
        }
    }

protected:
    Stream *log_stream_ptr;
    LogLevel log_level;
};

extern PicoLogger Logger; // Support for logging
