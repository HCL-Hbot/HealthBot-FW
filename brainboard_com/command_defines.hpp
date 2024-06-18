/*
 * Copyright 2024 JUNE - HAN University of Applied Sciences, Health Concept Lab
 * Project HealthBot
 * Authors: Victor Hogeweij, Marelle Vink and Richard Kroesen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef COMMAND_DEFINES_HPP
#define COMMAND_DEFINES_HPP

#include <PicoLed.hpp>
#include <embedded_cli.h>

namespace COM {

const static bool addCliCommandBinding(EmbeddedCli* cli, const char* name, const char* help, bool tokenizeArgs, void* context, void (*binding)(EmbeddedCli* cli, char* args, void* context)) {
    CliCommandBinding commandBinding = { name, help, tokenizeArgs, context, binding };
    return embeddedCliAddBinding(cli, commandBinding);
}

} // namespace COM
#endif // COMMAND_DEFINES_HPP