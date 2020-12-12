//! Command Parser implementation file.
/**
 * @file      CommandParser.cpp
 * @author    Stanislav Sotnikov (stanislav.sotnikov145@gmail.com)
 */

#include "CommandParser.h"

namespace CommandParser{

    const std::string& execute(const std::string &command){

        // Check if help requested.
        if(command[0] == '?')
            return help_str;

        // Parse tokens into a vector
        std::vector<std::string> tokens = {};

        const auto args_valid = parse_tokens<' ', 5>(command, tokens);

        if(not args_valid)
            return invalid_args;
        
        // Lookup the first positioned token in the command table.
        // In case found, returns a poiter to the corresponding command container.
        const auto executable = std::find_if(command_table.begin(), 
                                             command_table.end(), 
                                             [&tokens](const Cmd &cmd_table_entry){

                                    return cmd_table_entry.token == tokens.front();
                                });

        // Check if given token exists in the command table.
        if(executable == command_table.end())
            return unknown_command;

        tokens.erase(tokens.begin());

        // Execute the corresponding command from the table.
        executable->execute(tokens);
  
        // Return status of the executed command.
        return HardwareDriver::get_status();
    }
}