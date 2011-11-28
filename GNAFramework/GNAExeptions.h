#ifndef GNAEXEPTIONS_H
#define	GNAEXEPTIONS_H

namespace GNAFramework {


    class GNAException {
    public:

        GNAException();
        GNAException(const char * message);
        GNAException(const char * message, GNAException *innerException);

        GNAException *InnerException() const;

        virtual const char *Message() const;

        virtual const GNAException *GetBaseException() const;

        virtual const char *getExceptionName() const;

        virtual char *toString() const;

        char *ExceptionTrace() const;

        virtual ~GNAException();

    protected:
        char * message;
        GNAException *innerException;
    };

    class SystemException : public GNAException {
    public:

        SystemException()
        : GNAException() {};

        SystemException(const char * message)
        : GNAException(message) {};

        SystemException(const char * message, GNAException *innerException)
        : GNAException(message, innerException) {};

        virtual const char *getExceptionName() const
        {    return "SystemException"; }


    private:
    };

    class NotImplementedException : public SystemException {
    public:

        NotImplementedException()
        : SystemException() {};

        NotImplementedException(const char * message)
        : SystemException(message) {};

        NotImplementedException(const char * message, GNAException *innerException)
        : SystemException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "NotImplementedException"; }
        
    private:
    };

    class InvalidOperationException : public SystemException {
    public:

        InvalidOperationException()
        : SystemException() {};

        InvalidOperationException(const char * message)
        : SystemException(message) {};

        InvalidOperationException(const char * message, GNAException *innerException)
        : SystemException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "InvalidOperationException"; }

    private:
    };

    class ObjectDisposedException : public InvalidOperationException {
    public:

        ObjectDisposedException()
        : InvalidOperationException() {};

        ObjectDisposedException(const char * message)
        : InvalidOperationException(message) {};

        ObjectDisposedException(const char * message, GNAException *innerException)
        : InvalidOperationException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "ObjectDisposedException"; }

    private:
    };

    class ArgumentException : public SystemException {
    public:

        ArgumentException()
        : SystemException() {};

        ArgumentException(const char * message)
        : SystemException(message) {};

        ArgumentException(const char * message, GNAException *innerException)
        : SystemException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "ArgumentException"; }

    private:
    };

    class ArgumentNullException : public ArgumentException {
    public:

        ArgumentNullException()
        : ArgumentException() {};

        ArgumentNullException(const char * message)
        : ArgumentException(message) {};

        ArgumentNullException(const char * message, GNAException *innerException)
        : ArgumentException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "ArgumentNullException"; }

    private:
    };

    class ArgumentOutOfRangeException : public ArgumentException {
    public:

        ArgumentOutOfRangeException()
        : ArgumentException() {};

        ArgumentOutOfRangeException(const char * message)
        : ArgumentException(message) {};

        ArgumentOutOfRangeException(const char * message, GNAException *innerException)
        : ArgumentException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "ArgumentOutOfRangeException"; }

    private:
    };

    class ExecutionEngineException : public SystemException {
    public:

        ExecutionEngineException()
        : SystemException() {};

        ExecutionEngineException(const char * message)
        : SystemException(message) {};

        ExecutionEngineException(const char * message, GNAException *innerException)
        : SystemException(message, innerException) {}

        virtual const char *getExceptionName() const
        {    return "ExecutionEngineException"; }

    private:
    };

    class ContentLoadException : public GNAException {
    public:

        ContentLoadException()
        : GNAException() {};

        ContentLoadException(const char * message)
        : GNAException(message) {};

        ContentLoadException(const char * message, GNAException *innerException)
        : GNAException(message, innerException) {};

        virtual const char *getExceptionName() const
        {    return "ContentLoadException"; }


    private:
    };
}

#endif	/* GNAEXEPTIONS_H */

