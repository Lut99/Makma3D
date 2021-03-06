/* LOGGER.cpp
 *   by Lut99
 *
 * Created:
 *   25/07/2021, 14:11:06
 * Last edited:
 *   05/10/2021, 16:04:56
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Contains the Logger class, which logs different things based on the
 *   severity level.
**/

#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(NDEBUG)
#include <execinfo.h>
#include <stdio.h>
#endif
#include <iostream>

#include "tools/Logger.hpp"

using namespace std;
using namespace Makma3D;
using namespace Makma3D::Tools;


/***** GLOBALS *****/
/* Global instance of the Logger everyone uses. */
Logger Makma3D::logger(std::cout, std::cerr, Verbosity::none);





/***** LOGGER::FATAL CLASS *****/
/* Constructor for the Fatal exception, which takes a message describing what happened. */
Logger::Fatal::Fatal(const std::string& message) :
    message(message)
{}





/***** LOGGER CLASS *****/
/* Constructor for the Logger class, which takes an output stream to write its non-messages to, an output stream to write error messages to and a verbosity level. */
Logger::Logger(std::ostream& stdos, std::ostream& erros, Verbosity verbosity) :
    stdos(&stdos),
    erros(&erros),

    verbosity(verbosity),
    start_time(chrono::system_clock::now())
{}

/* Copy constructor for the Logger class. */
Logger::Logger(const Logger& other) :
    stdos(other.stdos),
    erros(other.erros),

    verbosity(other.verbosity),
    start_time(chrono::system_clock::now()),

    thread_names(other.thread_names)
{}

/* Move constructor for the Logger class. */
Logger::Logger(Logger&& other) :
    stdos(std::move(other.stdos)),
    erros(std::move(other.erros)),

    verbosity(std::move(other.verbosity)),
    start_time(other.start_time),

    thread_names(other.thread_names)
{}

/* Destructor for the Logger class. */
Logger::~Logger() {}



#if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(NDEBUG)
/* Private static helper function that prints the current stack on unix systems.
 * Code from: https://stackoverflow.com/a/77336/5270125 */
void Logger::print_stacktrace() {
    void* helper_array[10];
    size_t size;

    // Get pointers for all entries on the stack
    size = backtrace(helper_array, 10);

    // Print out the frames to stderror
    fprintf(stderr, "Stacktrace:\n");
    backtrace_symbols_fd(helper_array, size, fileno(stderr));
}
#endif

/* Private static helper function that pads a float to always have three decimals. */
std::string Logger::pad_float(float value) {
    // Convert to string
    std::stringstream sstr;
    sstr << value;

    // Try to find the decimal point
    size_t dec_pos = sstr.str().find_last_of('.');
    if (dec_pos == std::string::npos) {
        sstr << ".000";
    } else {
        std::string decimals = sstr.str().substr(dec_pos + 1);
        for (size_t i = decimals.size(); i < 3; i++) {
            sstr << '0';
        }
    }

    // Done, return
    return sstr.str();
}


/* Links the given thread ID to the given name. */
void Logger::set_thread_name(const std::thread::id& tid, const std::string& name) {
    // Get a lock
    std::unique_lock<std::mutex> local_lock(this->lock);

    // Set the name, overwriting it if it exist or else creating a new entry
    this->thread_names[tid] = name;
}

/* Removes the name mapping for the given thread ID. */
void Logger::unset_thread_name(const std::thread::id& tid) {
    // Get a lock
    std::unique_lock<std::mutex> local_lock(this->lock);

    // Try to find the ID
    std::unordered_map<std::thread::id, std::string>::iterator iter = this->thread_names.find(tid);
    if (iter != this->thread_names.end()) {
        // Remove it
        this->thread_names.erase(iter);
    }
}



/* Sets the output stream for this Logger. */
void Logger::set_output_stream(std::ostream& new_os) {
    // Get the lock for this Logger
    std::unique_lock<std::mutex> local_lock(this->lock);

    // Set the output stream
    this->stdos = &new_os;
}

/* Sets the error stream for this Logger. */
void Logger::set_error_stream(std::ostream& new_os) {
    // Get the lock for this Logger
    std::unique_lock<std::mutex> local_lock(this->lock);

    // Set the error stream
    this->erros = &new_os;
}

/* Sets the internal verbosity to the given value. */
void Logger::set_verbosity(Verbosity new_value) {
    // Get the lock for this Logger
    std::unique_lock<std::mutex> local_lock(this->lock);

    // Set the error stream
    this->verbosity = new_value;
}



/* Swap operator for the Logger class. */
void Tools::swap(Logger& l1, Logger& l2) {
    using std::swap;

    swap(l1.stdos, l2.stdos);
    swap(l1.erros, l2.erros);

    swap(l1.verbosity, l2.verbosity);
    swap(l1.start_time, l2.start_time);

    swap(l1.thread_names, l2.thread_names);
}
