/*
 *  Copyright (c) 2018 Henry G. Stratmann III
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#pragma once

#include <cassert>
#include <type_traits>

template<class T>
class lazy_val
{
public:
    lazy_val() = default;

    lazy_val(const lazy_val<T> &other) = delete;
    lazy_val<T> &operator=(const lazy_val<T> &other) = delete;

    ~lazy_val()
    {
        if (_constructed)
        {
            this->ptr()->~T();
            _constructed = false;
        }
    }

    template <class ...Args>
    void init(Args&& ...args)
    {
        assert(!_constructed);
        new (reinterpret_cast<void *>(&_storage)) T(std::forward<Args>(args)...);
        _constructed = true;
    }

    inline T *ptr() noexcept
    {
        assert(_constructed);
        return reinterpret_cast<T *>(&_storage);
    }

    inline const T *ptr() const noexcept
    {
        assert(_constructed);
        return reinterpret_cast<const T *>(&_storage);
    }

    inline T &ref() noexcept
    {
        return *ptr();
    }

    inline const T &ref() const noexcept
    {
        return *ptr();
    }

    inline T *operator->() noexcept
    {
        assert(_constructed);
        return reinterpret_cast<T *>(&_storage);
    }

    inline const T *operator->() const noexcept
    {
        assert(_constructed);
        return reinterpret_cast<const T *>(&_storage);
    }

    inline explicit operator bool() const noexcept
    {
        return static_cast<bool>(ref());
    }

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
    bool _constructed = false;
};

#define LAZY_VALUE_BINOP(OP)                                                \
    template<class T>                                                       \
    inline bool operator OP (const lazy_val<T> &x, const lazy_val<T> &y)    \
    {                                                                       \
        return x.ref() OP y.ref();                                          \
    }                                                                       \
    template<class T>                                                       \
    inline bool operator OP (const lazy_val<T> &x, const T &y)              \
    {                                                                       \
        return x.ref() OP y;                                                \
    }

#define LAZY_VALUE_UNOP(OP)                                                 \
    template<class T>                                                       \
    inline T operator OP (const lazy_val<T> &x)                             \
    {                                                                       \
        return OP x.ref();                                                  \
    }

LAZY_VALUE_BINOP(==)
LAZY_VALUE_BINOP(!=)
LAZY_VALUE_BINOP(>)
LAZY_VALUE_BINOP(>=)
LAZY_VALUE_BINOP(<)
LAZY_VALUE_BINOP(<=)

LAZY_VALUE_BINOP(+)
LAZY_VALUE_BINOP(-)
LAZY_VALUE_BINOP(*)
LAZY_VALUE_BINOP(/)
LAZY_VALUE_BINOP(%)
LAZY_VALUE_BINOP(<<)
LAZY_VALUE_BINOP(>>)

LAZY_VALUE_UNOP(-)
LAZY_VALUE_UNOP(!)
LAZY_VALUE_UNOP(~)
