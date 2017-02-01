/*
Copyright (c) 2017,  Benjamin YanXiang Huang
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of yxbh nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include "macros.hpp"

#include <utility>

namespace ke
{

    namespace impl
    {

        /** @brief ScopeFunc implementation impl. */
        template<typename ... T> class ScopeFuncImpl;
        /** @brief ScopeFunc implementation impl. */
        template<> class ScopeFuncImpl<> {};
        /** @brief ScopeFunc implementation impl. */
        template<typename First_T, typename ... Rest_T>
        class ScopeFuncImpl<First_T, Rest_T...>
            : private ScopeFuncImpl<Rest_T...>
        {
        private:
            using BaseType = ScopeFuncImpl<Rest_T...>;

            First_T m_Functor;

        public:
            ScopeFuncImpl(First_T && p_Functor, Rest_T && ... p_Functors)
                : BaseType(std::forward<Rest_T>(p_Functors)...), m_Functor(p_Functor) {}

            ~ScopeFuncImpl(void) { m_Functor(); }
        }; // ScopeFuncImpl

    } // impl ns

    /**
    * @class ScopeFunc
    * @brief A RAII class that takes a single functor object, executing the object's operator()
    * in the ScopeFunc object's destructor when the ScopeFunc object is destroyed.
    */
    template<typename ... Functor_T>
    class ScopeFunc
        : private impl::ScopeFuncImpl<Functor_T...>
    {
    private:
        using BaseType = impl::ScopeFuncImpl<Functor_T...>;

    public:
        ScopeFunc(Functor_T && ... p_Functors)
            : BaseType(std::forward<Functor_T>(p_Functors)...) {}
    };

    /**
    * Utility function for creating a ScopeFunc object with an arbitrary amount of functors.
    *
    * Sample usuage:
    * @code
    *      auto scope_func = makeScopeFunc([](void){ std::cout << "end of scope" << std::endl; });
    * @endcode
    * @return a ScopeFunc object.
    */
    template<typename ... Functor_T>
    ScopeFunc<Functor_T...> makeScopeFunc(Functor_T && ... p_rrFunctor)
    {
        return ScopeFunc<Functor_T...>(std::forward<Functor_T>(p_rrFunctor) ...);
    }

} // ke ns

  /**
  * Utility macro function for creating a ScopeFunc object with an arbitrary amount of functors.
  * This function takes care of creating the ScopeFunc object for you so you don't declare one.
  * makeScopeFunc is used internally.
  *
  * Sample usuage:
  * @code
  *      MakeScopeFunc([](void){ std::cout << "end of scope" << std::endl; });
  * @endcode
  * @return a ScopeFunc object.
  */
#define MakeScopeFunc(...) auto MAKE_UNIQUE_NAME() = ke::makeScopeFunc(__VA_ARGS__);
#define KE_MAKE_SCOPEFUNC(...) auto KE_MAKE_UNIQUE_NAME() = ke::makeScopeFunc(__VA_ARGS__);