#pragma once

namespace algorand {
namespace vertices {

    class Request
    {
    public:
        virtual ~Request() {}
    };

    class Response
    {
    public:
        virtual ~Response() {}

        void SetSuccessful(bool InSuccessful) { Successful = InSuccessful; }
        bool IsSuccessful() const { return Successful; }

    private:
        bool Successful;
    };

}
}