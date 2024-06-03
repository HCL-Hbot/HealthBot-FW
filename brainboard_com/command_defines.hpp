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