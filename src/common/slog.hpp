//==============================================================
// Copyright(C) 2023-2024 Intel Corporation
// Licensed under the Intel Proprietary License
// =============================================================

/**
 * @brief a header file with logging facility for common samples
 * @file slog.hpp
 */
#ifndef CVOI_COMMON_SLOG_HPP_
#define CVOI_COMMON_SLOG_HPP_

#pragma once

#include <iostream>
#include <string>

namespace slog
{
#if 1
enum COLOR
{
    RESET = 0,
    BLUE = 1,
    GREEN = 2,
    YELLOW = 3,
    RED = 4,
    WHITE = 5,
};

#else
    // the following are UBUNTU/LINUX ONLY terminal color codes.
    #define RESET "\033[0m"
    #define BLACK "\033[30m"              /* Black */
    #define RED "\033[31m"                /* Red */
    #define GREEN "\033[32m"              /* Green */
    #define YELLOW "\033[33m"             /* Yellow */
    #define BLUE "\033[34m"               /* Blue */
    #define MAGENTA "\033[35m"            /* Magenta */
    #define CYAN "\033[36m"               /* Cyan */
    #define WHITE "\033[37m"              /* White */
    #define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
    #define BOLDRED "\033[1m\033[31m"     /* Bold Red */
    #define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
    #define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
    #define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
    #define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
    #define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
    #define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#endif

/**
 * @class LogStreamEndLine
 * @brief The LogStreamEndLine class implements an end line marker for a log
 * stream
 */
class LogStreamEndLine
{
};

static constexpr LogStreamEndLine endl;

/**
 * @class LogStream
 * @brief The LogStream class implements a stream for sample logging
 */
class LogStream
{
    std::string _prefix;
    std::ostream* _log_stream;
    bool _new_line;
    int _color_id;

public:
    /**
     * @brief A constructor. Creates an LogStream object
     * @param prefix The prefix to print
     */
    LogStream(const std::string& prefix, std::ostream& log_stream, const int color_id = -1)
        : _prefix(prefix), _new_line(true), _color_id(color_id)
    {
        _log_stream = &log_stream;
    }

    /**
     * @brief A stream output operator to be used within the logger
     * @param arg Object for serialization in the logger message
     */
    template <class T>
    LogStream& operator<<(const T& arg)
    {
        if (_new_line)
        {
            setLineColor();
            (*_log_stream) << "[ " << _prefix << " ] ";
            _new_line = false;
        }

        (*_log_stream) << arg;
        return *this;
    }

    // Specializing for LogStreamEndLine to support slog::endl
    LogStream& operator<<(const LogStreamEndLine& arg)
    {
        _new_line = true;
        resetLineColor();
        (*_log_stream) << std::endl;
        return *this;
    }

    void setLineColor()
    {
        switch (_color_id)
        {
            case BLUE:
                (*_log_stream) << "\033[34m";
                break;
            case GREEN:
                (*_log_stream) << "\033[32m";
                break;
            case YELLOW:
                (*_log_stream) << "\033[33m";
                break;
            case RED:
                (*_log_stream) << "\033[31m";
                break;
            case WHITE:
                (*_log_stream) << "\033[37m";
                // (*_log_stream) << "\033[1m\033[37m";
                break;
            default:
                break;
        }
    }

    void resetLineColor()
    {
        if (_color_id > 0)
        {
            (*_log_stream) << "\033[0m"; // RESET
        }
    }
};

class NullStream
{
public:
    NullStream() {}

    NullStream(const std::string& prefix, std::ostream& log_stream)
    {
        (void)prefix;
        (void)log_stream;
    }

    template <class T>
    NullStream& operator<<(const T& arg)
    {
        return *this;
    }
};

#ifdef LOG_LEVEL_DEBUG
static LogStream debug("DEBUG", std::cout, GREEN);
#else
static NullStream debug;
#endif
static LogStream verbose("VERBOSE", std::cout, WHITE);
static LogStream info("INFO", std::cout, BLUE);
static LogStream warn("WARNING", std::cout, YELLOW);
static LogStream err("ERROR", std::cerr, RED);

} // namespace slog
#endif // CVOI_COMMON_SLOG_HPP_
