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

/// remove unused parameter warning.
#define UNUSED_PARAMETER(X) ((void)X)
#define KE_UNUSED(X) UNUSED_PARAMETER(X)

/*
* The following set of macros allow the creation of unique variable names within the current scope.
* Taken from: http://stackoverflow.com/questions/2419650/c-c-macro-template-blackmagic-to-generate-unique-name
*/
/// boiler-plate
#define CONCATENATE_DETAIL(x, y) x##y
/// boiler-plate
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
/// @return an unique name base on the given 'variable' name by concatenating __COUNTER__.
#define MAKE_UNIQUE_NAME_WITH(x) CONCATENATE(x, __COUNTER__)
/// @return an unique name which is constructed by concatenating 'UNIQUE_VAR_NAME' and __COUNTER__ together.
#define MAKE_UNIQUE_NAME() CONCATENATE(UNIQUE_VAR_NAME_, __COUNTER__)
/// @return an unique name base on the given 'variable' name by concatenating __COUNTER__.
#define KE_MAKE_UNIQUE_NAME_WITH(x) CONCATENATE(x, __COUNTER__)
/// @return an unique name which is constructed by concatenating 'UNIQUE_VAR_NAME' and __COUNTER__ together.
#define KE_MAKE_UNIQUE_NAME() CONCATENATE(UNIQUE_VAR_NAME_, __COUNTER__)