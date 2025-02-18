#pragma once

#include <array>

#include "types.hpp"

namespace emulator::components
{
    template <typename T>
    class Register
    {
    public:
        virtual ~Register() = default;

        virtual T get() = 0;
        virtual void set(const T) = 0;
        virtual void inc() = 0;
        virtual void dec() = 0;
    };

    class Register8 : public Register<byte>
    {
    private:
        byte value;

    public:
        Register8() = default;

        byte get() override;
        void set(const byte) override;
        void inc() override;
        void dec() override;
    };

    class Register16 : public Register<word>
    {
    private:
        word value;

    public:
        Register16() = default;

        word get() override;
        void set(const word) override;
        void inc() override;
        void dec() override;
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