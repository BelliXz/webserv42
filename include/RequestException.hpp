#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <stdexcept>
#include <string>

class RequestException : public std::runtime_error 
{
    private:
        int statusCode;
        std::string message;

    public:
        RequestException(int code, const std::string &msg)
            : std::runtime_error(msg), statusCode(code), message(msg) {}

        // ต้องประกาศ destructor ให้ตรง spec ของ base class
        virtual ~RequestException() _GLIBCXX_NOTHROW {}

        int getStatusCode() const { return statusCode; }
        const std::string &getMessage() const { return message; }
};

#endif // REQUESTEXCEPTION_HPP

