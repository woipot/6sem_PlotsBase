#ifndef MODELEXCEPTION_H
#define MODELEXCEPTION_H

#include <exception>
#include <QString>

class ModelException : public std::exception
{
    QString additional_msg_;

public:
    ModelException(QString addit_msg)
    {
        additional_msg_ = addit_msg;
    }

    QString get_msg()
    {
        return additional_msg_;
    }

    const char *what() const noexcept override
    {
        return "Graph Model Exception";
    }
};

#endif // MODELEXCEPTION_H

