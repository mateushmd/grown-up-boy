#pragma once

#include <array>

#include "types.hpp"

namespace emulator::components
{
    template <typename T>
    class Register
    {
    protected:
        T value = 0;

    public:
        Register() = default;
        virtual ~Register() = default;

        virtual T get() { return value; };
        virtual void set(const T rvalue) { value = rvalue; };
        virtual void inc() { ++value; };
        virtual void dec() { --value; };
    };

    class Register8 : public Register<byte>
    {
    public:
        using Register::Register;
    };

    class Register16 : public Register<word>
    {
    public:
        using Register::Register;
    };

    class RegisterPair : public Register<word>
    {
    private:
        Register8 &hi, &lo;

    public:
        RegisterPair(Register8 &, Register8 &);
        word get() override;
        void set(const word) override;
        void inc() override;
        void dec() override;
    };
}